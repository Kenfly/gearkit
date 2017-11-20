#include "Packet.h"
#include "Buffer.h"

namespace kit {

void readPacketInfo(Buffer* buf, PacketHeader& header)
{
    (*buf) >> header.seed;
    (*buf) >> header.length;
}

void writePacketInfo(Buffer* buf, const PacketHeader& header)
{
    (*buf) << header.seed;
    (*buf) << header.length;
}

} // namespace kit

