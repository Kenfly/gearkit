#ifndef __KIT_SOCKET_H__
#define __KIT_SOCKET_H__

#include "Ref.h"
#include "kitsys.h"
#include "Packet.h"
#include "Queue.h"
#include "List.h"

#define DSOCKERR		-1

namespace kit {

class SockAddr;
class Buffer;

class ISocket : public Ref
{
public:
	ISocket();
	virtual ~ISocket();

    virtual void init(void);
	virtual void init(int32_t family, int32_t type, int32_t protocol);
    virtual void init(int32_t sock);

	void open();
	void close();

	int32_t connect(SockAddr* addr);
	int32_t shutdown(int32_t mode);
	int32_t bind(SockAddr* addr);
	int32_t listen(int32_t count);
	int32_t accept(SockAddr* addr);
	int32_t getErrno();


	int32_t send(const char* buf, int32_t size, int32_t mode = 0);
	int32_t recv(char* buf, int32_t size, int32_t mode = 0);
	int32_t sendTo(const char* buf, int32_t size, int32_t mode, SockAddr* addr);
	int32_t recvFrom(char* buf, int32_t size, int32_t mode, SockAddr* addr);

	int32_t ioctl(unsigned long cmd, char* argp);
	int32_t setOption(int32_t level, int32_t optname, const char* optval, int32_t optlen);
	int32_t getOption(int32_t level, int32_t optname, char* optval, int32_t* optlen);

	void setHandle(int32_t _sock);
	inline int32_t getHandle() const { return sock_; }

    void setAddr(SockAddr* addr);
    SockAddr* getAddr() const { return addr_; }

	// flush recv/send列表，尽可能地清空缓冲
	// -1断开,否则返回总字节数
	int32_t flushRecv();
	int32_t flushSend();

    // 发送一个包
    // 返回-1，发送错误，返回0，发送完毕, 返回1，还可以继续发送
    int32_t sendPacket(Buffer* buf);
    // 处理接收到的包
    void dealRecv();

    bool valid() const { return valid_; }

    // 有效标志，没效后删除
    bool valid_;
    // 可发送标志,不可发送放到队列里去
    bool readyOut_;
protected:
    // recv/send一个buffer
    // 成功返回已接收的字节数，-1掉线
    int32_t recvBuffer(Buffer* buf);
    int32_t sendBuffer(Buffer* buf);
    // 清空recv/send缓冲区,抛弃掉错误包
    void recvClear();
	void sendClear();
    // 接收缓冲区
    // 返回-1，接收错误，返回0, 结束或未收到完整包，否则返回接收字节数
    int32_t doRecv();
    // 发送缓冲区
    // 返回-1，发送错误, 返回0, 结束获未发送完整包，否则返回接收字节数
    int32_t doSend();

protected:
	int32_t sock_;
    SockAddr* addr_;

private:
    // 包种子，如果客户端发来的包与服务端的不对应，则认为包不可信
    uint32_t packet_seed_;
    // 统计有效包数量
    uint32_t recv_count_;
    // 接收发送包头
    Buffer* recv_head_buf_;
    Buffer* send_head_buf_;
    // 一个完整包接收buf
    Buffer* recv_buf_;
    Buffer* send_buf_;
    Buffer* send_bufs_;

    // 存储各个完整的包
    typedef Queue<Buffer*, 200> BufferQue;
    // 处理队列
    BufferQue recv_que_;
    BufferQue send_que_;
    // 缓存包
    List<Buffer*> recv_list_;
    List<Buffer*> send_list_;
};

} // namespcae kit

#ifdef PLATFORM_LINUX
#include <platform/linux/Socket_linux.h>
#elif defined PLATFORM_WINDOWS
#include <platform/win/Socket_win.h>
#endif


#endif

