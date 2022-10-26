//App includes
#include "CPsPectrum.h"

#include "..\Game\CFileSelector.h"
#include "..\Game\CKeyboard.h"
#include "..\Game\CKeyboard48k.h"
#include "..\Game\CKeyboard128k.h"
#include "..\Game\CGUI.h"
#include "..\Game\COption.h"
#include "..\Game\CPage.h"
#include "..\Game\TEmuOptions.h"
#include "..\Game\CMsgBox.h"

#include "CMGESpriteBank.h"
#include "CMGERenderDevice.h"
#include "CMGESoundGenerator.h"

#include "fuse/fuse.h"
#include "fuse/display.h"
#include "fuse/machine.h"
#include "fuse/snapshot.h"
#include "fuse/tape.h"
#include "fuse/utils.h"
#include "fuse/joystick.h"
#include "fuse/settings.h"
#include "fuse/memory.h"

#include "string.h"

//#include "..\Game\borrar.h"

#ifdef _PSP
	#include <psppower.h>
#endif

//Setting app desc
PSP_MODULE_INFO("PsPectrum", 0, 1, 1);


//Load data
bool CPsPectrum::Load()
{
	//loading result
	bool bResult = true;
	
	//Initialization needed to let everything create ok>
	m_pGUI=0;
	m_pFileSelector=0;
	m_pEmuScreen=0;
	m_pTexScreen=0;
	m_pFontBank=0;
	m_pEmuOptions=0;

	//Needed to make fuse render correctly first time (Must be here)
	#if defined(_PSP)
		m_pEmuScreen=new CMGEVramScreen();
		((CMGEVramScreen*)m_pEmuScreen)->Init(480,272,512,3);
		m_pTexScreen=new CMGEVramScreen();
		((CMGEVramScreen*)m_pTexScreen)->Init(480,272,512,2);
	#elif defined(_WIN32)
		m_pEmuScreen=new CMGEMemoryScreen();
		m_pEmuScreen->Init(480,272);
	#endif

	char* CmdFake[]={"pspectrum"};
	if(fuse_init(1, CmdFake))
		CMGELog::SysLog(CMGELog::lError,"error initalising -- giving up!\n");
	else
		CMGELog::SysLog(CMGELog::lError,"Fuse Initialized Ok\n");

	//Inicializa();

	//Emulator Options [ m_pSpeccy must be == 0 in order to init ok ]
	m_pEmuOptions=new TEmuOptions;
	
	//Menu + DefaultOptions configured
	m_pGUI=new CGUI;
	m_pGUI->Init(m_pEmuOptions);

	//configure playable control, button 0 (Action)
	m_oControl.m_cuControl.buttons[BT_CROSS]=PSP_CTRL_CROSS;
	m_oControl.m_cuControl.buttons[BT_SQUARE]=PSP_CTRL_SQUARE;
	m_oControl.m_cuControl.buttons[BT_CIRCLE]=PSP_CTRL_CIRCLE;
	m_oControl.m_cuControl.buttons[BT_TRIANGLE]=PSP_CTRL_TRIANGLE;
	m_oControl.m_cuControl.buttons[BT_LTRIGGER]=PSP_CTRL_LTRIGGER;
	m_oControl.m_cuControl.buttons[BT_RTRIGGER]=PSP_CTRL_RTRIGGER;
	m_oControl.m_cuControl.buttons[BT_START]=PSP_CTRL_START;
	m_oControl.m_cuControl.buttons[BT_SELECT]=PSP_CTRL_SELECT;
	m_oControl.m_cuControl.buttons[BT_UP]=PSP_CTRL_UP;
	m_oControl.m_cuControl.buttons[BT_DOWN]=PSP_CTRL_DOWN;
	m_oControl.m_cuControl.buttons[BT_LEFT]=PSP_CTRL_LEFT;
	m_oControl.m_cuControl.buttons[BT_RIGHT]=PSP_CTRL_RIGHT;

	
	//Load font
	m_pFontBank=new CMGESpriteBank;
	m_bOk=m_pFontBank->Init("media/Font/Font.xml");
		
	//Init selector
	m_pFileSelector=new CFileSelector;
	m_pFileSelector->Init("");
	
	m_pKeyboard=CKeyboard::Create(MT_48K);
	bResult= m_pKeyboard->Init(62,101);
	
	#ifdef _PSP
		m_pKeyboard->UploadTex((CMGEVramScreen*)m_pTexScreen,0,0);
	#endif
	
	//Clean Keyb Control structure
	memset(m_KeybState,0,sizeof(m_KeybState));
	
	//Sound
	m_pSoundGen=new CMGESoundGenerator;
	m_pSoundGen->Init();
	GetSoundDevice()->Play(m_pSoundGen);

	CMGELog::SysLog(CMGELog::lError,"Sound initialized");

	m_eState=AS_EMULATING;

	//No game basic configuration
	m_pEmuOptions->LoadOptions("cfgs/Default.cfg",false);
	
	CMGELog::SysLog(CMGELog::lError,"Default Options Loaded");
	
	//Ahoara qeu todo esta inicializado ya podemos llamar a setoptions
	SetOptions();
	
	//Inicializar MsgBox
	m_pMsgBox=new CMsgBox;
	
	CMGELog::SysLog(CMGELog::lError,"FInished loading method");
	return bResult;
}

