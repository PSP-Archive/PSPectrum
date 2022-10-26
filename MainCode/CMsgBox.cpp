#include "CMsgBox.h"
#include "CPsPectrum.h"
#include "CMGERenderDevice.h"
#include "CMGEControl.h"

bool CMsgBox::ShowMsg(const char* sLine, EMsgBoxType eMsgBox)
{
	Reset();
	AddLine(sLine);
	return Run(eMsgBox);
}

void CMsgBox::AddLine(const char* sLine)
{
	strcpy(m_Lines[m_iNumLines],sLine);
	int iWidth=(int)strlen(sLine)*8;
	if(m_iMaxWidth<iWidth)
		m_iMaxWidth=iWidth;
	++m_iNumLines;
}

void CMsgBox::Reset()
{
	m_iNumLines=0;
	m_iMaxWidth=0;
}

bool CMsgBox::Run(EMsgBoxType eType)
{
	bool bExit=false;
	bool bRetVal=false;
	CPsPectrum* pApp=CPsPectrum::GetApp();
	CMGERenderDevice* pRenderer=pApp->GetRenderDevice();

	CPsPectrum::EAppState eOldState=pApp->GetAppState();
	pApp->SetAppState(CPsPectrum::AS_DIALOG);

	CMGEControl::ControlResponse* pControl=&pApp->GetControl()->m_cuResponse;

	if(eType==MBOX_NORMAL)
	{	
		AddLine("");
		AddLine("");
		AddLine("X Exit");
	}
	else
	{
		AddLine("");
		AddLine("");
		AddLine("X Yes     O No");
	}

	int iWidth=m_iMaxWidth;
	int iHeight=m_iNumLines*13;
	//Make dialog a little more big
	iWidth=iWidth+32;
	iHeight=iHeight+52;
	
	int iXPos=240-iWidth/2;
	int iYPos=136-iHeight/2;

	#ifdef _PSP   //Get Synched with display first (if not we leave one gu list not rendered)
		pRenderer->EndFrame();
		pRenderer->Flip();
	#endif

	while(!bExit)
	{
		pApp->DoJobs();
		#ifdef _PSP
			sceKernelDelayThread(20000);
		#endif
		
		//Render
		pRenderer->GetWorkingScreen()->Box(iXPos,iYPos,iXPos+iWidth,iHeight+iYPos,pApp->GetRenderDevice()->GetColor(100,0xFF,0));
		
		for(int i=0; i<m_iNumLines;++i)
		{
			int iLineWidth=(int)(strlen(m_Lines[i])*8);
			int	iTextX=240-(iLineWidth>>1);
			int	iTextY=i*13 + iYPos + 16;

			pApp->PrintText(pRenderer->GetWorkingScreen(),m_Lines[i],iTextX,iTextY);
		}
		
		
		if(pControl->buttons[CPsPectrum::BT_CROSS].wasPressed)
		{			
			bRetVal=true;
			bExit=true;
		}
		else if(eType==MBOX_YES_NO && pControl->buttons[CPsPectrum::BT_CIRCLE].wasPressed)
		{
			bExit=true;
			bRetVal=false;
		}
		
	}

	//Clean buttons
	CPsPectrum::GetApp()->GetControl()->m_cuResponse.buttons[CPsPectrum::BT_CROSS].wasPressed=false;
	CPsPectrum::GetApp()->GetControl()->m_cuResponse.buttons[CPsPectrum::BT_CIRCLE].wasPressed=false;
	
	pApp->SetAppState(eOldState);
	
	return bRetVal;
}
