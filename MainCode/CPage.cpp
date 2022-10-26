#include "CPage.h"
#include "COption.h"
#include "string.h"
#include "CPsPectrum.h"
#include "TEmuOptions.h"
#include "cfileselector.h"
#include "CGUI.h"
#include "CMGESprite.h"
#include "snapshot.h"
#include <string.h>
#include "fuse/machine.h"
#include "CMsgBox.h"

CMGESprite* CPage::m_pBar=0;
CMGESprite* CPage::m_pMoreUpIcon=0;
CMGESprite* CPage::m_pMoreDownIcon=0;

bool CPage::Init(const char* sTitle)
{
	m_iNumOptions=0; m_pCurrentOption=0; m_iXPos=0; m_iYPos=0; m_sTitle[0]='\0';
	m_pFirstOption=m_pLastOption=0;m_pNext=m_pPrev=0;

	if(!m_pBar)
	{	
		m_pBar=new CMGESprite;
		m_bOK=m_pBar->Init("media/Bar_480x14.tga",0,0,0,0,0);
		if(m_bOK)
		{
			m_pMoreUpIcon=new CMGESprite;
			m_bOK=m_pMoreUpIcon->Init("media/MoreUpIcon_20x14.tga",0,0,0,0,0);
			if(m_bOK)
			{
				m_pMoreDownIcon=new CMGESprite;
				m_bOK=m_pMoreDownIcon->Init("media/MoreDownIcon_20x14.tga",0,0,0,0,0);
			}
		}
	}
	m_fAccumTime=0;
	m_pFirstOptionOnScreen=0;	
	m_iCurrentOptionPos=0;
	strcpy(m_sTitle,sTitle);
	
	m_bKeybFocus=true; //if true page receives all control events if not selected control
	
	return m_bOK;
}

void CPage::End()
{
	if(m_bOK)
	{
		COption* pOp=m_pFirstOption;
		COption* pOpTemp=pOp->GetNext();
		while(pOp)
		{
			SafeDelete(pOp);
			pOp=pOpTemp;
			if(pOp)
				pOpTemp=pOp->GetNext();
		}
	
	
		SafeDelete(m_pBar);
		SafeDelete(m_pMoreUpIcon);
		SafeDelete(m_pMoreDownIcon);
	}

}

void CPage::AddOption(COption* pOption)
{
	//No options
	if(m_pLastOption==0)
	{	
		m_pFirstOption=m_pLastOption=pOption;
		m_pCurrentOption=pOption;
		pOption->SetPosInPage(0);
		m_pFirstOptionOnScreen=m_pFirstOption;
	}
	else
	{	
		m_pLastOption->SetNext(pOption);
		pOption->SetPrev(m_pLastOption);
		m_pLastOption=pOption;
		pOption->SetPosInPage(m_iNumOptions);
	}

	pOption->SetNext(0);
	++m_iNumOptions;
}

void CPage::NextOption()
{
	if(m_pCurrentOption->GetNext())
	{	
		m_pCurrentOption=m_pCurrentOption->GetNext();
		while(m_pCurrentOption->GetType()==COption::OT_LABEL && m_pCurrentOption->GetNext())
		{
			m_pCurrentOption=m_pCurrentOption->GetNext();
		}
	}

}

void CPage::PreviousOption()
{
	if(m_pCurrentOption->GetPrev())
	{	
		m_pCurrentOption=m_pCurrentOption->GetPrev();
		while(m_pCurrentOption->GetType()==COption::OT_LABEL && m_pCurrentOption->GetPrev())
		{
			m_pCurrentOption=m_pCurrentOption->GetPrev();
		}
	}
}

COption* CPage::GetOption(const char*sName)
{
	bool bFound=false;
	COption* pOption=m_pFirstOption;
	while(!bFound && pOption)
	{
		if(!strcmp(pOption->GetOptionName(),sName))
			bFound=true;
		else 
			pOption=pOption->GetNext();
	}

	return bFound?pOption: 0;
}

int CPage::GetOptionIndex(const char*sName)
{
	bool bFound=false;
	int iIndex=0;
	COption* pOption=m_pFirstOption;
	while(!bFound && pOption)
	{
		if(!strcmp(pOption->GetOptionName(),sName))
			bFound=true;
		else 
		{
			pOption=pOption->GetNext();
			++iIndex;
		}
	}

	return bFound?iIndex: -1;
}


void CPage::SetTitle(const char* sTitle)
{
	strcpy(m_sTitle,sTitle);
}