void CPsPectrum::Unload()
{
	fuse_end();
//	Finaliza();

	SafeDelete(m_pGUI);
	SafeDelete(m_pFileSelector);
	#if defined(_PSP)
		SafeDelete(m_pEmuScreen);
		SafeDelete(m_pTexScreen);
	#endif
	SafeDelete(m_pFontBank);
	GetSoundDevice()->StopSound();
	SafeDelete(m_pSoundGen);
	SafeDelete(m_pEmuOptions);
}

//App Draw frame
void CPsPectrum::Draw()
{
	CMGERenderDevice* pRenderer=GetRenderDevice();
	switch(m_eState)
	{
		case AS_EMULATING:
		{	
			//CMGELog::SysLog(CMGELog::lError,"gonna render");
			#if defined (_PSP)
				pRenderer->RenderTexturedCuad(m_pEmuScreen->m_Lines[0],m_iRealOffsetX,m_iRealOffsetY,
					m_iRealWith,m_iRealHeight,m_iPSPPosX,m_iPSPPosY,
					m_iPSPWidth,m_iPSPHeight);
				m_pKeyboard->Render(pRenderer->GetWorkingScreen());
			#else
				m_pEmuScreen->ScreenCopy(pRenderer->GetWorkingScreen(),0,0);
				m_pKeyboard->Render(pRenderer->GetWorkingScreen());
			#endif
			//CMGELog::SysLog(CMGELog::lError,"rendered");
			break;
		}
		case AS_FILESELECTOR:
			#if defined(_PSP)
				m_pFileSelector->Render(pRenderer->GetWorkingScreen());
			#else
				m_pFileSelector->Render(GetRenderDevice()->GetWorkingScreen());
			#endif
			break;
		case AS_OPTIONS:
			#if defined(_PSP)
				m_pGUI->Render(pRenderer->GetWorkingScreen());
				//RenderCuad(m_pScreen,pRenderer,480,272);
			#else
				m_pGUI->Render(GetRenderDevice()->GetWorkingScreen());
			#endif
			
			break;
	
	}
}

void CPsPectrum::EnteringModule()
{
	GetRenderDevice()->GetFrontBuffer()->Clear(0);
	GetRenderDevice()->GetBackBuffer()->Clear(0);
	m_pSoundGen->Reset();
}

void CPsPectrum::ExitingModule()
{
	GetRenderDevice()->GetFrontBuffer()->Clear(0);
	GetRenderDevice()->GetBackBuffer()->Clear(0);
	m_pSoundGen->Reset();
	display_refresh_all();
}

