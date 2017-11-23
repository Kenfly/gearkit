#include "Session.h"

namespace kit {

uint32_t Session::session_id_count_ = 0;

Session::Session()
: sid_(++session_id_count_)
{
}

Session::~Session()
{
}

} // namespace kit
