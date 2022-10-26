#include "string.h"
#include "CGUI.h"
#include "CPage.h"
#include "CMGELog.h"
#include "CPsPectrum.h"
#include "EmulatorDefines.h"
#include "CMGETgaFile.h"
#include "COption.h"
#include "CKeyboard.h"

void	CGUI::Init(TEmuOptions* pEmuOptions) 
{
	m_pCurrentPage=0; 
	m_pFirstPage=0; 
	m_pLastPage=0;
	m_iNumPages=0; 
	m_iXPos=0; 
	m_iYPos=0;
	m_fAccumTime=0;
	m_pEmuOptions=pEmuOptions;
	BuildMenu();
	SetDefaultOptions();
	m_pBG=new CMGETgaFile;
	m_pBG->Init("media/OptionsBG.tga");
}	

CGUI::~CGUI()
{
	SafeDelete(m_pBG);
	CPage* pPage=m_pFirstPage;
	CPage* pPageTemp=pPage->GetNext();
	while(pPage)
	{
		delete pPage;
		pPage=pPageTemp;
		if(pPage)
			pPageTemp=pPage->GetNext();
	}
}


void CGUI::AddPage(CPage* pPage)
{
	//No options
	if(m_pLastPage==0)
	{	
		m_pFirstPage=m_pLastPage=pPage;
		m_pCurrentPage=pPage;
	}
	else
	{	
		m_pLastPage->SetNext(pPage);
		pPage->SetPrev(m_pLastPage);
		m_pLastPage=pPage;
	}

	pPage->SetNext(0);
	++m_iNumPages;
}

       
void CGUI::NextPage()
{
	m_pCurrentPage=m_pCurrentPage->GetNext();
	if(m_pCurrentPage==0)
		m_pCurrentPage=m_pFirstPage;
}

void CGUI::PreviousPage()
{
	m_pCurrentPage=m_pCurrentPage->GetPrev();
	if(m_pCurrentPage==0)
		m_pCurrentPage=m_pLastPage;	
}
       
void CGUI::Render(CMGEScreen *pScr)
{
    m_pBG->ScreenCopy(pScr,0,0);
	m_pCurrentPage->Render(pScr,m_iXPos,m_iYPos);
}

bool CGUI::Run(float fTime)
{

	CMGEControl* pControl=CPsPectrum::GetApp()->GetControl();
	
	m_fAccumTime+=fTime;
	
	m_pCurrentPage->Run(fTime);
	
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_LTRIGGER].wasPressed)
		NextPage();
	else if(pControl->m_cuResponse.buttons[CPsPectrum::BT_START].wasPressed)
	{	
		FillEmuOptions();
		return true;
	}

	//Did we exit?
	return false;
}

COption* CGUI::GetOption(const char* sOptionName)
{
    bool bFound=false;
    CPage* pPage=m_pFirstPage;
	COption* pOption;
	while(pPage && !bFound)
	{
		pOption=pPage->GetOption(sOptionName);
		if(pOption)
			bFound=true;
		pPage=pPage->GetNext();
	}
	
	
    return bFound ? pOption : 0;
}

//Returns -1 if doesn´t exist a option with sOptionName
int CGUI::GetOptionIndex(const char* sOptionName)
{
    int iValue=-1;
    bool bFound=false;
    CPage* pPage=m_pFirstPage;
	
	while(pPage && !bFound)
	{
		iValue=pPage->GetOptionIndex(sOptionName);
		if(iValue>-1)
			bFound=true;
		pPage=pPage->GetNext();
	}
	
    return bFound ? iValue : -1;
}

//Returns -1 if doesn´t exist a option with sOptionName
int CGUI::GetOptionValueIndex(const char* sOptionName)
{
    int iValue=-1;
    bool bFound=false;
    CPage* pPage=m_pFirstPage;
	
	while(pPage && !bFound)
	{
		iValue=pPage->GetOptionValueIndex(sOptionName);
		if(iValue>-1)
			bFound=true;
		pPage=pPage->GetNext();
	}
	
	
    return bFound ? iValue : -1;
}

