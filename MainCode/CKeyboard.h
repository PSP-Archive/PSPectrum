#ifndef _KEYBOARD_
#define _KEYBOARD_

#include "MiscUtils.h"
#include "types.h"
#include "EmulatorDefines.h"

#include "CMGETgaFile.h"
#include "CMGEVramScreen.h"
#include "fuse/keyboard.h"

class CMGEScreen;

#define CURSOR48_SPEEDX    3
#define CURSOR48_SPEEDY    3

class CKeyboard
{
public:
    virtual ~CKeyboard() {}
    
	struct TKeyDef
	{
		char	   m_Name[15];
		bool	   m_bKeepPressed;
		keyboard_key_name m_Key1;
		keyboard_key_name m_Key2;
	};

	static CKeyboard* CKeyboard::Create(EMachineType eMahcineType);
    
	//iXPos and iYPos are screen coordinates
	virtual bool Init(int iXPos, int iYPos)=0;
    virtual void End();

    void Render(CMGEScreen*);
    
    void Run();
    
	bool GetActive()			 { return m_bActive; }
	void SetActive(bool bActive) 
	{
		m_bActive=bActive; 
		//Release last key pressed
		if(!m_bActive)
		{
			if(m_iLastKeyPressedIndex!=-1)
			{
				keyboard_release(m_KeyDefs[m_iLastKeyPressedIndex].m_Key1);
				keyboard_release(m_KeyDefs[m_iLastKeyPressedIndex].m_Key2);
			}
		}
	}

	//Called to finish keyboard input, etc... (Mainly release all keys)
    virtual void Close() {this->CheckKeyRelease();}

	void UploadTex(CMGEVramScreen* pScr,int iX, int iY);
	
	void SetTransparency(float fTrans) {m_fTrans=fTrans;}
	void SetCursorSize(u16 iCursorSize) {m_iCursorSize=iCursorSize; }

	static const TKeyDef* GetKeyDefFromName(EMachineType eMachineType,const char* sName);
	static const TKeyDef* GetKeyDefFromIndex(EMachineType eMachineType,int index)
	{
		return &m_KeyDefs[index];
	}
	static int	GetIndexFromName(EMachineType eMachineType,const char* sName);
	

protected:
    CKeyboard() : m_bOK(false){}
	
	virtual bool Init(int iXPos, int iYPos,const char* sImgFileName);
	
	enum
	{
		MAX_CURSOR_COLORS=6,
		MAX_KEYS =58,
		MAX_KEYS_48K = 40,
		MAX_KEYS_128K = 58
	};
	
    struct TKeyBox
	{
		CBox  m_Box;
		TKeyBox* m_pNext;
		TKeyBox() {m_pNext=0;}
	};
	
	virtual void CheckKeyPress(bool bFirstPress);
    virtual void CheckKeyRelease();
	int GetKeyUnderCursor();
	inline int GetRelCursorPosX() { return m_iXPosCursor-m_iXPos;}
	inline int GetRelCursorPosY() { return m_iYPosCursor-m_iYPos;}
    TKeyBox* m_pKeyMapping;
	bool	m_bActive;
	u16*	m_pKeybTex;	//Only used in psp ver
	u16     m_iXPos;    //Screen coord
    u16     m_iYPos;    //Screen coord
    u16     m_iXPosCursor;
    u16     m_iYPosCursor;
    u16		m_iCursorSize;
	u16		m_wCursorColorIndex;
	u16     m_CursorColors[MAX_CURSOR_COLORS];
	u16		m_iNumKeys;
	float	m_fTrans;

	CMGETgaFile*    m_pKeyboardImage;
    int				m_iKeepPressedKey; //What key is the one that we left pressed (special key)
	
    bool    m_bOK;

    //Key that was pressed and will produce a keyrelease later when released
    //-1 if no currentkey pressed
    int     m_iLastKeyPressedIndex; 
	
	static	TKeyDef m_KeyDefs[MAX_KEYS];
};

#endif


