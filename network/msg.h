#ifndef __KIT_MSG_H__
#define __KIT_MSG_H__

// net系统级别协议, 协议号为负数
// 普通协议自定义，由0开始

#include "kitsys.h"

namespace kit {

const int32_t PT_CONNECT = -1;  //每个client连接上了

} // namespace kit

#endif

