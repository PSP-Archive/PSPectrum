#ifndef _GUI_
#define _GUI_

#include "types.h"
#include "CPsPectrum.h"

class CMGEScreen;
class CMGETgaFile;
class COption;
class CPage;

struct TEmuOptions;

//pApp->PrintText(pScr,"DPAD Move/Change   L Next Page   R Exit",150,250);



class CGUI
{
public:
    
    #define OPTION_X_POS       70
	
	~CGUI();
	void			Init(TEmuOptions* pEmuOptions);
	void            AddPage(CPage* pPage);
    void            NextPage();
    void            PreviousPage();
    void            SetXYPos(int iXPos, int iYPos) {m_iXPos=iXPos, m_iYPos=iYPos;}
    void            Render(CMGEScreen* pScr);
    
    bool            Run(float fTime);
    
	//Look for a option name in all pages (Pages can´t have same names, albeit they´re different)
    int			GetOptionValueIndex(const char* sOptionName);
	const char* GetOptionValueString(const char* sOptionName);
	const char*	GetTextValue(const char* sOptionName);

	//Look for a option name in all pages (Pages can´t have same names, albeit they´re different)    
    void SetOptionValue(const char* sOptionName, int iValIndex);
	void SetTextValue(const char* sOptionName, const char* sTextValue);
	
	virtual void FillEmuOptions();
	

private:
	//Fill TEmuOptions with current options
	void SetDefaultOptions();
	void SetKeyOptions();
	virtual void BuildMenu();
	
	COption* GetOption(const char* sOptionName);
	int		 GetOptionIndex(const char* sOptionName);
    
	int  m_iNumPages;
	CPage*  m_pCurrentPage;
	CPage*  m_pFirstPage;
	CPage*  m_pLastPage;
    TEmuOptions* m_pEmuOptions;
	//Positions on screen of Pages
    //Pages will print relative to this
    int  m_iXPos;
    int  m_iYPos;
    CMGETgaFile* m_pBG;
    float m_fAccumTime;
};


#endif
