#include "CKeyboard48k.h"
#include "CMGETgaFile.h"
#include "CMGELog.h"
#include "CPsPectrum.h"
#include "EmulatorDefines.h"
#include "fuse/keyboard.h"
#include "CKeyboard48k.h"
#include "CKeyboard128k.h"
CKeyboard* CKeyboard::Create(EMachineType eMachineType)
{
	CKeyboard* pKeyboard;
	//Init Keyboard
	if(eMachineType==MT_48K)
		pKeyboard=new CKeyboard48k;
	else if (eMachineType==MT_128K || eMachineType==MT_128KA)
		pKeyboard=new CKeyboard128k;
	return pKeyboard;
}


bool CKeyboard::Init(int iXPos, int iYPos,const char* sImgFileName)
{
    End();

    m_bOK=true;
    
	
	//Position on screen
    m_iXPos=iXPos;
    m_iYPos=iYPos;

	m_iNumKeys=0;

	
    m_iLastKeyPressedIndex=-1;
    

    m_pKeyboardImage=new CMGETgaFile;
	if(!m_pKeyboardImage->Init(sImgFileName))
		m_bOK=false;
	
	if(m_bOK)
    {
		m_bActive=false;
		m_iCursorSize=4;
		m_iXPosCursor=iXPos+m_pKeyboardImage->m_Width /2;
		m_iYPosCursor=iYPos+m_pKeyboardImage->m_Height/2;
		
		m_CursorColors[0]=CPsPectrum::GetApp()->GetRenderDevice()->GetColor(255,255,255);
		m_CursorColors[1]=CPsPectrum::GetApp()->GetRenderDevice()->GetColor(0,0,0);
		m_CursorColors[2]=CPsPectrum::GetApp()->GetRenderDevice()->GetColor(255,0,0);
		m_CursorColors[3]=CPsPectrum::GetApp()->GetRenderDevice()->GetColor(0,255,0);
		m_CursorColors[4]=CPsPectrum::GetApp()->GetRenderDevice()->GetColor(0,0,255);
		m_wCursorColorIndex=0;
		
		//No keep key press pressed at first
		m_iKeepPressedKey=KEYBOARD_NONE;
		
	}
	
    if(!m_bOK)
	{    
		CMGELog::SysLog(CMGELog::lError,"Keyb Base class Init Fail...");
		End();
	}
	else
		CMGELog::SysLog(CMGELog::lError,"Keyb Base class Inited Ok");
    
   return m_bOK;

}

void CKeyboard::End()
{
    if (m_bOK)
    {
        SafeDelete(m_pKeyboardImage);
        m_bOK=false;

		CMGELog::SysLog(CMGELog::lError,"Releasing keyb base class\n");
	 }
}


int CKeyboard::GetKeyUnderCursor()
{
	int i=0;
	
	TKeyBox* pKeyBox;
	while(i<m_iNumKeys)
	{
		pKeyBox=&m_pKeyMapping[i];

		while(pKeyBox)
		{
			if(pKeyBox->m_Box.IsPointIn(GetRelCursorPosX(),GetRelCursorPosY()))
				return i;
			else
				pKeyBox=pKeyBox->m_pNext;
			
		}
	
		++i;
	}

	return -1;
}

