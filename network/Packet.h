#ifndef __KIT_PACKET_H__
#define __KIT_PACKET_H__

#include "kitsys.h"

namespace kit {

struct PacketHeader
{
    uint16_t seed;
    uint16_t length;
};

// 包最大在小
const uint32_t PACKET_MAX_SIZE = 0xFFFF;
// 包头大小
const uint32_t PACKET_HEADER_SIZE = sizeof(PacketHeader);

class Buffer;
void readPacketInfo(Buffer* buf, PacketHeader& header);
void writePacketInfo(Buffer* buf, const PacketHeader& header);

} // namespace kit

#endif
