#include "Ref.h"
#include "RefPool.h"
#include <assert.h>

namespace kit {

Ref::Ref()
: reference_count_(1)
, parent_(nullptr)
{
}

Ref::~Ref()
{
    clearChildren();
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
    return this;
}

void Ref::update()
{
    int cnt = (int)children_.size();
    for (int i = 0; i != cnt; ++i)
    {
        children_[i]->update();
    }
}

void Ref::addChild(Ref* child)
{
    if (children_.empty())
    {
        children_.reserve(5);
    }
    child->retain();
    child->parent_ = this;
    children_.push_back(child);
}

void Ref::delChild(Ref* child)
{
    for(RefVec::iterator ix = children_.begin(); ix != children_.end(); ++ix)
    {
        if ((*ix) == child)
        {
            children_.erase(ix);
            child->parent_ = nullptr;
            child->release();
            break;
        }
    }
}

void Ref::clearChildren()
{
    for(RefVec::iterator ix = children_.begin(); ix != children_.end(); ++ix)
    {
        (*ix)->parent_ = nullptr;
        (*ix)->release();
    }
    children_.clear();
}

void Ref::removeFromParent()
{
    if (parent_)
    {
        parent_->delChild(this);
    }
}

} // namespace kit

