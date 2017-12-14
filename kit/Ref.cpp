#include "Ref.h"
#include "RefPool.h"
#include <assert.h>

namespace kit {

Ref::Ref()
: reference_count_(1)
{
}

Ref::~Ref()
{
}

void Ref::retain()
{
    ++reference_count_;
}

void Ref::release()
{
    assert(reference_count_ > 0);

    --reference_count_;
    if (reference_count_ == 0)
        delete this;
}

Ref* Ref::autoRelease()
{
    g_RefPool->getCurrentPool()->addRef(this);
}

} // namespace kit

