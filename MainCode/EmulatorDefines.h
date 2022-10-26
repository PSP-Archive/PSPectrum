#ifndef __PspecturmDefines_h__
#define __PspecturmDefines_h__

#include "Types.h"

enum EJoyActions {JA_UP,JA_DOWN,JA_LEFT,JA_RIGHT,JA_FIRE};

#define	SINCLAIR1_UP "4"
#define	SINCLAIR1_DOWN "3"
#define	SINCLAIR1_LEFT "1"
#define	SINCLAIR1_RIGHT "2"
#define	SINCLAIR1_FIRE "5"


#define	SINCLAIR2_UP "9"
#define	SINCLAIR2_DOWN "8"
#define	SINCLAIR2_LEFT "6"
#define	SINCLAIR2_RIGHT "7"
#define	SINCLAIR2_FIRE "0"

enum ESPECIAL_KEYS_CODES
{
	ENTER_KEY=       13,
	SYMBOLSHIFT_KEY=  0,
	CAPSSHIFT_KEY=   1,
	SPACE_KEY=       32
};


enum EMachineType {	MT_48K=0,MT_128K,MT_128KA,MT_NUM};

#endif