//Basic Page Run
void CPage::Run( float fTime)
{
	CPsPectrum* pApp=CPsPectrum::GetApp();
	CMGEControl* pControl=pApp->GetControl();

	m_fAccumTime+=fTime;

	bool bGotEvent=m_pCurrentOption->Run(fTime);
	
	//Si esta tecla se procesa en un cotnrol no tenerla en cuetna aquí
	if(!bGotEvent)
	{
		//Fast move
		if(pControl->m_cuResponse.buttons[CPsPectrum::BT_TRIANGLE].bePushed)
			m_fAccumTime=kDELAY;

		if(pControl->m_cuResponse.up && pApp->LetMove(CPsPectrum::BT_UP,pControl,m_fAccumTime,kDELAY))
		{		
			this->PreviousOption();

			//If we pressed up, and we got to a previous option that is before the current
			//on screen, the current on screen is the currentoptio. Check for labels though
			if(m_pFirstOptionOnScreen->GetPosInPage()>m_pCurrentOption->GetPosInPage())
			{		
				m_pFirstOptionOnScreen=m_pCurrentOption;
				while(m_pFirstOptionOnScreen->GetType()==COption::OT_LABEL && m_pFirstOptionOnScreen->GetPrev())
					m_pFirstOptionOnScreen=m_pFirstOptionOnScreen->GetPrev();
			}
			m_fAccumTime=0;
		}
		else if(pControl->m_cuResponse.down && pApp->LetMove(CPsPectrum::BT_DOWN,pControl,m_fAccumTime,kDELAY))
		{		
			this->NextOption();
			if(m_pCurrentOption->GetPosInPage()-m_pFirstOptionOnScreen->GetPosInPage()>=MAX_OPTIONS_SCREEN)
				m_pFirstOptionOnScreen=m_pFirstOptionOnScreen->GetNext();
			m_fAccumTime=0;
		}
	
		DoActions();
	}
}

void CPage::Render(CMGEScreen* pScr,int iParentX, int iParentY)
{
	//Print Page title centered
	CPsPectrum* pApp=CPsPectrum::GetApp();

	int iCenteredXPos=480/2 - (int)(strlen(m_sTitle)*8/2);
	pApp->PrintText(pScr,m_sTitle,iCenteredXPos+iParentX/2+m_iXPos/2,16+m_iYPos+iParentY);

	int iCursorYRow=m_pCurrentOption->GetPosInPage()-m_pFirstOptionOnScreen->GetPosInPage();
	//Print Now Option Highlighter
	m_pBar->Draw(pScr,0,OPTIONS_Y_OFFSET+m_iYPos+iParentY+iCursorYRow*16);
	
	//Draw arrows
	if(m_pFirstOptionOnScreen!=m_pFirstOption)
		m_pMoreUpIcon->Draw(pScr,450,OPTIONS_Y_OFFSET);

	if(m_pFirstOptionOnScreen->GetPosInPage()+MAX_OPTIONS_SCREEN<m_iNumOptions)
		m_pMoreDownIcon->Draw(pScr,450,OPTIONS_Y_OFFSET+16*(MAX_OPTIONS_SCREEN-1));
	
	//Print options
	COption* pOp=m_pFirstOptionOnScreen;
	int i=0;
	while(pOp && i<MAX_OPTIONS_SCREEN)
	{    
		pOp->Render(pScr,m_iXPos+iParentX+SELECTABLE_OFFSET_X,OPTIONS_Y_OFFSET+m_iYPos+iParentY+i*16);
		pOp=pOp->GetNext();
		++i;
	}

	
}

int CPage::GetOptionValueIndex(const char* sOptionName)
{
	int iValue=-1;
	bool bFound=false;
	COption* pOp=m_pFirstOption;

	while(pOp && !bFound)
	{
		if(!strcmp(pOp->GetOptionName(),sOptionName))
		{
			assert(pOp->GetType()==COption::OT_SELECT);
			iValue = ((CSelectOption*)pOp)->GetActiveValueIndex();
			bFound=true;
		}

		pOp=pOp->GetNext();
	}

	return iValue ;
}


const char*  CPage::GetOptionValueString(const char* sOptionName)
{
	const char* pValue=0;
	bool bFound=false;
	COption* pOp=m_pFirstOption;

	while(pOp && !bFound)
	{
		if(!strcmp(pOp->GetOptionName(),sOptionName))
		{
			assert(pOp->GetType()==COption::OT_SELECT);
			pValue = ((CSelectOption*)pOp)->GetActiveValueString();

			bFound=true;
		}

		pOp=pOp->GetNext();
	}

	return pValue;
}

void CPage::SetOptionValue(const char* sOptionName, int iIndex)
{
	int iValue=-1;
	bool bFound=false;
	COption* pOp=m_pFirstOption;

	while(pOp && !bFound)
	{
		if(!strcmp(pOp->GetOptionName(),sOptionName))
		{
			assert(pOp->GetType()==COption::OT_SELECT);
			((CSelectOption*)pOp)->SetActiveValue(iIndex);
			bFound=true;
		}

		pOp=pOp->GetNext();
	}
}


const char*  CPage::GetEditBoxText(const char* sOptionName)
{
	const char* pValue=0;
	bool bFound=false;
	COption* pOp=m_pFirstOption;

	while(pOp && !bFound)
	{
		if(!strcmp(pOp->GetOptionName(),sOptionName))
		{
			assert(pOp->GetType()==COption::OT_EDITBOX);
			pValue = ((CEditOption*)pOp)->GetText();

			bFound=true;
		}

		pOp=pOp->GetNext();
	}

	return pValue;
}

