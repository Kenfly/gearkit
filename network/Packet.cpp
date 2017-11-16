#include "Packet.h"
#include "Buffer.h"

namespace kit {

void getPacketInfo(Buffer* buf, uint32_t& seed, uint32_t& lenght)
{
    PacketHeader header;
    buf >> (uint32_t)header;
    getPacketInfo(header, seed, lenght);
}

} // namespace kit

