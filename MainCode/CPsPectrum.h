//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - Sample 01 application class
//
// Tbis its the implementation of the sample 01
//////////////////////////////////////////////////////////////////////////

#if !defined(__CPsPectrum__)
#define __CPsPectrum__


#include "TEmuOptions.h"

//includes some classes
#include "CMGEApp.h"
#include "CMGETgaFile.h"

class CGUI;
class CFileSelector;
class CMGESpriteBank;
class CMGESoundGenerator;
class CKeyboard;
class CMGEVramScreen;
class CMsgBox;

//Our App
class CPsPectrum : public CMGEApp
{
	//this inherits from?
	typedef CMGEApp inherited;
	
	public:
		enum EButtons
		{
			BT_CROSS=0, BT_SQUARE, BT_CIRCLE, BT_TRIANGLE, BT_LTRIGGER, BT_RTRIGGER,
			BT_START, BT_SELECT,BT_UP,BT_DOWN,BT_LEFT,BT_RIGHT
		};
		
		enum EAppState
		{
			AS_EMULATING, AS_FILESELECTOR,AS_OPTIONS,AS_DIALOG
		};
		
		enum Global
		{
			SPECCY_SCREEN_WIDTH=256,
			SPECCY_SCREEN_HEIGHT=192,
			SPECCY_SCREEN_WIDTH_BORDER=320,
			SPECCY_SCREEN_HEIGHT_BORDER=240
		};
		
		enum EMsgBoxType
		{
			MB_INFO,
			MB_YES_NO
		};
			
		//base object style of constructor & destructor
		CPsPectrum()			{}		
		virtual ~CPsPectrum()	{ End(); }	

		//App AI Logic		
		void Logic();

		//App Draw frame
		void Draw();

		//Load, Unload app data
		bool Load();
		void Unload();
		
		inline static DWORD GetTicks()
		{
			#if defined(_PSP)
				return sceKernelLibcClock();
			#else 
				return GetTickCount();
			#endif
		}
		
		inline static float GetTimeDivisor()
		{
			#if defined(_WINDOWS) 
				return 1000.0f;
			#else 
				return 1000000.0f;
			#endif
		}
		
		inline CMGEScreen* GetEmuScr() { return m_pEmuScreen;}

		void PrintText(CMGEScreen* pScr,const char* sText, int x, int y, u8 r=0xFF, u8 g=0xFF, u8 b=0xFF);
		
		inline bool LetMove(CPsPectrum::EButtons eButton, CMGEControl* pControl, float fAccumTime, float fDelay )
		{
			if(pControl->m_cuResponse.buttons[eButton].wasPressed ||
				(!pControl->m_cuResponse.buttons[eButton].wasPressed && fAccumTime>=fDelay))
				return true;
			else
				return false;
		}
		
		void ShowMessageBox(const char* sText);
		
		TEmuOptions* GetEmuOptions() { return m_pEmuOptions; }
		CGUI*		 GetGUI()		{ return m_pGUI; }
		CFileSelector* GetFileSelector() { return m_pFileSelector; }
		static CPsPectrum* GetApp() { return (CPsPectrum*)m_pApp;}
		CMGESoundGenerator* GetSoundGen() { return m_pSoundGen;}
		CMsgBox*	GetMsgBox() { return m_pMsgBox;}
		EAppState	GetAppState() { return m_eState;}
		void		SetAppState(EAppState eState) { m_eState=eState;}
		
		static void GetFileFromPath(const char* sPath,char* Buffer);
		static void GetDirFromPath(const char* sPath, char* Buffer);
		static void GetFileFromPathWithoutExtension(const char* sPath, char* Buffer);
		static void GetExtensionFromPath(const char* sFile, char* Buffer);
		static void GetPathUpDir(const char* sPath,char * Buffer);
		static void GetPathDownDir(const char* sPath, const char* sDir,char * Buffer);
	private:
		
		void SetOptions();
		void SetMachineOptions();
		void SetScreenOptions();
		void SendKeyEventsToEmulation();
		void LoadDefaultOptions();
		void CheckKey(EButtons ePSPButton, TEmuOptions::EUserKeyName eActionButton);
		void SaveShot(const char* sDir);
		void ConvertPoke(const char* sPoke,u16& iDir, u8& iData);
		
		
		void EnteringModule();
		void ExitingModule();

		EAppState		m_eState;
		EMsgBoxType     m_eMsgBoxType;
		CGUI*			m_pGUI;
		CKeyboard*		m_pKeyboard;
		CFileSelector*  m_pFileSelector;
		CMGEScreen* m_pEmuScreen;
		CMGEScreen* m_pTexScreen;
		CMGESpriteBank*	m_pFontBank;
		CMGESoundGenerator* m_pSoundGen;
		TEmuOptions*	m_pEmuOptions;
		CMsgBox*		m_pMsgBox;
		int				m_iRealOffsetX; 
		int				m_iRealOffsetY; //Offset to start getting pixels from real screen
		int				m_iRealWith;
		int				m_iRealHeight; //Height/Width that we want to map to PsPscreen
		int				m_iPSPPosX;		//Ofsset from 0,0 whre screen starts drawing
		int				m_iPSPPosY;
		int				m_iPSPWidth;  //Final width and height of the mapped real screen (scaling)
		int				m_iPSPHeight;

		//We need this to only send a key release to the emulation this way we can have
		//serveral buttons pushing the same key (if not one will pushed and the others will
		//erase it.
		char			m_KeybState[TEmuOptions::UK_NUM];
};
#endif
