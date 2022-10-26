#ifndef _SPECTRUMOPTIONS_
#define _SPECTRUMOPTIONS_

#include "EmulatorDefines.h"
#include "CKeyboard.h"


struct TEmuOptions
{
	enum EControlOption {CT_DIGITAL, CT_ANALOGIC};
	enum EBoolOption	{BO_YES, BO_NO,BO_NUM};
	enum ESpeedOption   {SO_233, SO_266, SO_333, SO_NUM};
	enum EJoyTypeOption {JTO_KEMPSTON, JTO_SINCLAIR1, JTO_SINCLAIR2, JTO_QAOP, JTO_CUSTOM,JTO_NUM};
	enum EUserKeyName   {UK_UP=0,UK_DOWN,UK_LEFT,UK_RIGHT,UK_SQUARE,UK_TRIANGLE,
						 UK_CROSS,UK_CIRCLE,UK_LUP,UK_LDOWN,UK_LLEFT,UK_LRIGHT,UK_LSQUARE,
						 UK_LTRIANGLE,UK_LCROSS,UK_LCIRCLE,UK_NUM};
	enum EKeyb48Option  {K48_1=0,K48_2,K48_3,K48_4,K48_5,K48_6,K48_7,K48_8,K48_9,K48_0,
		K48_A,K48_B,K48_C,K48_D,K48_E,K48_F,K48_G,K48_H,K48_I,K48_J,K48_K,K48_L,K48_M,
		K48_N,K48_O,K48_P,K48_Q,K48_R,K48_S,K48_T,K48_U,K48_V,K48_W,K48_X,K48_Y,K48_Z,
		K48_ENTER, K48_CAPSSHIFT,K48_SYBOLSHIFT,K48_SPACE,K48_NUM};
	enum ECursorSizeOption {CS_1,CS_2,CS_3,CS_4,CS_5,CS_6,CS_7,CS_8,CS_NUM};
	enum EKeybTransOption { KT_10=0,KT_20,KT_30,KT_40,KT_50,KT_60,KT_70,KT_80,KT_90,KT_100,KT_NUM};
	enum EScreenSize {SS_1_1, SS_1_1_NB, SS_15_15, SS_15_15_NB, SS_FULLSCREEN, SS_FULLSCREEN_NB, SS_NUM};
	
	struct TPokeOption
	{
		char	sPoke[256];
		EBoolOption ePokeActive;
		s16		OldData;
		u16		Dir;
		u8		Data;
	};

	EBoolOption     eSound;
    EMachineType    eSpectrumType;
    EJoyTypeOption  eJoyType;
    ESpeedOption    ePSPSpeed;
    EBoolOption     eVSync;
	CKeyboard::TKeyDef	Controls[UK_NUM]; //What speccy Key for every control
	u16				iCursorSize;
	float			fKeybTrans;
	EScreenSize		eScreenSize;
	EBoolOption		eAutoload;
	TPokeOption		Pokes[4];

	void	SetDefaults();
	bool	SaveOptions(const char* sFileName);
	bool	LoadOptions(const char* sFileName, bool bShowError=false);
	void	SetGUIOptions();
	EKeyb48Option GetKeyDefineFromName(const char* sKeyName);
	void	Refresh();
	void	ConvertPoke(const char* sPoke,u16& iDir, u8& iData);
	
	//This is synched with the TEmuOptions enum, do not change!!!!!.
	static const char* psButtonOptions[UK_NUM];
	static const char* psJoyTypeOptions[JTO_NUM];
	static const char* psYesNoOptions[BO_NUM];
	static const char* psSpeedOptions[SO_NUM];
	static const char* psKeys48Options[K48_NUM];
	static const char* psMachineTypeOptions[MT_NUM];
	static const char* psCursorSizeOptions[CS_NUM];
	static const char* psKeybTrans[KT_NUM];
	static const char* psScreenSize[TEmuOptions::SS_NUM];
};
    

#endif