void CKeyboard::Render(CMGEScreen* pScreen)
{
	if(!m_bActive)
		return;

	int iKeyIndex=GetKeyUnderCursor();
	#if defined(_PSP)
		CMGERenderDevice* pRenderer=CPsPectrum::GetApp()->GetRenderDevice();
		pRenderer->EnableColorBlend(m_fTrans);
		pRenderer->RenderTexturedCuad(m_pKeybTex,0,0,m_pKeyboardImage->m_Width,m_pKeyboardImage->m_Height,m_iXPos,m_iYPos,m_pKeyboardImage->m_Width,m_pKeyboardImage->m_Height);
		if(iKeyIndex!=-1)
		{
			CBox* pBox=&m_pKeyMapping[iKeyIndex].m_Box;
			pRenderer->RenderColoredCuad(pBox->m_iTopLeftX+m_iXPos,pBox->m_iTopLeftY+m_iYPos,pBox->m_iBottomRightX-pBox->m_iTopLeftX,
				pBox->m_iBottomRightY-pBox->m_iTopLeftY,0xFFFF);
		}
		pRenderer->DisableColorBlend();
		pRenderer->RenderColoredCuad(m_iXPosCursor,m_iYPosCursor,m_iCursorSize,m_iCursorSize,m_CursorColors[m_wCursorColorIndex]);
	
	#else
		m_pKeyboardImage->ScreenCopy(pScreen,m_iXPos, m_iYPos);
		if(iKeyIndex!=-1)
		{
			CBox* pBox=&m_pKeyMapping[iKeyIndex].m_Box;
			pScreen->Box(pBox->m_iTopLeftX+m_iXPos,pBox->m_iTopLeftY+m_iYPos,pBox->m_iBottomRightX+m_iXPos,
				pBox->m_iBottomRightY+m_iYPos,0xFFFF);
		}
		
		//Draw Key that is keep pressed if any
		if(m_iKeepPressedKey!=KEYBOARD_NONE)
		{
			CBox* pBox=&m_pKeyMapping[m_iKeepPressedKey].m_Box;
			pScreen->Box(pBox->m_iTopLeftX+m_iXPos,pBox->m_iTopLeftY+m_iYPos,pBox->m_iBottomRightX+m_iXPos,
				pBox->m_iBottomRightY+m_iYPos,0xFF00);
		}
		int iHalfSize=m_iCursorSize/2;
		pScreen->Box(m_iXPosCursor-iHalfSize, m_iYPosCursor-iHalfSize,m_iXPosCursor+iHalfSize,m_iYPosCursor+iHalfSize,m_CursorColors[m_wCursorColorIndex]);
	
	#endif
}    


void CKeyboard::Run()
{
	if(!m_bActive)
		return;
	
	CMGEControl* pControl=CPsPectrum::GetApp()->GetControl();
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_CROSS].bePushed)
	{    
		//If a key was pressed first unpress it
		CheckKeyRelease();
		CheckKeyPress(pControl->m_cuResponse.buttons[CPsPectrum::BT_CROSS].wasPressed);
	}
	else
        CheckKeyRelease();
       
   
    if((pControl->m_cuResponse.rigth))
    {
        m_iXPosCursor+=CURSOR48_SPEEDX;
		if(GetRelCursorPosX()+m_iCursorSize>m_pKeyboardImage->m_Width)
            m_iXPosCursor=m_iXPos+m_pKeyboardImage->m_Width-m_iCursorSize;
    }
    if((pControl->m_cuResponse.left) )
    {
        m_iXPosCursor-=CURSOR48_SPEEDX;
        if(m_iXPosCursor<m_iXPos)
            m_iXPosCursor=m_iXPos;
    }

    if((pControl->m_cuResponse.up))
    {
        m_iYPosCursor-=CURSOR48_SPEEDY;
        if(m_iYPosCursor<m_iYPos)
            m_iYPosCursor=m_iYPos;
    }
    
    if((pControl->m_cuResponse.down))
    {
        m_iYPosCursor+=CURSOR48_SPEEDY;
		if(GetRelCursorPosY()+m_iCursorSize>m_pKeyboardImage->m_Height)
            m_iYPosCursor=m_iYPos+m_pKeyboardImage->m_Height-m_iCursorSize;
    }   
    
	if((pControl->m_cuResponse.buttons[CPsPectrum::BT_TRIANGLE].wasPressed))
	{
		++m_wCursorColorIndex;
		if(m_wCursorColorIndex>MAX_CURSOR_COLORS-1)
			m_wCursorColorIndex=0;
	}   
	
}


const CKeyboard::TKeyDef* CKeyboard::GetKeyDefFromName(EMachineType eMachineType,const char* sKeyName)
{
	bool bFound=false;
	int i=0;
	int iNumKeys;
	
	if(eMachineType==MT_48K)
		iNumKeys=MAX_KEYS_48K;
	else
		iNumKeys=MAX_KEYS_128K;
	
	while(!bFound && i<iNumKeys)
	{
		if(!strcmp(m_KeyDefs[i].m_Name,sKeyName))
			return &m_KeyDefs[i];
		++i;
	}
	
	return 0;
}

