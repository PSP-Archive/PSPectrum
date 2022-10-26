#ifndef _KEYBOARD_48K_
#define _KEYBOARD_48K_

#include "MiscUtils.h"
#include "CKeyboard.h"
#include "types.h"
#include "EmulatorDefines.h"

class CKeyboard48k : public CKeyboard
{
public:
    virtual ~CKeyboard48k() { End(); }
    virtual bool Init(int iXPos, int iYPos);
    virtual void End();
};

#endif