void CPage::SetTextBoxText(const char* sOptionName, const char* sText)
{
	int iValue=-1;
	bool bFound=false;
	COption* pOp=m_pFirstOption;

	while(pOp && !bFound)
	{
		if(!strcmp(pOp->GetOptionName(),sOptionName))
		{
			assert(pOp->GetType()==COption::OT_EDITBOX);
			((CEditOption*)pOp)->SetText(sText);
			bFound=true;
		}

		pOp=pOp->GetNext();
	}
}



/*****************/

void CGamePage::Init()
{
	CPage::Init("GAME OPTIONS");
}

void CGamePage::DoActions()
{
	CMGEControl* pControl=CPsPectrum::GetApp()->GetControl();
	CPsPectrum* pApp=CPsPectrum::GetApp();

	//Check if any of my buttons where pressed
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_CROSS].wasPressed)
	{		
		//We must fill emuoptions from gui
		pApp->GetGUI()->FillEmuOptions();

		if(m_pCurrentOption->GetPosInPage()==0)	
		{		
			//Get Only the name
			char Buffer[256];
			const char* sFile=pApp->GetFileSelector()->GetLastLoadedFile();
			if(!sFile)
				strcpy(Buffer,"cfgs/Default.cfg");
			else
			{
				strcpy(Buffer,"cfgs/");
				strcat(Buffer,sFile);
				char* p=strchr(Buffer,'.');
				*p='\0';
				strcat(Buffer,".cfg");
			}

			bool bSuccess=pApp->GetEmuOptions()->SaveOptions(Buffer);
			if(bSuccess)
				pApp->GetMsgBox()->ShowMsg("Configuration was saved OK");
			else
				pApp->GetMsgBox()->ShowMsg("Found a ploblem saving configuration");
		}
		else if(m_pCurrentOption->GetPosInPage()==1)
		{
			char sBuffer[256];
			char sBuffer2[256];
			
			
			CPsPectrum::GetFileFromPathWithoutExtension(pApp->GetFileSelector()->GetLastLoadedFile(),sBuffer2);
			
			strcpy(sBuffer,"savestates/");
			strcat(sBuffer,sBuffer2);
			strcat(sBuffer,".Z80");

			if(!snapshot_write(sBuffer))
				pApp->GetMsgBox()->ShowMsg("Snapshot was saved OK");
			else
				pApp->GetMsgBox()->ShowMsg("Snapshot failed to save");
		}
		else if(m_pCurrentOption->GetPosInPage()==2)
		{
			char sBuffer[256];
			char sBuffer2[256];

			CPsPectrum::GetFileFromPathWithoutExtension(pApp->GetFileSelector()->GetLastLoadedFile(),sBuffer2);
			
			strcpy(sBuffer,"savestates/");
			strcat(sBuffer,sBuffer2);
			strcat(sBuffer,".Z80");

			if(!snapshot_read(sBuffer))
				pApp->GetMsgBox()->ShowMsg("Snapshot was loaded OK");
			else
				pApp->GetMsgBox()->ShowMsg("Snapshot failed to load");
		}
	}
}

/*****************/

void CMachinePage::Init()
{
	CPage::Init("MACHINE OPTIONS");
}

void CMachinePage::DoActions()
{
	CMGEControl* pControl=CPsPectrum::GetApp()->GetControl();
	CPsPectrum* pApp=CPsPectrum::GetApp();

	
	//Check if any of my buttons where pressed
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_CROSS].wasPressed)
	{		
		//We must fill emuoptions from gui
		pApp->GetGUI()->FillEmuOptions();

		if(!stricmp(m_pCurrentOption->GetOptionName(),"reset computer"))
		{		
			bool bVal=pApp->GetMsgBox()->ShowMsg("Really reset computer?",CMsgBox::MBOX_YES_NO);
			if(bVal)
			{			
				pApp->GetMsgBox()->ShowMsg("Computer Has been Reseted");
				machine_reset();
			}
		}

	}
}

/*****************/

void CGeneralPage::Init()
{
	CPage::Init("GENERAL OPTIONS");
}

void CGeneralPage::DoActions()
{
	CMGEControl* pControl=CPsPectrum::GetApp()->GetControl();
	CPsPectrum* pApp=CPsPectrum::GetApp();

	//Check if any of my buttons where pressed
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_CROSS].wasPressed)
	{		
		//We must fill emuoptions from gui
		pApp->GetGUI()->FillEmuOptions();

		if(!stricmp(m_pCurrentOption->GetOptionName(),"Exit Emulation"))
		{		
			#ifdef _PSP
				sceKernelExitGame();
			#else
				exit(0);
			#endif
		}

	}
}


/*******************************************/
void CPokerPage::Init()
{
	CPage::Init("POKER");
}

void CPokerPage::DoActions()
{
	CMGEControl* pControl=CPsPectrum::GetApp()->GetControl();
	CPsPectrum* pApp=CPsPectrum::GetApp();

	//Check if any of my buttons where pressed
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_CROSS].wasPressed)
	{		

	}
}