const char*  CGUI::GetOptionValueString(const char* sOptionName)
{
	const char* pValue=0;
	bool bFound=false;
	CPage* pPage=m_pFirstPage;
	while(pPage && !bFound)
	{
		pValue=pPage->GetOptionValueString(sOptionName);
		if(pValue)
			bFound=true;
		pPage=pPage->GetNext();
	}


	return bFound ? pValue : 0;
}

void CGUI::SetOptionValue(const char* sOptionName, int iIndex)
{
	int iValue=0;
	bool bFound=false;
	CPage* pPage=m_pFirstPage;
	while(pPage && !bFound)
	{
		iValue=pPage->GetOptionValueIndex(sOptionName);
		if(iValue!=-1)
		{		
			bFound=true;
			pPage->SetOptionValue(sOptionName,iIndex);
		}
		pPage=pPage->GetNext();
	}
}

void CGUI::SetTextValue(const char* sOptionName, const char* sTextValue)
{
	int iValue=0;
	bool bFound=false;
	CPage* pPage=m_pFirstPage;
	while(pPage && !bFound)
	{
		COption* pOption=pPage->GetOption(sOptionName);
		if(pOption)
		{		
			bFound=true;
			((CEditOption*)pOption)->SetText(sTextValue);
		}
		else
			pPage=pPage->GetNext();
	}
}

const char* CGUI::GetTextValue(const char* sOptionName)
{
	int iValue=0;
	bool bFound=false;
	const char* pText=0;
	CPage* pPage=m_pFirstPage;
	while(pPage && !bFound)
	{
		COption* pOption=pPage->GetOption(sOptionName);
		if(pOption)
		{		
			bFound=true;
			pText=((CEditOption*)pOption)->GetText();
		}
		else
			pPage=pPage->GetNext();
	}
	return pText;
}


//Fills Emu options from GUI data
void CGUI::FillEmuOptions()
{
	const char* pValue;
	char Buffer[256];
	m_pEmuOptions->eJoyType=(TEmuOptions::EJoyTypeOption)GetOptionValueIndex("Fast Config");
	m_pEmuOptions->eSpectrumType=(EMachineType)GetOptionValueIndex("Spectrum Type");
	m_pEmuOptions->eScreenSize=(TEmuOptions::EScreenSize)GetOptionValueIndex("Screen Size");
	m_pEmuOptions->eSound=(TEmuOptions::EBoolOption)GetOptionValueIndex("Sound");
	m_pEmuOptions->ePSPSpeed=(TEmuOptions::ESpeedOption)GetOptionValueIndex("Speed");
	m_pEmuOptions->eAutoload=(TEmuOptions::EBoolOption)GetOptionValueIndex("Autoload Tape");
	
	//Keyb Transp option set
		pValue=GetOptionValueString("Keyb Transparency");
		//Copiamos y le quitamos el %
		strcpy(Buffer,pValue);
		Buffer[strlen(Buffer)-1]='\0';
		m_pEmuOptions->fKeybTrans=(float)(atof(Buffer)/100.0f);
	//Cursor Size option set
		pValue=GetOptionValueString("Pointer Size");
		strcpy(Buffer,pValue);
		m_pEmuOptions->iCursorSize=atoi(Buffer);

	//Keys -> We only need first char of returned string
	//It is needed to put the correct EMachineType before calling setkeyoptions
	//because uses the type to know how many keys must check in the static array at CKeyboard
	SetKeyOptions();

	//Poker options
	strcpy(m_pEmuOptions->Pokes[0].sPoke,GetTextValue("Poke 1:"));
	strcpy(m_pEmuOptions->Pokes[1].sPoke,GetTextValue("Poke 2:"));
	strcpy(m_pEmuOptions->Pokes[2].sPoke,GetTextValue("Poke 3:"));
	strcpy(m_pEmuOptions->Pokes[3].sPoke,GetTextValue("Poke 4:"));
	m_pEmuOptions->Pokes[0].ePokeActive=(TEmuOptions::EBoolOption)GetOptionValueIndex("Poke 1 Active:");
	m_pEmuOptions->Pokes[1].ePokeActive=(TEmuOptions::EBoolOption)GetOptionValueIndex("Poke 2 Active:");
	m_pEmuOptions->Pokes[2].ePokeActive=(TEmuOptions::EBoolOption)GetOptionValueIndex("Poke 3 Active:");
	m_pEmuOptions->Pokes[3].ePokeActive=(TEmuOptions::EBoolOption)GetOptionValueIndex("Poke 4 Active:");

	for(int i=0;i<4;++i)
	{
		m_pEmuOptions->ConvertPoke((const char*)m_pEmuOptions->Pokes[i].sPoke,m_pEmuOptions->Pokes[i].Dir,m_pEmuOptions->Pokes[i].Data);
	}
}


