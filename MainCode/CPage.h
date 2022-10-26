#ifndef __CPAGE__
#define __CPAGE__

class COption;
class CMGEScreen;
class CMGESprite;

class CPage
{
public:    
	#define MAX_STRING_LENGTH 100
	#define MAX_OPTIONS_SCREEN 12
	#define SELECTABLE_OFFSET_X 70
	#define OPTIONS_Y_OFFSET 50
	#define kDELAY 0.2f

	CPage() : m_bOK(false) {}
	virtual ~CPage() { End(); }
	virtual bool        Init(const char* sTitle);
	virtual void		End();
	void        AddOption(COption* pOption);
	void        NextOption();
	void        PreviousOption();
	int			GetOptionValueIndex(const char*);
	const char* GetOptionValueString(const char* sOptionName);
	COption*	GetOption(const char*sName);
	int			GetOptionIndex(const char*sName);
	void        SetOptionValue(const char* sOptionName, int iIndex);
	const char* GetEditBoxText(const char* sOptionName);
	void		SetTextBoxText(const char* sOptionName, const char* sText);
	COption*    GetCurrentOption() { return m_pCurrentOption;}
	void        SetTitle(const char* sTitle);
	void        SetXYPos(int iXPos, int iYPos) {m_iXPos=iXPos, m_iYPos=iYPos;}
	void        Render(CMGEScreen* pScr, int iParentX, int iParentY);
	CPage*		GetNext() { return m_pNext;}
	CPage*		GetPrev() { return m_pPrev;}
	void		SetNext(CPage* pNext) {m_pNext=pNext;}    
	void		SetPrev(CPage* pPrev) {m_pPrev=pPrev;}    

	virtual		void	Run(float fTime);
	virtual		void	DoActions() {}

protected:
	char m_sTitle[MAX_STRING_LENGTH];
	int  m_iNumOptions;
	COption*  m_pCurrentOption;
	COption*  m_pFirstOptionOnScreen;
	COption*  m_pFirstOption;
	COption*  m_pLastOption;
	CPage*	  m_pNext;
	CPage*	  m_pPrev;
	int  m_iXPos;
	int  m_iYPos;
	float m_fAccumTime;
	bool	m_bOK;
	//Order of this option on screen (to draw the selected cursor)
	int		  m_iCurrentOptionPos;
	
	bool m_bKeybFocus; //if true page receives all control events if not selected control

	//Static
	static CMGESprite* m_pBar;
	static CMGESprite* m_pMoreUpIcon;
	static CMGESprite* m_pMoreDownIcon;
}; 


class CGamePage : public CPage
{
public:
	void        Init();
	void		DoActions();
};

class CMachinePage : public CPage
{
public:
	void        Init();
	void		DoActions();
};

class CGeneralPage : public CPage
{
public:
	void        Init();
	void		DoActions();
};

class CPokerPage : public CPage
{
public:
	void        Init();
	void		DoActions();
};


#endif