void CPsPectrum::Logic()
{ 	
	static long m_lLastTime=0;
	long Ticks;
	if(!m_lLastTime)
		Ticks=m_lLastTime=CPsPectrum::GetTicks();
	else
		Ticks=CPsPectrum::GetTicks();

	float fElapsedTime=(Ticks-m_lLastTime)/CPsPectrum::GetTimeDivisor();

	switch(m_eState)
	{
		case AS_EMULATING:
		{		
			//CMGELog::SysLog(CMGELog::lError,"gonna run emu");
			DoEmulation();

			if(!m_pKeyboard->GetActive())
				SendKeyEventsToEmulation();
			
			//CMGELog::SysLog(CMGELog::lError,"displayed");
			m_pKeyboard->Run();
			
			if(m_oControl.m_cuResponse.buttons[BT_SELECT].wasPressed)
			{			
				m_eState=AS_FILESELECTOR;	
				/*m_pSoundGen->Reset();
				m_pSpeccy->SetSound(false);*/
			}
			else if(m_oControl.m_cuResponse.buttons[BT_LTRIGGER].bePushed &&
					m_oControl.m_cuResponse.buttons[BT_RTRIGGER].wasPressed)
			{
				EnteringModule();
				SaveShot("shots");
				ExitingModule();
			}
			else if(m_oControl.m_cuResponse.buttons[BT_RTRIGGER].wasPressed)
			{
				m_pKeyboard->SetActive(!m_pKeyboard->GetActive());
				ExitingModule();
			}
			else if(m_oControl.m_cuResponse.buttons[BT_START].wasPressed)
			{	
				/*m_pSoundGen->Reset();
				m_pSpeccy->SetSound(false);*/
				m_eState=AS_OPTIONS;
			}
			break;
		}
		case AS_FILESELECTOR:
		{
			CFileSelector::EAction eAction=m_pFileSelector->Run();
			if(eAction==CFileSelector::AT_SELECTED)
			{	
				char Buffer[256];
				const char* sFile=m_pFileSelector->GetLastLoadedFile();
				//if sFile==0 ".." was returned
				if(sFile)
				{	
					strcpy(Buffer,m_pFileSelector->GetCurrentPath());
					strcat(Buffer,m_pFileSelector->GetLastLoadedFile());
					libspectrum_id_t FileID;
					utils_open_file(Buffer,settings_current.auto_load,&FileID);
					m_pEmuOptions->Refresh();
					
					strcpy(Buffer,"cfgs/");
					char Buffer2[256]={0};
					GetFileFromPathWithoutExtension(m_pFileSelector->GetLastLoadedFile(),Buffer2);
					strcat(Buffer2,".cfg");
					strcat(Buffer,Buffer2);
					
					m_pEmuOptions->LoadOptions(Buffer);
					
					SetOptions();
					m_pEmuOptions->SetGUIOptions();
				}				
				m_eState=AS_EMULATING;
				ExitingModule();
			}
			else if(eAction==CFileSelector::AT_EXIT)
			{	
				m_eState=AS_EMULATING;
				ExitingModule();
			}
			break;
		}
		case AS_OPTIONS:
		{
			//Unique different run, we need boolean to know if we exit
			//the m_pGUI when exists fills emuoptions for us
			bool bExit=m_pGUI->Run(fElapsedTime);
			if(bExit)
			{
				//Apply EmuOptions -- No need to call sound enable/disable in here
				//it is called by options
				m_eState=AS_EMULATING;
				ExitingModule();
				SetOptions();
				//m_pSoundGen->Reset();
				//m_pSpeccy->SetSound(m_pEmuOptions->eSound==TEmuOptions::BO_YES);
			}
			break;
		}		
		case AS_DIALOG:
			//Everthing is done from dialog (that calls logic itself)
			break;
	}

	m_lLastTime=Ticks;
}

void CPsPectrum::PrintText(CMGEScreen* pScr,const char* sText, int x, int y,u8 r, u8 g, u8 b)
{
	int iCurChar=0;
	u8 Value;
	while(sText[iCurChar]!='\0')
	{
		Value=sText[iCurChar];
		m_pFontBank->Get(Value)->InkDraw(pScr,x+iCurChar*8,y,r,g,b);
		++iCurChar;
	}
}

void CPsPectrum::SaveShot(const char* sDir)
{
	char sPath[256];
	strcpy(sPath,sDir);
	strcat(sPath,"/");
	
	const char* sFileName=m_pFileSelector->GetLastLoadedFile();
	if(!sFileName)
		strcat(sPath,"Shot");
	else
		strcat(sPath,sFileName);
	
	bool bSuccess;
	
	//#if defined(_PSP)
	if(bSuccess=CMGETgaFile::Save(sPath,m_pEmuScreen))
		m_pFileSelector->ShotDirty(true);
	/*#else
		if(bSuccess=CMGETgaFile::Save(sPath,GetRenderDevice()->GetWorkingScreen()))
			m_pFileSelector->ShotDirty(true);
	#endif*/

	if(bSuccess)
		CPsPectrum::GetMsgBox()->ShowMsg("Screen Shot has been saved Ok.");
	else
		CPsPectrum::GetMsgBox()->ShowMsg("Screen Shot has failed to save.");
}