void CGUI::SetKeyOptions()
{
	for(int i=0;i<TEmuOptions::UK_NUM;++i) 
	{
		//We only need first char
		const char *sStr=GetOptionValueString(TEmuOptions::psButtonOptions[i]);
		m_pEmuOptions->Controls[i]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,sStr);
	}

	
	TEmuOptions::EJoyTypeOption  eJoyType=(TEmuOptions::EJoyTypeOption)GetOptionValueIndex("Fast Config");
	if(eJoyType==TEmuOptions::JTO_KEMPSTON)
		m_pEmuOptions->eJoyType=TEmuOptions::JTO_KEMPSTON;
	else if(eJoyType==TEmuOptions::JTO_SINCLAIR1)
	{
		m_pEmuOptions->Controls[TEmuOptions::UK_UP]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR1_UP);
		m_pEmuOptions->Controls[TEmuOptions::UK_DOWN]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR1_DOWN);
		m_pEmuOptions->Controls[TEmuOptions::UK_LEFT]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR1_LEFT);
		m_pEmuOptions->Controls[TEmuOptions::UK_RIGHT]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR1_RIGHT);
		m_pEmuOptions->Controls[TEmuOptions::UK_SQUARE]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR1_FIRE);
		m_pEmuOptions->eJoyType=TEmuOptions::JTO_SINCLAIR1;
	}
	else if(eJoyType==TEmuOptions::JTO_SINCLAIR2)
	{	
		m_pEmuOptions->Controls[TEmuOptions::UK_UP]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR2_UP);
		m_pEmuOptions->Controls[TEmuOptions::UK_DOWN]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR2_DOWN);
		m_pEmuOptions->Controls[TEmuOptions::UK_LEFT]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR2_LEFT);
		m_pEmuOptions->Controls[TEmuOptions::UK_RIGHT]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR2_RIGHT);
		m_pEmuOptions->Controls[TEmuOptions::UK_SQUARE]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,SINCLAIR2_FIRE);
		m_pEmuOptions->eJoyType=TEmuOptions::JTO_SINCLAIR1;
	}
	else if(eJoyType==TEmuOptions::JTO_QAOP)
	{
		m_pEmuOptions->Controls[TEmuOptions::UK_UP]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,"Q");
		m_pEmuOptions->Controls[TEmuOptions::UK_DOWN]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,"A");
		m_pEmuOptions->Controls[TEmuOptions::UK_LEFT]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,"O");
		m_pEmuOptions->Controls[TEmuOptions::UK_RIGHT]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,"P");
		m_pEmuOptions->Controls[TEmuOptions::UK_SQUARE]=*CKeyboard::GetKeyDefFromName(m_pEmuOptions->eSpectrumType,"SPACE");
	
		m_pEmuOptions->eJoyType=TEmuOptions::JTO_QAOP;
	}
}


