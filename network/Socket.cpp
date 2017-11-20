#include "Socket.h"
#include "SockAddr.h"
#include "netsys.h"
#include "base.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

namespace kit {

ISocket::ISocket()
: valid_(false) 
, sock_(DSOCKERR)
, addr_(NULL)
, readyOut_(false)
, recv_head_buf_(NULL)
, send_head_buf_(NULL)
, recv_buf_(NULL)
, send_buf_(NULL)
{
}

ISocket::~ISocket()
{
    if (sock_ != DSOCKERR)
	    close();
    if (addr_)
    {
        addr_->release();
        addr_ = NULL;
    }
    if (recv_head_buf_)
    {
        recv_head_buf_->release();
        recv_head_buf_ = NULL;
    }
    if (send_head_buf_)
    {
        send_head_buf_->release();
        send_head_buf_ = NULL;
    }
    if (recv_buf_)
    {
        recv_buf_->release();
        recv_buf_ = NULL;
    }
}

void ISocket::init(void)
{
    recv_head_buf_ = new Buffer();
    recv_head_buf_->init(PACKET_HEADER_SIZE);
    send_head_buf_ = new Buffer();
    send_head_buf_->init(PACKET_HEADER_SIZE);

    // 生成随机种子
    packet_seed_ = ::rand();
}

void ISocket::init(int32_t family, int32_t type, int32_t protocol)
{
    init();
    if (sock_ != DSOCKERR)
	    close();
	sock_ = (int32_t)::socket(family, type, protocol);
	if (sock_ < 0) 
    {
        sock_ = DSOCKERR;
        return;
    }

	open();
}

void ISocket::init(int32_t sock)
{
    init();
    setHandle(sock);
}

void ISocket::open()
{
	int32_t noblock = 1;
	int32_t revalue = 1;

	// set socket to nonblock
	ioctl(FIONBIO, (char*)(&noblock));

	// set socket reuse addr
	setOption(SOL_SOCKET, SO_REUSEADDR, (char*)&revalue, (int32_t)sizeof(revalue));

	// set socket nodelay
#ifndef PLATFORM_LINUX
	setOption(IPPROTO_TCP, TCP_NODELAY, (char*)&revalue, (int32_t)sizeof(revalue));
#else
    setOption(SOL_TCP, TCP_NODELAY, (char*)&revalue, (int32_t)sizeof(revalue));
#endif
}

void ISocket::close()
{
	if (sock_ == DSOCKERR) 
        return;

	::close(sock_);

	sock_ = DSOCKERR;
}

void ISocket::setHandle(int32_t sock)
{
    if (sock_ != DSOCKERR)
        close();

	sock_ = sock;
	open();
}

int32_t ISocket::connect(SockAddr* addr)
{
	socklen_t len = (socklen_t)sizeof(struct sockaddr);
	int32_t ret = ::connect(sock_, *addr, len);

    setAddr(addr);

    return ret;
}

int32_t ISocket::shutdown(int32_t mode)
{
	if (sock_ == DSOCKERR)
        return 0;

    int32_t ret = ::shutdown(sock_, mode);
    if(ret == 0 && mode == 2)
        sock_ = DSOCKERR;
	return ret;
}

int32_t ISocket::bind(SockAddr* addr)
{
    setAddr(addr);
	socklen_t len = (socklen_t)sizeof(struct sockaddr);
	return ::bind(sock_, *addr, len);
}

int32_t ISocket::listen(int32_t count)
{
	return ::listen(sock_, count);
}

int32_t ISocket::accept(SockAddr* addr)
{
	socklen_t len = (socklen_t)sizeof(struct sockaddr);
	return ::accept(sock_, *addr, &len);
}

int32_t ISocket::ioctl(unsigned long cmd, char* argp)
{
	return ::ioctl(sock_, cmd, argp);
}

int32_t ISocket::setOption(int32_t level, int32_t optname, const char* optval, int32_t optlen)
{
	return ::setsockopt(sock_, level, optname, optval, optlen);
}

int32_t ISocket::getOption(int32_t level, int32_t optname, char* optval, int32_t* optlen)
{
	socklen_t len = (socklen_t)((optlen) ? *optlen : 0);
	int32_t retval;
	retval = getsockopt(sock_, level, optname, optval, &len);
	if (optlen) 
		*optlen = len;

	return retval;
}

int32_t ISocket::getErrno()
{
	return errno;
}

int32_t ISocket::doRecv()
{
    // 先把缓存列队的放进que
    Buffer* p = NULL;
    while (recv_list_.count() > 0)
    {
        if (!recv_list_.front(p) || !recv_que_.push(p))
            break;
        recv_list_.pop(p);
    }

    // 接收包头
    int32_t ret = 0;
    uint32_t rest_header_size = recv_head_buf_->getWritableSize();
    if (rest_header_size > 0)
    {
        ret = recvBuffer(recv_head_buf_);
        // 接收错误，断开连接了
        if (ret == -1)
            return -1;
        // 还未接收完毕
        if (ret < rest_header_size)
            return 0;
        // 接收完包头
        if (ret == rest_header_size)
        {
            PacketHeader header;
            readPacketInfo(recv_head_buf_, header);
            if (header.seed != packet_seed_)
            {
                // 收到错误包
                recvClear();
            }
            if (recv_buf_)
                recv_buf_->release();
            recv_buf_ = new Buffer();
            recv_buf_->init(header.length);
        }
    }
    // 接收包体
    uint32_t rest_size = recv_buf_->getWritableSize();
    if (rest_size > 0)
    {
        ret = recvBuffer(recv_buf_);
        // 接收错误，断开连接了
        if (ret == -1)
            return -1;
        // 还未接收完毕
        if (ret < rest_size)
            return 0;
    }

    // 接收完包体
    // 放进包队列，等待处理
    if (recv_list_.count() > 0 || !recv_que_.push(recv_buf_))
    {
        recv_list_.push(recv_buf_);
    }
    recv_buf_ = NULL;
    recv_head_buf_->reset();
    return 1;
}

int32_t ISocket::doSend()
{
    uint32_t rest_size = 0;
    uint32_t nsend = 0;
    if (send_bufs_ == NULL)
    {
        send_bufs_ = g_BufPool->createBuffer(PACKET_SIZE);
        rest_size = PACKET_SIZE;
    }
    else
        rest_size = send_bufs_->getWritableSize();

    uint32_t rest_header_size = send_head_buf_->getReadableSize();
    if (rest_header_size > 0)
    {
        int len = send_bufs_->writeBuffer(send_head_buf_->read_cur_, min(rest_header_size, rest_size));
        if (len == -1)
            return -1;
        if (len == 0)
            return 0;
        nsend = len;
        rest_size -= len;
    }
    if (send_buf_ == 0)
    {
        if (!send_que_.pop(send_buf_) || !send_list_.pop(send_buf_))
        {
            return nsend;
        }
        PacketHeader header;
        header.seed = packet_seed_;
        header.length = send_buf_->getReadableSize();
        int len = send_buf_->getWritableSize();
        send_head_buf_->reset();
    }

    // 先把缓存列队的放进que
    Buffer* p = send_buf_;
    while (p || send_que_.pop(p) || send_list_.pop(p))
    {
        // 发送包头
        rest_size = send_head_buf_->getReadableSize();
    }
    while (send_list_.count() > 0)
    {
        if (!send_list_.front(p) || !send_que_.push(p))
            break;
        send_list_.pop(p);
    }
    g_BufPool->destroyBuffer(sbuf);

    return 1;
}

int32_t ISocket::sendPacket(Buffer* buf)
{
    // 先把缓存里的发送完毕
    int32_t ret = 0;
    do {
        ret = doSend();
    } while(ret > 0);

    buf->retain();
    if (send_list_.count() > 0 || !send_que_.push(buf))
    {
        send_list_.push(buf);
    }
}

int32_t ISocket::send(const char* buf, int32_t size, int32_t mode)
{
	return ::send(sock_, buf, size, mode);
}

int32_t ISocket::recv(char* buf, int32_t size, int32_t mode)
{
	return ::recv(sock_, buf, size, mode);
}

int32_t ISocket::recvFrom(char* buf, int32_t size, int32_t mode, SockAddr *addr)
{
    socklen_t len = sizeof(struct sockaddr);
	return ::recvfrom(sock_, buf, size, mode, *addr, &len);
}

int32_t ISocket::sendTo(const char* buf, int32_t size, int32_t mode, SockAddr* addr)
{
    socklen_t len = sizeof(struct sockaddr);
	return ::sendto(sock_, buf, size, mode, *addr, len);
}

void ISocket::setAddr(SockAddr* addr)
{
    if (addr_ == addr)
        return;
    if (addr_)
        addr_->release();
    addr_ = addr;
    addr_->retain();
}

int32_t ISocket::recvBuffer(Buffer* buf)
{
    uint32_t size = buf->getWritableSize();
    uint32_t nrecv = 0;
    int32_t len = 0;
    while (nrecv < size)
    {
        len = this->recv(buf->write_cur_, size - nrecv, 0);
        if (len < 0)
            return -1;
        if (len == 0)
            break;
        nrecv += len;
        buf->skipWrite(len);
    }
    return nrecv;
}

int32_t ISocket::sendBuffer(Buffer* buf)
{
    uint32_t size = buf->getReadableSize();
    uint32_t nsend = 0;
    int32_t len = 0;
    while (nsend < size)
    {
        len = this->send(buf->read_cur_, size - nsend, 0);
        if (len < 0)
            return -1;
        if (len == 0)
            break;
        nsend += len;
        buf->skipRead(len);
    }
    return nsend;
}

void ISocket::recvClear()
{
    const int32_t BSIZE = 512;
    char buf[BSIZE];
    int32_t len = -1;
    do {
        len = this->recv(buf, BSIZE, 0);
    } while (len < BSIZE);
}

void ISocket::dealRecv()
{
}

} // namespace kit