//Sets options to the different emulation systems getting 
void CPsPectrum::SetOptions()
{
	SetMachineOptions();
	SetScreenOptions();

	//Sound 
	//m_pEmuOptions->eSound==TEmuOptions::BO_YES ? m_pSpeccy->SetSound(true) : m_pSpeccy->SetSound(false);
	
	//Set Keyboard Transparency
	m_pKeyboard->SetTransparency(m_pEmuOptions->fKeybTrans);
	m_pGUI->SetOptionValue("Keyb Transparency",(int)(m_pEmuOptions->fKeybTrans*TEmuOptions::KT_NUM-1));

	//Set Cursor Size
	m_pKeyboard->SetCursorSize(m_pEmuOptions->iCursorSize);
	m_pGUI->SetOptionValue("Pointer Size",m_pEmuOptions->iCursorSize-1);

	settings_current.joy_kempston=1;
	
	//Speed
	#if defined _PSP
		switch(m_pEmuOptions->ePSPSpeed)
		{
			case TEmuOptions::SO_233:
				scePowerSetClockFrequency(222, 222, 111);
				break;
			case TEmuOptions::SO_266:
				scePowerSetClockFrequency(266,266,133);
				break;
			case TEmuOptions::SO_333:
				scePowerSetClockFrequency(333, 333, 166);
				
				break;
		}
	#endif

	
	//Set pokes
	for(int i=0;i<4;++i)
	{
		if(m_pEmuOptions->Pokes[i].ePokeActive==TEmuOptions::BO_YES)
		{	
			//If it is not the same, we must keep the original to allow poke off
			if(m_pEmuOptions->Pokes[i].OldData==-1)
				m_pEmuOptions->Pokes[i].OldData=readbyte(m_pEmuOptions->Pokes[i].Dir);
			writebyte( m_pEmuOptions->Pokes[i].Dir, m_pEmuOptions->Pokes[i].Data );
		}
		else if(m_pEmuOptions->Pokes[i].OldData!=-1)
			writebyte( m_pEmuOptions->Pokes[i].Dir,(u8)m_pEmuOptions->Pokes[i].OldData);
	}	
	
}

//Checks if ePSPButton 
void CPsPectrum::CheckKey(EButtons ePSPButton, TEmuOptions::EUserKeyName eActionButton)
{
	CMGEControl::ControlResponse* pControl=&GetControl()->m_cuResponse;
	
	if(pControl->buttons[ePSPButton].bePushed)
	{	
		CKeyboard::TKeyDef* pKey=&m_pEmuOptions->Controls[eActionButton];
		keyboard_press(pKey->m_Key1);
		keyboard_press(pKey->m_Key2);
		m_KeybState[eActionButton]=1; //let know taht it can be released
	}
	else if(m_KeybState[eActionButton] && !pControl->buttons[ePSPButton].bePushed)
	{	
		CKeyboard::TKeyDef* pKey=&m_pEmuOptions->Controls[eActionButton];
		keyboard_release(pKey->m_Key1);
		keyboard_release(pKey->m_Key2);
		m_KeybState[eActionButton]=0; //Made Release
	}
}

