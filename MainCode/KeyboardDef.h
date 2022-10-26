#ifndef __KEYBOARD_DEF__
#define __KEYBOARD_DEF__

#define NUM_MAX_KEYS 58

struct TKeybKey
{
    int		   m_iKey    //Estado de la linea para esta tecla
    int		   m_iKeyLine; //Linea de la matriz
	char	   m_ASCII;
	//CBox       m_Box;
	char	   m_Name[15];
	
	bool	   m_bKeepPressed;
	TKeybDef()
	{
		m_bKeepPressed=false;
	}

};

static TKeybKey KeyDefs[NUM_MAX_KEYS];

#endif