#ifndef _KEYBOARD_128K_
#define _KEYBOARD_128K_

#include "MiscUtils.h"
#include "CKeyboard.h"
#include "types.h"

class CKeyboard128k : public CKeyboard
{
public:
    virtual ~CKeyboard128k() {}
    virtual bool Init(int iXPos, int iYPos);
    virtual void End();


};

#endif