void CPsPectrum::SendKeyEventsToEmulation()
{
	CMGEControl::ControlResponse* pControl=&GetControl()->m_cuResponse;
	
	//Extra Keys
	if(pControl->buttons[BT_LTRIGGER].bePushed)
	{
		CheckKey(BT_UP,TEmuOptions::UK_LUP);
		CheckKey(BT_DOWN,TEmuOptions::UK_LDOWN);
		CheckKey(BT_LEFT,TEmuOptions::UK_LLEFT);
		CheckKey(BT_RIGHT,TEmuOptions::UK_LRIGHT);
		CheckKey(BT_SQUARE,TEmuOptions::UK_LSQUARE);
		CheckKey(BT_TRIANGLE,TEmuOptions::UK_LTRIANGLE);
		CheckKey(BT_CROSS,TEmuOptions::UK_LCROSS);
		CheckKey(BT_CIRCLE,TEmuOptions::UK_LCIRCLE);
	}
	else
	{
		CheckKey(BT_TRIANGLE,TEmuOptions::UK_TRIANGLE);
		CheckKey(BT_CROSS,TEmuOptions::UK_CROSS);
		CheckKey(BT_CIRCLE,TEmuOptions::UK_CIRCLE);
	}
	joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_RIGHT   , 1 );
	switch(m_pEmuOptions->eJoyType)
	{
		
		case TEmuOptions::JTO_KEMPSTON:
			if(pControl->up)
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_UP   , 1 );
			else
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_UP   , 0 );
			if(pControl->down)
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_DOWN   , 1 );
			else
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_DOWN   , 0 );
			if(pControl->rigth)
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_RIGHT   , 1 );
			else
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_RIGHT   , 0 );
			if(pControl->left)
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_LEFT   , 1 );
			else
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_LEFT   , 0 );
			
			if(!pControl->buttons[BT_LTRIGGER].bePushed && pControl->buttons[BT_SQUARE].bePushed)
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_FIRE   , 1 );
			else
				joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_FIRE   , 0);
			break;
		case TEmuOptions::JTO_SINCLAIR1:
		case TEmuOptions::JTO_SINCLAIR2:
		case TEmuOptions::JTO_QAOP:
		case TEmuOptions::JTO_CUSTOM:
			joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_FIRE   , 0);
			joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_DOWN   , 0);
			joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_UP   , 0);
			joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_RIGHT   , 0);
			joystick_press( JOYSTICK_TYPE_KEMPSTON, JOYSTICK_BUTTON_LEFT   , 0);

			if(!pControl->buttons[BT_LTRIGGER].bePushed)
			{	
				CheckKey(BT_UP,TEmuOptions::UK_UP);
				CheckKey(BT_DOWN,TEmuOptions::UK_DOWN);
				CheckKey(BT_LEFT,TEmuOptions::UK_LEFT);
				CheckKey(BT_RIGHT,TEmuOptions::UK_RIGHT);
				if(!pControl->buttons[BT_LTRIGGER].bePushed)
					CheckKey(BT_SQUARE,TEmuOptions::UK_SQUARE);
			}
			break;
	}
	
	

}


void CPsPectrum::SetMachineOptions()
{
	if(m_pEmuOptions->eSpectrumType==MT_48K)
	{	
		if(machine_current->machine!=LIBSPECTRUM_MACHINE_48 &&
		   machine_current->machine!=LIBSPECTRUM_MACHINE_16)
			machine_select(LIBSPECTRUM_MACHINE_48);
	}
	else if (m_pEmuOptions->eSpectrumType==MT_128KA)
	{
		if(machine_current->machine!=LIBSPECTRUM_MACHINE_PLUS2 &&
			machine_current->machine!=LIBSPECTRUM_MACHINE_PLUS2A)
			machine_select(LIBSPECTRUM_MACHINE_PLUS2);

	}
	else if(m_pEmuOptions->eSpectrumType==MT_128K)
	{
		if(machine_current->machine!=LIBSPECTRUM_MACHINE_128)
			machine_select(LIBSPECTRUM_MACHINE_128);

	}
	//Change keyboard acorddly
	SafeDelete(m_pKeyboard);
	m_pKeyboard=CKeyboard::Create(m_pEmuOptions->eSpectrumType);
	m_pKeyboard->Init(62,101);


	#ifdef _PSP
		m_pKeyboard->UploadTex((CMGEVramScreen*)m_pTexScreen,0,0);
	#endif

	//Set Autoload
	settings_current.auto_load=m_pEmuOptions->eAutoload==TEmuOptions::BO_YES ? 1 : 0;
}