int CKeyboard::GetIndexFromName(EMachineType eMachineType,const char* sKeyName)
{
	bool bFound=false;
	int i=0;
	int iNumKeys;
	
	if(eMachineType==MT_48K)
		iNumKeys=MAX_KEYS_48K;
	else
		iNumKeys=MAX_KEYS_128K;
	
	while(!bFound && i<iNumKeys)
	{
		if(!strcmp(m_KeyDefs[i].m_Name,sKeyName))
			return i;
		++i;
	}
	
	return -1;
}

void CKeyboard::UploadTex(CMGEVramScreen* pScr,int iX, int iY)
{
	CMGELog::SysLog(CMGELog::lError,"Gona do Texture Upload");
	m_pKeyboardImage->ScreenCopy((CMGEScreen*)pScr,iX,iY);
	m_pKeybTex=pScr->m_Lines[0]+iY*512+iX; //Tex Start
	CMGELog::SysLog(CMGELog::lError,"Texture Uploaded");
}


//Ask every Box if coordinates are in
//bFirstTime tells us if the key was jsut pressed and not keep it
void CKeyboard::CheckKeyPress(bool bFirstPress)
{
	int i=0;
	bool bExit=false;

	TKeyBox* pKeyBox;

	while(i<m_iNumKeys && !bExit)
	{
		pKeyBox=&m_pKeyMapping[i];
		while(pKeyBox && !bExit)
		{

			if(pKeyBox->m_Box.IsPointIn(GetRelCursorPosX(),GetRelCursorPosY()))
			{
				//Mandar la pulsacion de la tecla (Mandar solo la primera pulsacion si es una telca
				//que puedes dejar pulsada en el teclado).
				if(m_KeyDefs[i].m_bKeepPressed==true)
				{			
					//Es la primera pulsacion? (la uníca que nos intersas)
					if(bFirstPress)
					{
						//if another keep key pressed was pressed
						if(m_iKeepPressedKey!=KEYBOARD_NONE)
						{
							//if it is the same, unpress it 
							if(m_iKeepPressedKey==i)
							{						
								keyboard_release(m_KeyDefs[i].m_Key1);
								keyboard_release(m_KeyDefs[i].m_Key2);
								m_iKeepPressedKey=KEYBOARD_NONE;

							}
						}
						else //if not keep pressed key was pressed before press this one
						{	
							m_iKeepPressedKey=i;
							keyboard_press(m_KeyDefs[i].m_Key1);
							keyboard_press(m_KeyDefs[i].m_Key2);
						}
					}
				}
				else
				{	
					keyboard_press(m_KeyDefs[i].m_Key1);
					keyboard_press(m_KeyDefs[i].m_Key2);
					m_iLastKeyPressedIndex=i;
				}

				bExit=true;
			}                
			++i;


			pKeyBox=pKeyBox->m_pNext;
		}
	}
}

void CKeyboard::CheckKeyRelease()
{
	//We processed a key press, but only if this is not a "can keep pressed", because
	//this only releases when we press it again
	if(m_iLastKeyPressedIndex!=-1 && m_KeyDefs[m_iLastKeyPressedIndex].m_bKeepPressed==false)
	{	
		keyboard_release(m_KeyDefs[m_iLastKeyPressedIndex].m_Key1);
		keyboard_release(m_KeyDefs[m_iLastKeyPressedIndex].m_Key2);
	}
}




