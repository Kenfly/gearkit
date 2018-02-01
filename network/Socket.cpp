#include "Socket.h"
#include "SockAddr.h"
#include "Buffer.h"
#include "BufferPool.h"
#include "Packet.h"
#include "Session.h"
#include "netsys.h"
#include "base.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

namespace kit {

ISocket::ISocket()
: sock_(DSOCKERR)
, session_(NULL)
, addr_(NULL)
, packet_seed_(0)
, recv_count_(0)
, recv_head_buf_(NULL)
, send_head_buf_(NULL)
, recv_buf_(NULL)
, recv_packet_(NULL)
, send_buf_(NULL)
, send_bufs_(NULL)
, send_packet_(NULL)
, deleted(false)
, ready_out(false)
{
}

ISocket::~ISocket()
{
    if (sock_ != DSOCKERR)
	    close();
    KIT_SAFE_RELEASE(addr_);
    KIT_SAFE_RELEASE(recv_head_buf_);
    KIT_SAFE_RELEASE(send_head_buf_);
    KIT_SAFE_RELEASE(recv_buf_);
    KIT_SAFE_RELEASE(recv_packet_);
    KIT_SAFE_RELEASE(send_buf_);
    KIT_SAFE_RELEASE(send_bufs_);
}

bool ISocket::baseInit()
{
    if (!recv_head_buf_)
        recv_head_buf_ = Buffer::create(false);
    recv_head_buf_->init(PACKET_HEADER_SIZE);
    if (!send_head_buf_)
        send_head_buf_ = Buffer::create(false);
    send_head_buf_->init(PACKET_HEADER_SIZE);

    // 生成随机种子
    //packet_seed_ = ::rand();
    return true;
}

void ISocket::init(int32_t family, int32_t type, int32_t protocol)
{
    if (sock_ != DSOCKERR)
	    close();
	sock_ = (SocketID)::socket(family, type, protocol);
	if (sock_ < 0) 
    {
        sock_ = DSOCKERR;
        return;
    }

	open();
}

void ISocket::init(SocketID sock)
{
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

void ISocket::setHandle(SocketID sock)
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
    // 接收包头
	uint32_t nrecv = 0;
    if (recv_packet_ == NULL)
    {
        uint32_t rest_header_size = recv_head_buf_->getWritableSize();
        int32_t len = recvBuffer(recv_head_buf_);
        // 接收错误，断开连接了
        if (len == -1)
        {
            return -1;
        }
        // 还未接收完毕
        if (len < (int32_t)rest_header_size)
            return 0;
		else
        {
			nrecv += len;
			// 接收完包头
            Packet* packet = Packet::create(false);
            packet->unpackHeader(recv_head_buf_);
            recv_head_buf_->reset();
            if (packet->getSeed() != packet_seed_)
            {
                // 收到错误包
                recvClear();
                packet->release();
                return 0;
            }
            recv_packet_ = packet;

            // 创建接收buf
            KIT_SAFE_RELEASE(recv_buf_);
            recv_buf_ = g_BufPoolThread->createBuffer(packet->getLength());
        }
    }

    if (recv_buf_ == NULL || recv_packet_ == NULL)
        return 0;

    // 接收包体
    uint32_t rest_size = recv_buf_->getWritableSize();
    if (rest_size > 0)
    {
        int32_t len = recvBuffer(recv_buf_);
        // 接收错误，断开连接了
        if (len == -1)
            return -1;
        // 还未接收完毕
        if (len < (int32_t)rest_size)
            return 0;
		nrecv += rest_size;
    }

    // 接收完包体
    recv_packet_->setBuffer(recv_buf_);
    KIT_SAFE_RELEASE(recv_buf_);

    recvPacket(recv_packet_);
    KIT_SAFE_RELEASE(recv_packet_);

    return nrecv;
}

int32_t ISocket::doSend()
{
    uint32_t rest_size = 0;
    rest_size = send_bufs_->getWritableSize();

    uint32_t rest_header_size = send_head_buf_->getReadableSize();
    if (rest_header_size > 0)
    {
		int len = std::min(rest_header_size, rest_size);
        send_bufs_->writeBuffer(send_head_buf_->readCur(), len);
        rest_size -= len;
		send_head_buf_->skipRead(len);
		rest_header_size -= len;
    }
	// 包头还没发送完毕
	if (rest_header_size > 0 || rest_size == 0)
		return 0;

    if (send_packet_ == NULL)
    {
        if (!send_que_.pop(send_packet_))
        {
            return 0;
        }
		// 先尝试发送包头
        send_head_buf_->reset();
        send_packet_->setSeed(packet_seed_);
        send_packet_->packHeader(send_head_buf_);
        uint32_t size = std::min(PACKET_HEADER_SIZE, rest_size);
        send_bufs_->writeBuffer(send_head_buf_->readCur(), size);
        send_head_buf_->skipRead(size);
        rest_size -= size;

        // 读出内容buf，准备发送
        KIT_SAFE_RELEASE(send_buf_);
        send_buf_ = send_packet_->getBuffer();
        if (send_buf_)
        {
            send_buf_->retain();
            send_packet_->delBuffer();
        }
    }
	if (rest_size == 0 || send_buf_ == NULL)
		return 0;
	// 发送buff
	uint32_t rest_buffer_size = send_buf_->getReadableSize();
	if (rest_buffer_size > 0)
	{
		uint32_t len = std::min(rest_size, rest_buffer_size);
		send_bufs_->writeBuffer(send_buf_->readCur(), len);
		rest_size -= len;
		rest_buffer_size -= len;
	}
	// 包体还未全部发送
	if (rest_buffer_size > 0)
		return 0;
	else
	{
        KIT_SAFE_RELEASE(send_packet_);
		g_BufPool->destroyBuffer(send_buf_);
		send_buf_ = NULL;
	}

	if (rest_size == 0)
		return 0;
    return 1;
}

int32_t ISocket::sendPacket(Packet* pack)
{
    pack->retain();
    send_que_.push(pack);

    if (ready_out)
        return flushSend();
    return 0;
}

int32_t ISocket::recvPacket(Packet* pack)
{
    if (recv_que_.count() == 0 && session_)
    {
        session_->recvPacket(pack);
        return 0;
    }

    // 放进包队列，等待处理
    KIT_SAFE_RETAIN(pack);
    recv_que_.push(pack);
    pullPacketsToSession();
    return 0;
}

void ISocket::pullPacketsToSession()
{
    if (!session_)
        return;
    Packet* pack;
    int cnt = recv_que_.count();
    for (int i = 0; i != cnt; ++i)
    {
        if (!recv_que_.pop(pack))
            break;
        session_->recvPacket(pack);
        pack->release();
    }
}

int32_t ISocket::send(const char* buf, int32_t size, int32_t mode)
{
    printf("_______send_______%d\n", size);
	return ::send(sock_, buf, size, mode);
}

int32_t ISocket::recv(char* buf, int32_t size, int32_t mode)
{
    printf("_______recv_______%d\n", size);
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
        len = this->recv(buf->writeCur(), size - nrecv, 0);
        if (len < 0 && getErrno() != KIT_WOULDBLOCK)
            return -1;
        if (len <= 0)
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
        len = this->send(buf->readCur(), size - nsend, 0);
        if (len < 0 && getErrno() != KIT_WOULDBLOCK)
            return -1;
        if (len <= 0)
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
    } while (len > 0);
}

void ISocket::sendClear()
{
	// TODO
}

int32_t ISocket::flushRecv()
{
	int32_t len = 0;
	do {
		len = doRecv();
		if (len < 0)
			return -1;
	} while (len > 0);
	return 0;
}

int32_t ISocket::flushSend()
{
	int32_t len = 0;
	int32_t ret = 0;
    int32_t send_len = 0;
	if (send_bufs_ == NULL)
		send_bufs_ = g_BufPool->createBuffer(PACKET_SIZE);
	do {
		len = doSend();
		if (len == -1)
		{
			ret = -1;
			break;
		}
		if (send_bufs_->getWritableSize() == 0 || (len == 0 and send_bufs_->getReadableSize() > 0))
		{
            send_len = this->sendBuffer(send_bufs_);
			if (send_len == -1)
			{
				ret = -1;
				break;
			}
		}
	} while (len > 0);
	if (ret == -1 || send_bufs_->getReadableSize() == 0)
	{
		g_BufPool->destroyBuffer(send_bufs_);
		send_bufs_ = NULL;
	}
	return ret;
}

} // namespace kit

