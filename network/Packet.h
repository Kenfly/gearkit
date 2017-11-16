#ifndef __KIT_PACKET_H__
#define __KIT_PACKET_H__

#include "kitsys.h"

namespace kit {

typedef uint32_t PacketHeader;

// 包最大在小
const uint32_t PACKET_MAX_SIZE = 0xFFFF;
// 包头大小
const uint32_t PACKET_HEADER_SIZE = sizeof(PacketHeader);

// 获取sn,包大小
inline void getPacketInfo(const PacketHeader& header,
        uint32_t& seed,
        uint32_t& length)
{
    seed = header >> 16;
    length = header & 0xFFFF;
}

void getPacketInfo(Buffer* buf, uint32_t& seed, uint32_t& lenght);

} // namespace kit

#endif

