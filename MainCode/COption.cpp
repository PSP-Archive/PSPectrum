#include "COption.h"
#include "CMGEScreen.h"
#include "CPsPectrum.h"
#include "CMsgBox.h"

bool COption::Init(const char* sOptionName)
{
	End();
	
	if(sOptionName)
		strcpy(m_sOptionName,sOptionName);
	else
		m_sOptionName[0]='\0';

	m_fAccumTime=0;
	m_pNext=m_pPrev=0;

	m_eType=OT_OTHER;
	
	return true;
}


void COption::Render(CMGEScreen* pScr, int iXPos, int iYPos)
{
	CPsPectrum* pApp=CPsPectrum::GetApp();
	if(m_sOptionName)
	{	
		int iHalf=480>>1;
		int iPos=iHalf-((strlen(m_sOptionName)*8)>>1);
		pApp->PrintText(pScr,m_sOptionName,iPos,iYPos);
	}
}

void CSelectOption::Render(CMGEScreen* pScr,int iXPos, int iYPos)
{
	CPsPectrum* pApp=CPsPectrum::GetApp();
	pApp->PrintText(pScr,m_sOptionName,iXPos,iYPos);
    pApp->PrintText(pScr,m_psSelections[m_dwCurrentSelection],SELECTION_OFFSET+iXPos,iYPos);
}


void CSelectOption::Init(const char* sOptionName, const char** sSelections, u32 dwNumSelections) 
{ 
	End();

	COption::Init(sOptionName);

	m_eType=OT_SELECT;	
	m_psSelections=sSelections;
	m_dwNumSelections=dwNumSelections; 
	m_dwCurrentSelection=0; 
	m_fAccumTime=0;
}

void CSelectOption::NextSelection()
{
    ++m_dwCurrentSelection;
    if(m_dwCurrentSelection>=m_dwNumSelections)
        m_dwCurrentSelection=0;
}

void CSelectOption::PreviousSelection()
{
    --m_dwCurrentSelection;
    if(m_dwCurrentSelection<0)
        m_dwCurrentSelection=m_dwNumSelections-1;
}


//Puts value at iIndex iIndex in the value array as active
void CSelectOption::SetActiveValue(int iIndex)
{
    if(iIndex>-1 && iIndex<m_dwNumSelections)
        m_dwCurrentSelection=iIndex;
}


bool CSelectOption::Run(float fTime)
{
	CPsPectrum* pApp=CPsPectrum::GetApp();
	CMGEControl* pControl=pApp->GetControl();

	m_fAccumTime+=fTime;

	if(pControl->m_cuResponse.rigth && pApp->LetMove(CPsPectrum::BT_RIGHT,pControl,m_fAccumTime,kDELAY))
	{		
		NextSelection();
		m_fAccumTime=0;
		return true;
	}
	else if(pControl->m_cuResponse.left && pApp->LetMove(CPsPectrum::BT_LEFT,pControl,m_fAccumTime,kDELAY))
	{		
		PreviousSelection();
		m_fAccumTime=0;
		return true;
	}
	
	return false;
}

bool CLabelOption::Init(const char* sOptionName)
{
	COption::Init(sOptionName);
	m_eType=OT_LABEL;

	return m_bOk;
}

bool CButtonOption::Init(const char* sOptionName)
{
	COption::Init(sOptionName);
	m_eType=OT_BUTTON;

	return m_bOk;
}

//pChars indica que juego de caracteres son los que queremos mostrar
//FixedDigit indica si queremos tener un digito fijo con cierto caracter
bool CEditOption::Init(const char* sOptionName, const unsigned char* pChars, int iNumChars,int iMaxNumDigits)
{
	COption::Init(sOptionName);
	m_eType=OT_EDITBOX;
	m_iMaxNumDigits=iMaxNumDigits;
	strncpy((char*)m_sChars,(char*)pChars,iNumChars);
	m_iNumChars=iNumChars;
	m_bEditing=0;
	m_iCurrentEditingDigit=0;
	for(int i=0;i<m_iMaxNumDigits;++i)
		m_piText[i]=0;
	return true; 
}

const char* CEditOption::GetText()
{
	//Build string
	for(int i=0;i<m_iMaxNumDigits;++i)
		m_sText[i]=m_sChars[m_piText[i]];
	m_sText[m_iMaxNumDigits]='\0';
	return m_sText;
}

void CEditOption::SetText(const char* sText)
{
	strcpy(m_sText,sText);
	for(int i=0;i<m_iMaxNumDigits;++i)
	{
		int j=0;
		while(j<m_iNumChars)
		{
			if(m_sText[i]==m_sChars[j])
				m_piText[i]=j;
			++j;
		}
	}
}