void CPsPectrum::SetScreenOptions()
{
	switch(m_pEmuOptions->eScreenSize)
	{
		case TEmuOptions::SS_1_1:
			m_iRealOffsetX=0; 
			m_iRealOffsetY=0; 
			m_iRealWith=SPECCY_SCREEN_WIDTH_BORDER;
			m_iRealHeight=SPECCY_SCREEN_HEIGHT_BORDER; 
			m_iPSPWidth=SPECCY_SCREEN_WIDTH_BORDER;  
			m_iPSPHeight=SPECCY_SCREEN_HEIGHT_BORDER;
			break;
		case TEmuOptions::SS_1_1_NB:
			m_iRealOffsetX=32; 
			m_iRealOffsetY=24; 
			m_iRealWith=SPECCY_SCREEN_WIDTH;
			m_iRealHeight=SPECCY_SCREEN_HEIGHT; 
			m_iPSPWidth=SPECCY_SCREEN_WIDTH_BORDER;  
			m_iPSPHeight=SPECCY_SCREEN_HEIGHT_BORDER;
			break;
		case TEmuOptions::SS_15_15:
			m_iRealOffsetX=0; 
			m_iRealOffsetY=0; 
			m_iRealWith=SPECCY_SCREEN_WIDTH_BORDER;
			m_iRealHeight=SPECCY_SCREEN_HEIGHT_BORDER; 
			m_iPSPWidth=SPECCY_SCREEN_WIDTH_BORDER*1.3f;  
			m_iPSPHeight=SPECCY_SCREEN_HEIGHT_BORDER*1.3f;
			break;
		case TEmuOptions::SS_FULLSCREEN:
			m_iRealOffsetX=0; 
			m_iRealOffsetY=0; 
			m_iRealWith=SPECCY_SCREEN_WIDTH_BORDER;
			m_iRealHeight=SPECCY_SCREEN_HEIGHT_BORDER; 
			m_iPSPWidth=480;  
			m_iPSPHeight=272;
			break;
		case TEmuOptions::SS_15_15_NB:
			m_iRealOffsetX=32; 
			m_iRealOffsetY=24; 
			m_iRealWith=SPECCY_SCREEN_WIDTH;
			m_iRealHeight=SPECCY_SCREEN_HEIGHT; 
			m_iPSPWidth=SPECCY_SCREEN_WIDTH*1.4;  
			m_iPSPHeight=SPECCY_SCREEN_HEIGHT*1.4;
			break;
		case TEmuOptions::SS_FULLSCREEN_NB:
			m_iRealOffsetX=32; 
			m_iRealOffsetY=24; 
			m_iRealWith=SPECCY_SCREEN_WIDTH;
			m_iRealHeight=SPECCY_SCREEN_HEIGHT; 
			m_iPSPWidth=480;
			m_iPSPHeight=272;
			break;


	}
	
	//Calculate position to be centered
	m_iPSPPosX=(480-m_iPSPWidth)/2;
	m_iPSPPosY=(272-m_iPSPHeight)/2;
	
}

//File utils
void CPsPectrum::GetFileFromPath(const char* sPath, char* Buffer)
{
	const char* p=strrchr(sPath,'/');
	if(p)
		strcpy(Buffer,p+1);
	else
		strcpy(Buffer,sPath);
}

void CPsPectrum::GetDirFromPath(const char* sPath, char* Buffer)
{
	char* p;
	strcpy(Buffer,sPath);
	p=strrchr(Buffer,'/');
	if(p)
		*(p+1)='\0';
	else
		Buffer[0]='\0';
}

void CPsPectrum::GetFileFromPathWithoutExtension(const char* sPath, char* Buffer)
{
	char* p;
	GetFileFromPath(sPath,Buffer);
	strupr(Buffer);
	p=strrchr(Buffer,'.');
	if(p)
		*p='\0';
	else
		Buffer[0]='\0';
}

void CPsPectrum::GetExtensionFromPath(const char* sFile, char* Buffer)
{
	const char* p;
	p=strrchr(sFile,'.');
	if(p)
		strcpy(Buffer,p+1);
	else
		Buffer[0]='\0';
	
	strupr(Buffer);
}

void CPsPectrum::GetPathUpDir(const char* sPath,char * Buffer)
{
	GetDirFromPath(sPath,Buffer);
	//Delete last '/'
	Buffer[strlen(Buffer)-1]='\0';
	char* p=strrchr(Buffer,'/');
	*(p+1)='\0';
}
		
void CPsPectrum::GetPathDownDir(const char* sPath, const char* sDir,char * Buffer)
{
	GetDirFromPath(sPath,Buffer);
	strcat(Buffer,sDir);
	strcat(Buffer,"/");
}