void CGUI::SetDefaultOptions()
{
	m_pEmuOptions->SetDefaults();
	
	SetOptionValue("Fast Config", m_pEmuOptions->eJoyType);
	
	//Set All keys ->TODO:Esto debería de inicializarse en m_pEmuOptions primero
	//y despues setearlo usando m_pEmuoptions pero hay que tocar coasa y paso ahoara
	SetOptionValue("Up",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("Down",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("Left",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("Right",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("Square",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("Triangle",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("Cross",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("Circle",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("L+Square",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("L+Cross",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("L+Triangle",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("L+Circle",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("L+Up",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("L+Down",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("L+Left",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	SetOptionValue("L+Right",CKeyboard::GetIndexFromName(m_pEmuOptions->eSpectrumType,m_pEmuOptions->Controls[TEmuOptions::UK_UP].m_Name));
	
	SetOptionValue("Keyb Transparency",m_pEmuOptions->fKeybTrans*10);
	SetOptionValue("Pointer Size",m_pEmuOptions->iCursorSize);
	SetOptionValue("Sound",m_pEmuOptions->eSound);
	SetOptionValue("Spectrum Type",m_pEmuOptions->eSpectrumType);
	SetOptionValue("Speed",m_pEmuOptions->ePSPSpeed);
	SetOptionValue("Autoload Tape",m_pEmuOptions->eAutoload);
	SetTextValue("Poke 1:","65535,255");
	SetTextValue("Poke 2:","65535,255");
	SetTextValue("Poke 3:","65535,255");
	SetTextValue("Poke 4:","65535,255");
	SetOptionValue("Poke 1 Active:",m_pEmuOptions->Pokes[0].ePokeActive);
	SetOptionValue("Poke 2 Active:",m_pEmuOptions->Pokes[1].ePokeActive);
	SetOptionValue("Poke 3 Active:",m_pEmuOptions->Pokes[2].ePokeActive);
	SetOptionValue("Poke 4 Active:",m_pEmuOptions->Pokes[3].ePokeActive);
}


void CGUI::BuildMenu()
{
	//MACHINE PAGE

	CMachinePage*  pMachinePage=new CMachinePage;
	pMachinePage->Init();

	CLabelOption* pLabelOp=0;
	CSelectOption* pOption=new CSelectOption;
	pOption->Init("Spectrum Type",TEmuOptions::psMachineTypeOptions,MT_NUM);
	pMachinePage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Autoload Tape",TEmuOptions::psYesNoOptions,TEmuOptions::BO_NUM);
	pMachinePage->AddOption(pOption);
	CButtonOption* pButtonOp=new CButtonOption;
	pButtonOp->Init("Reset Computer");
	pMachinePage->AddOption(pButtonOp);

	AddPage(pMachinePage);

	//GENERAL OPTIONS PAGE
	CPage* pPage=new CGeneralPage;
	((CGeneralPage*)pPage)->Init();

	static const char* psSpeeds[]={"233","266","333"};
	pOption=new CSelectOption;
	pOption->Init("Speed",TEmuOptions::psSpeedOptions,TEmuOptions::SO_NUM);
	pPage->AddOption(pOption);

	pOption=new CSelectOption;
	pOption->Init("Vsync",TEmuOptions::psYesNoOptions,TEmuOptions::BO_NUM);
	pPage->AddOption(pOption);

	pOption=new CSelectOption;
	pOption->Init("Sound",TEmuOptions::psYesNoOptions,TEmuOptions::BO_NUM);
	pPage->AddOption(pOption);
	AddPage(pPage);

	pOption=new CSelectOption;
	pOption->Init("Keyb Transparency",TEmuOptions::psKeybTrans,TEmuOptions::KT_NUM);
	pPage->AddOption(pOption);

	pOption=new CSelectOption;
	pOption->Init("Pointer Size",TEmuOptions::psCursorSizeOptions,TEmuOptions::CS_NUM);
	pPage->AddOption(pOption);

	pOption=new CSelectOption;
	pOption->Init("Screen Size",TEmuOptions::psScreenSize,TEmuOptions::SS_NUM);
	pPage->AddOption(pOption);
	
	CButtonOption* pButton=new CButtonOption;
	pButton->Init("Exit Emulation");
	pPage->AddOption(pButton);
	/*const char* sKeys128[56]={"TRUE VIDEO","INV VIDEO","1","2","3","4","5","6","7","8","9","0","BREAK",
	"DELETE","GRAPH","Q","W","E","R","T","Y","U","I","O","P",
	"EXTENDED MODE","EDIT","A","S","D","F","G","H","J","K","L","ENTER",
	"CAPS SHIFT","CAPS LOCK","Z","X","C","V","B","N","M",".",
	"SIMBOL SHIFT",";","\"","CURSOR LEFT","CURSOR RIGHT","SPACE","CURSOR UP","CURSOR DOWN",","};*/

	pPage=new CPage;
	pPage->Init("CUSTOM CONTROLS");

	static const char* sJoyTypes[]={"Kempston","Sinclair 1","Sinclair 2","QAOPSPC","Custom"};

	pOption=new CSelectOption;
	pOption->Init("Fast Config",sJoyTypes,5);
	pPage->AddOption(pOption);

	pLabelOp=new CLabelOption;
	pLabelOp->Init(0);
	pPage->AddOption(pLabelOp);
	pOption=new CSelectOption;
	pOption->Init("Up",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Down",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Left",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Right",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Square",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Triangle",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Cross",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Circle",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("L+Square",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("L+Cross",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("L+Triangle",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("L+Circle",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("L+Up",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("L+Down",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("L+Left",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("L+Right",TEmuOptions::psKeys48Options,40);
	pPage->AddOption(pOption);
	AddPage(pPage);

	CGamePage* pGamePage=new CGamePage;
	pGamePage->Init();
	pButtonOp=new CButtonOption;
	pButtonOp->Init("Save Game Config");
	pGamePage->AddOption(pButtonOp);
	pButtonOp=new CButtonOption;
	pButtonOp->Init("Save SaveState");
	pGamePage->AddOption(pButtonOp);
	pButtonOp=new CButtonOption;
	pButtonOp->Init("Load SaveState");
	pGamePage->AddOption(pButtonOp);
	AddPage(pGamePage);

	CPokerPage* pPokerPage=new CPokerPage;
	pPokerPage->Init();
	CEditOption* pEditOp=new CEditOption;
	pEditOp->Init("Poke 1:",(unsigned char*)"0123456789, ",11,9);
	pPokerPage->AddOption(pEditOp);
	pEditOp=new CEditOption;
	pEditOp->Init("Poke 2:",(unsigned char*)"0123456789, ",11,9);
	pPokerPage->AddOption(pEditOp);
	pEditOp=new CEditOption;
	pEditOp->Init("Poke 3:",(unsigned char*)"0123456789, ",11,9);
	pPokerPage->AddOption(pEditOp);
	pEditOp=new CEditOption;
	pEditOp->Init("Poke 4:",(unsigned char*)"0123456789, ",11,9);
	pPokerPage->AddOption(pEditOp);
	
	pLabelOp=new CLabelOption;
	pLabelOp->Init(0);
	pPokerPage->AddOption(pLabelOp);
	pOption=new CSelectOption;
	pOption->Init("Poke 1 Active:",TEmuOptions::psYesNoOptions,TEmuOptions::BO_NUM);
	pPokerPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Poke 2 Active:",TEmuOptions::psYesNoOptions,TEmuOptions::BO_NUM);
	pPokerPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Poke 3 Active:",TEmuOptions::psYesNoOptions,TEmuOptions::BO_NUM);
	pPokerPage->AddOption(pOption);
	pOption=new CSelectOption;
	pOption->Init("Poke 4 Active:",TEmuOptions::psYesNoOptions,TEmuOptions::BO_NUM);
	pPokerPage->AddOption(pOption);


	AddPage(pPokerPage);
}