bool CEditOption::ValidatePoke()
{
	char sBuffer[256];
	char sBuffer2[256];
	bool bValid=false;
	int Dir,  Data;
	//Build string
	for(int i=0;i<m_iMaxNumDigits;++i)
		sBuffer[i]=m_sChars[m_piText[i]];
	sBuffer[m_iMaxNumDigits]='\0';
	
	if(sBuffer[5]==',')
		bValid=true;

	
	char *pDir=strchr(sBuffer,',');
	
	if(pDir && bValid)
	{
		//check first part
		strncpy(sBuffer2,sBuffer,pDir-sBuffer);
		sBuffer2[pDir-sBuffer]='\0';
		if(Dir=atoi(sBuffer2))
		{
			//check second part
			strncpy(sBuffer2,pDir+1,3);
			sBuffer2[3]='\0';
			if(Data=atoi(sBuffer2) || !strcmp(sBuffer2,"000"))
			{			
				if(Dir>=0 && Dir<65536 && Data>=0 && Data<=255)
					bValid=true;
		
			}
		}

	}
		
	return bValid;
}

bool CEditOption::Run(float fTime)
{
	CPsPectrum* pApp=CPsPectrum::GetApp();
	CMGEControl* pControl=pApp->GetControl();

	m_fAccumTime+=fTime;
	bool bReturn=false;
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_CROSS].wasPressed)
	{
		if(m_bEditing)
		{
			bool bPokeOk=ValidatePoke();
			if(!bPokeOk)
				pApp->GetMsgBox()->ShowMsg("Poke format invalid try something like 65536,255");
			else
				m_bEditing=false;
		}
		else
			m_bEditing=true;

		bReturn=true;
	}

	if(m_bEditing)
	{
		DoEdit();
				
		//We want to get all messages
		return true;
	}


	return bReturn;
}

void CEditOption::Render(CMGEScreen* pScr,int iXPos, int iYPos)
{
	char Digit[2]; Digit[1]='\0';
	char sBuffer[256];

	CPsPectrum* pApp=CPsPectrum::GetApp();
	CMGERenderDevice* pRenderer=pApp->GetRenderDevice();

	pApp->PrintText(pScr,m_sOptionName,iXPos,iYPos);
	//Build String
	for(int i=0;i<m_iMaxNumDigits;++i)
		sBuffer[i]=m_sChars[m_piText[i]];
	sBuffer[m_iMaxNumDigits]='\0';
	Digit[0]=sBuffer[m_iCurrentEditingDigit];
	pApp->PrintText(pRenderer->GetWorkingScreen(),(char*)sBuffer,iXPos+SELECTION_OFFSET,iYPos);
	if(m_bEditing)
		pApp->PrintText(pRenderer->GetWorkingScreen(),Digit,iXPos+m_iCurrentEditingDigit*8+SELECTION_OFFSET,iYPos,0xFF,0,0);
}

bool CEditOption::DoEdit()
{
	CPsPectrum* pApp=CPsPectrum::GetApp();
	CMGEControl* pControl=pApp->GetControl();
	bool bReturn=false;
	
	if(pControl->m_cuResponse.left  && pApp->LetMove(CPsPectrum::BT_LEFT,pControl,m_fAccumTime,kDELAY))
	{
		if(m_iCurrentEditingDigit>0)
			--m_iCurrentEditingDigit;
	
		bReturn=true;
	}
	else if(pControl->m_cuResponse.rigth && pApp->LetMove(CPsPectrum::BT_RIGHT,pControl,m_fAccumTime,kDELAY))
	{
		if(m_iCurrentEditingDigit<m_iMaxNumDigits-1)
			++m_iCurrentEditingDigit;
		bReturn=true;
	}
	else if(pControl->m_cuResponse.up && pApp->LetMove(CPsPectrum::BT_UP,pControl,m_fAccumTime,kDELAY))
	{
		if(m_piText[m_iCurrentEditingDigit]==m_iNumChars-1)
			m_piText[m_iCurrentEditingDigit]=0;
		else
			++m_piText[m_iCurrentEditingDigit];
		bReturn=true;
	}
	else if(pControl->m_cuResponse.down && pApp->LetMove(CPsPectrum::BT_DOWN,pControl,m_fAccumTime,kDELAY))
	{
		if(m_piText[m_iCurrentEditingDigit]==0)
			m_piText[m_iCurrentEditingDigit]=m_iNumChars-1;
		else
			--m_piText[m_iCurrentEditingDigit];
		bReturn=true;
	}
	
	if(bReturn)
		m_fAccumTime=0;

	return bReturn;
}