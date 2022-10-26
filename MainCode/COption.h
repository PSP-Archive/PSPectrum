#ifndef __COPTION__
#define __COPTION__

#include "Types.h"

class CMGEScreen;

class COption
{
public:    
    
	#define MAX_STRING_OPTION 100

	enum EOptionType { OT_SELECT, OT_LABEL, OT_BUTTON, OT_EDITBOX,OT_OTHER };

	COption() : m_bOk(false) {}
	virtual ~COption() { End(); }
	
	virtual bool       Init(const char* sOptionName);
	virtual void	   End(){ m_bOk=false; }
	
	virtual void        Render(CMGEScreen* pScr, int iXPos, int iYPos);
	virtual bool		Run(float fTime) {return false;}

	COption*	GetNext() {return m_pNext;}
	COption*	GetPrev() {return m_pPrev;}
	void		SetNext(COption* pNext) {m_pNext=pNext;}    
	void		SetPrev(COption* pPrev) {m_pPrev=pPrev;}    

	void		SetPosInPage(int iPosInPage) { m_iPosInPage=iPosInPage;}
	int			GetPosInPage() { return m_iPosInPage;}
		
	const char* GetOptionName() {return m_sOptionName;}
	
	EOptionType GetType() { return m_eType;}
	
	COption* GetOption(const char*sName);
	int GetOptionIndex(const char*sName);

protected:
    
    char m_sOptionName[MAX_STRING_OPTION];
	bool m_bIsSeparator;
	EOptionType m_eOptionType;
	COption* m_pNext;
	COption* m_pPrev;
	EOptionType m_eType;
	int m_iPosInPage; //Posicion en la página (indice)
	bool m_bOk;
	float m_fAccumTime;

};


class CSelectOption : public COption
{
public:    
    
    #define SELECTION_OFFSET   240   
	#define kDELAY			   0.2f
	
	~CSelectOption() { End(); }
	
	void        Init(const char* sOptionName, const char** sSelections, u32 dwNumSelections);
    void        NextSelection();
    void        PreviousSelection();
    
    int			GetActiveValueIndex() { return m_dwCurrentSelection; }
	const char* GetActiveValueString() { return m_psSelections[m_dwCurrentSelection];}
	void        SetActiveValue(int iIndex);
   
	virtual void        Render(CMGEScreen* pScr, int iParentX, int iParentY);

	bool		Run(float fTime);
private:
    
    const char **m_psSelections;
    int  m_dwNumSelections;
    int  m_dwCurrentSelection;
    float	m_fAccumTime;
};


class CLabelOption : public COption
{
public:    


	~CLabelOption() { End(); }

	bool        Init(const char* sOptionName);
};


class CButtonOption : public COption
{
public:    


	~CButtonOption() { End(); }

	bool        Init(const char* sOptionName);
};

class CEditOption : public COption
{
public:    


	~CEditOption() { End(); }
	bool Init(const char* sOptionName, const unsigned char* pChars, int iNumChars,int iMaxNumDigits=10);
	bool		Run(float fTime);
	void Render(CMGEScreen* pScr,int iXPos, int iYPos);
	const char* CEditOption::GetText();
	void	CEditOption::SetText(const char* sText);
private:
	bool DoEdit();
	bool CEditOption::ValidatePoke();
	
	int m_piText[256];
	char m_sText[256]; //Text representation of m_piText
	unsigned char m_sChars[256];
	bool m_bEditingMode;
	int m_iMaxNumDigits;
	int m_iNumChars;
	bool m_bEditing;
	int m_iCurrentEditingDigit;
};	


#endif
