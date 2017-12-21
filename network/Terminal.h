#ifndef __KIT_TERMINAL_H__
#define __KIT_TERMINAL_H__

#include <vector>
#include "Ref.h"
#include "kitsys.h"

namespace kit {

class Terminal : public Ref
{
public:
	Terminal();
    virtual ~Terminal();

protected:
    virtual bool baseInit();
};

} // namespcae kit

#endif
