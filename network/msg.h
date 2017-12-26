#ifndef __KIT_MSG_H__
#define __KIT_MSG_H__

// 0-128 net系统级别packet id, 其他作为协议id(+128)
// 普通协议自定义，由0开始

#include "kitsys.h"

namespace kit {

const uint16_t PT_CONNECT = 1;  // 每个client连接上了
const uint16_t PT_KEEP_ALIVE = 2; // 心跳包

const uint16_t PT_PACKET_ID_MAX = 0xFFFF;


// custom protocol id

} // namespace kit

#endif