//Keyboard definition
CKeyboard48k::TKeyDef CKeyboard::m_KeyDefs[MAX_KEYS]=
{
	{"1",false,KEYBOARD_1,     KEYBOARD_NONE},//0
	{"2",false,KEYBOARD_2,     KEYBOARD_NONE},//1
	{"3",false,KEYBOARD_3,     KEYBOARD_NONE},//2
	{"4",false,KEYBOARD_4,     KEYBOARD_NONE},//3
	{"5",false,KEYBOARD_5,     KEYBOARD_NONE},//4
	{"6",false,KEYBOARD_6,     KEYBOARD_NONE},//5
	{"7",false,KEYBOARD_7,     KEYBOARD_NONE},//6
	{"8",false,KEYBOARD_8,     KEYBOARD_NONE},//7
	{"9",false,KEYBOARD_9,     KEYBOARD_NONE},//8
	{"0",false,KEYBOARD_0,     KEYBOARD_NONE},//9
	{"Q",false,KEYBOARD_q,     KEYBOARD_NONE},//10
	{"W",false,KEYBOARD_w,     KEYBOARD_NONE},//11
	{"E",false,KEYBOARD_e,     KEYBOARD_NONE},//12
	{"R",false,KEYBOARD_r,     KEYBOARD_NONE},//13
	{"T",false,KEYBOARD_t,     KEYBOARD_NONE},//14
	{"Y",false,KEYBOARD_y,     KEYBOARD_NONE},//15
	{"U",false,KEYBOARD_u,     KEYBOARD_NONE},//16
	{"I",false,KEYBOARD_i,     KEYBOARD_NONE},//17
	{"O",false,KEYBOARD_o,     KEYBOARD_NONE},//18
	{"P",false,KEYBOARD_p,     KEYBOARD_NONE},//19
	{"A",false,KEYBOARD_a,     KEYBOARD_NONE},//20
	{"S",false,KEYBOARD_s,     KEYBOARD_NONE},//21
	{"D",false,KEYBOARD_d,     KEYBOARD_NONE},//22
	{"F",false,KEYBOARD_f,     KEYBOARD_NONE},//23
	{"G",false,KEYBOARD_g,     KEYBOARD_NONE},//24
	{"H",false,KEYBOARD_h,     KEYBOARD_NONE},//25
	{"J",false,KEYBOARD_j,     KEYBOARD_NONE},//26
	{"K",false,KEYBOARD_k,     KEYBOARD_NONE},//27
	{"L",false,KEYBOARD_l,     KEYBOARD_NONE},//28
	{"ENTER"	 ,false,KEYBOARD_Enter, KEYBOARD_NONE},//29
	{"CAPS SHIFT",true,	KEYBOARD_Caps,KEYBOARD_NONE },//30
	{"Z",false,KEYBOARD_z,     KEYBOARD_NONE},//31
	{"X",false,KEYBOARD_x,     KEYBOARD_NONE},//32
	{"C",false,KEYBOARD_c,     KEYBOARD_NONE},//33
	{"V",false,KEYBOARD_v,     KEYBOARD_NONE},//34
	{"B",false,KEYBOARD_b,     KEYBOARD_NONE},//35
	{"N",false,KEYBOARD_n,     KEYBOARD_NONE},//36
	{"M",false,KEYBOARD_m,     KEYBOARD_NONE},//37
	{"SYMBOL SHIFT",false,KEYBOARD_NONE,  KEYBOARD_Symbol},//38
	{"SPACE",false,KEYBOARD_space, KEYBOARD_NONE},//39
	{"TRUE VIDEO",false,KEYBOARD_3,KEYBOARD_Caps},//40
	{"INV VIDEO",false,KEYBOARD_4,KEYBOARD_Caps},//41
	{"BREAK",false,KEYBOARD_space,KEYBOARD_Caps},//42
	{"DELETE",false,KEYBOARD_0,KEYBOARD_Caps},//43
	{"GRAPH",false,KEYBOARD_9,KEYBOARD_Caps},//44
	{"EXTEND MODE",false},//45
	{"EDIT",false,KEYBOARD_1,     KEYBOARD_Caps},//46
	{"CAPS LOCK",false,KEYBOARD_2, KEYBOARD_Caps},//47
	{"CAPS SHIFT",true,	KEYBOARD_Caps,KEYBOARD_NONE },//48
	{";",false, KEYBOARD_n,     KEYBOARD_Symbol},//49
	{"\"",false,KEYBOARD_p,     KEYBOARD_Symbol},//50
	{"LEFT ARROW",false,KEYBOARD_5,     KEYBOARD_Caps},//51
	{"RIGHT ARROW",false,KEYBOARD_8,     KEYBOARD_Caps},//52
	{"UP ARROW",false,KEYBOARD_7,     KEYBOARD_Caps},//53
	{"DOWN ARROW",false,KEYBOARD_6,     KEYBOARD_Caps},//54
	{",",false,KEYBOARD_n,     KEYBOARD_Symbol},//55
	{"R SIMBOL SHIFT",false,KEYBOARD_NONE,  KEYBOARD_Symbol},//56
};
