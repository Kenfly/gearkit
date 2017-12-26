#include "Protocol.h"


namespace kit {

// pt data table
PTDataTable::~PTDataTable()
{
    clear();
}

void PTDataTable::serialize(Buffer* buffer)
{
    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        (*ix)->serialize(buffer);
    }
}

void PTDataTable::unserialize(Buffer* buffer)
{
    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        (*ix)->unserialize(buffer);
    }
}

void PTDataTable::addData(PTData* data)
{
    data->retain();
    datas.push_back(data);
}

void PTDataTable::delData(PTData* data)
{
    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        if (*ix == data)
        {
            (*ix)->release();
            datas.erase(ix);
            break;
        }
    }
}

void PTDataTable::clear()
{
    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        (*ix)->release();
    }
    datas.clear();
}


// protocol
Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

void Protocol::init(int32_t _pid)
{
    pid = _pid;
}

} // namespace kit

