#include "Socket.h"
#include "SockAddr.h"
#include "Buffer.h"
#include "BufferPool.h"
#include "Packet.h"
#include "Session.h"
#include "netsys.h"
#include "base.h"
#include "Logger.h"
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
, recv_bufs_(NULL)
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
    KIT_DESTROY_BUFFER(recv_head_buf_);
    KIT_DESTROY_BUFFER(send_head_buf_);
    KIT_DESTROY_BUFFER(recv_buf_);
    KIT_DESTROY_BUFFER(recv_bufs_);
    KIT_DESTROY_BUFFER(send_buf_);
    KIT_DESTROY_BUFFER(send_bufs_);
    KIT_SAFE_RELEASE(recv_packet_);
    KIT_SAFE_RELEASE(send_packet_);
}

bool ISocket::baseInit()
{
    if (!recv_head_buf_)
        recv_head_buf_ = KIT_CREATE_BUFFER(PACKET_HEADER_SIZE);
    if (!send_head_buf_)
        send_head_buf_ = KIT_CREATE_BUFFER(PACKET_HEADER_SIZE);

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

// 处理一条协议接收
int32_t ISocket::doRecv()
{
    uint32_t total_rest_size = recv_bufs_->getReadableSize();
    uint32_t rest_size = total_rest_size;
    uint32_t nsize;
    printf(".....doRecv..%d\n", rest_size);
    // 创建一条协议包
    if (recv_packet_ == NULL)
    {
        // TODO:: Pakcet pool
        recv_packet_ = Packet::create(false);
        // 重置包头，准备接收
        recv_head_buf_->reset();
    }
    // 接收包头
    uint32_t rest_header_size = recv_head_buf_->getWritableSize();
    if (rest_header_size > 0)
    {
        nsize = std::min(rest_header_size, rest_size);
        recv_head_buf_->writeBuffer(recv_bufs_->readCur(), nsize);
        rest_size -= nsize;
        recv_bufs_->skipRead(nsize);
        rest_header_size -= nsize;
    }
    printf("....rest_header_size:%d, %d\n", rest_header_size, rest_size);
    // 包头还没接收完毕
    if (rest_header_size > 0)
        return SOCKET_RP_BLOCK;
    // 接收完包头，创建buff
    if (! recv_buf_)
    {
        recv_packet_->unpackHeader(recv_head_buf_);
        if (recv_packet_->getSeed() != packet_seed_)
        {
            // 收到错误包
            recvClear();
            KIT_SAFE_RELEASE(recv_packet_);
            return SOCKET_RECV_ERROR;
        }
        nsize = recv_packet_->getLength();
        printf("xxxxxxxxx2222....%d\n", nsize);
        if (nsize > 0)
        {
            recv_buf_ = KIT_CREATE_BUFFER(nsize);
        }
    }
    if (! recv_buf_)
    {
        // 空协议
        recvPacket(recv_packet_);
        KIT_SAFE_RELEASE(recv_packet_);
        return total_rest_size - rest_size;
    }
    // 缓存区已读取空
    if (rest_size == 0)
    {
        return SOCKET_RP_BLOCK;
    }
    // 接收包体
    uint32_t rest_buffer_size = recv_buf_->getWritableSize();
    if (rest_buffer_size > 0)
    {
        nsize = std::min(rest_size, rest_buffer_size);
        recv_buf_->writeBuffer(recv_bufs_->readCur(), nsize);
        recv_bufs_->skipRead(nsize);
        rest_size -= nsize;
        rest_buffer_size -= nsize;
    }
    // 包体还未全部接收
    if (rest_buffer_size > 0)
        return SOCKET_RP_BLOCK;
    // 包体接收完毕
    recv_packet_->setBuffer(recv_buf_);
    KIT_SAFE_RELEASE(recv_buf_);
    recvPacket(recv_packet_);
    KIT_SAFE_RELEASE(recv_packet_);
    return total_rest_size - rest_size;
}

// 处理一条协议发送
int32_t ISocket::doSend()
{
    uint32_t total_rest_size = send_bufs_->getWritableSize();
    uint32_t rest_size = total_rest_size;
    uint32_t nsize;
    // 取出一条协议包
    if (send_packet_ == NULL)
    {
        if (!send_que_.pop(send_packet_))
        {
            return SOCKET_SP_CLEAR;
        }
		// 读出包头, 准备发送
        send_head_buf_->reset();
        send_packet_->setSeed(packet_seed_);
        send_packet_->packHeader(send_head_buf_);
        // 读出内容buf，准备发送
        KIT_DESTROY_BUFFER(send_buf_);
        send_buf_ = send_packet_->getBuffer();
        if (send_buf_)
        {
            send_buf_->retain();
            send_packet_->delBuffer();
        }
    }
    // 发送包头
    uint32_t rest_header_size = send_head_buf_->getReadableSize();
    if (rest_header_size > 0)
    {
		nsize = std::min(rest_header_size, rest_size);
        send_bufs_->writeBuffer(send_head_buf_->readCur(), nsize);
        rest_size -= nsize;
		send_head_buf_->skipRead(nsize);
		rest_header_size -= nsize;
    }
	// 包头还没发送完毕
	if (rest_header_size > 0)
		return SOCKET_SP_BLOCK;

    if (! send_buf_)
    {
        // 空协议
        KIT_SAFE_RELEASE(send_packet_);
        return total_rest_size - rest_size;
    }
    // 缓存区已写满
    if (rest_size == 0)
        return SOCKET_SP_BLOCK;
	// 发送buff
	uint32_t rest_buffer_size = send_buf_->getReadableSize();
	if (rest_buffer_size > 0)
	{
		nsize = std::min(rest_size, rest_buffer_size);
		send_bufs_->writeBuffer(send_buf_->readCur(), nsize);
        send_buf_->skipRead(nsize);
		rest_size -= nsize;
		rest_buffer_size -= nsize;
	}
	// 包体还未全部发送
	if (rest_buffer_size > 0)
		return SOCKET_SP_BLOCK;
    // 包体发送完毕
    KIT_SAFE_RELEASE(send_packet_);
    KIT_DESTROY_BUFFER(send_buf_);

	if (rest_size == 0)
		return SOCKET_SP_BLOCK;
    return total_rest_size - rest_size;
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
    // 放进包队列，等待处理
    KIT_SAFE_RETAIN(pack);
    recv_que_.push(pack);
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
	int32_t n = ::send(sock_, buf, size, mode);
    DBG("_______send_______[%d/%d]", n, size);
    return n;
}

int32_t ISocket::recv(char* buf, int32_t size, int32_t mode)
{
	int32_t n = ::recv(sock_, buf, size, mode);
    DBG("_______recv_______[%d/%d]", n, size);
    return n;
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
    uint32_t nsize = buf->getWritableSize();
    uint32_t nrecv = 0;
    int32_t len = 0;
    while (nrecv < nsize)
    {
        len = this->recv(buf->writeCur(), nsize - nrecv, 0);
        if (len < 0 && getErrno() != KIT_WOULDBLOCK)
            return SOCKET_RECV_ERROR;
        if (len <= 0)
            break;
        nrecv += len;
        buf->skipWrite(len);
        if (len < int32_t(nsize - nrecv))
            break;
    }
    return nrecv;
}

int32_t ISocket::sendBuffer(Buffer* buf)
{
    uint32_t nsize = buf->getReadableSize();
    uint32_t nsend = 0;
    int32_t len = 0;
    while (nsend < nsize)
    {
        len = this->send(buf->readCur(), nsize - nsend, 0);
        if (len < 0 && getErrno() != KIT_WOULDBLOCK)
            return SOCKET_SEND_ERROR;
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
    printf("------flushRecv--------\n");
    int32_t ret = 0;
    bool need_recv = true;
    if (recv_bufs_ == NULL)
        recv_bufs_ = KIT_CREATE_BUFFER(PACKET_SIZE);
	do {
        if (need_recv)
        {
            int32_t len = recv_bufs_->getWritableSize();
            ret = recvBuffer(recv_bufs_);
            printf("_______recv_bufs:%d, %d\n", len, ret);
            if (ret == SOCKET_RECV_ERROR)
                break;
            need_recv = ret == len;
        }
		ret = doRecv();
        printf(".......%d\n", ret);
		if (ret == SOCKET_RECV_ERROR)
            break;
	} while (ret > 0 || need_recv);
	return ret;
}

int32_t ISocket::flushSend()
{
	int32_t ret = 0;
	if (send_bufs_ == NULL)
		send_bufs_ = KIT_CREATE_BUFFER(PACKET_SIZE);
	do {
		ret = doSend();
		if (ret == SOCKET_SEND_ERROR)
			break;
		if (send_bufs_->getWritableSize() == 0 || ((ret == SOCKET_SP_CLEAR || ret == SOCKET_SP_BLOCK) && send_bufs_->getReadableSize() > 0))
		{
            // send_bufs_已满或装不下一条协议
            if (this->sendBuffer(send_bufs_) == SOCKET_SEND_ERROR)
            {
                ret = SOCKET_SEND_ERROR;
                break;
            }
		}
        printf(".......%d\n", ret);
	} while (ret > 0);
	if (ret == SOCKET_SEND_ERROR || send_bufs_->getReadableSize() == 0)
	{
        KIT_DESTROY_BUFFER(send_bufs_);
	}
	return ret;
}

} // namespace kit

