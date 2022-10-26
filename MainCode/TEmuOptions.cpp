#include "TEmuOptions.h"
#include "CGUI.h"
#include "CMGELog.h"
#include "CPsPectrum.h"
#include "TinyXml/TinyXml.h"
#include "EmulatorDefines.h"
#include "machine.h"

const char* TEmuOptions::psButtonOptions[TEmuOptions::UK_NUM]=
{"Up","Down","Left","Right","Square","Triangle","Cross","Circle",
"L+Up","L+Down","L+Left","L+Right","L+Square","L+Triangle","L+Cross","L+Circle"};

const char* TEmuOptions::psYesNoOptions[TEmuOptions::BO_NUM]={"Yes", "No"};

const char* TEmuOptions::psSpeedOptions[TEmuOptions::SO_NUM]={"233","266","333"};

const char* TEmuOptions::psJoyTypeOptions[TEmuOptions::JTO_NUM]=
	{"KEMPSTON","SINCLAIR 1","SINCLAIR 2","QAOPSPACE","CUSTOM"};

const char* TEmuOptions::psKeys48Options[TEmuOptions::K48_NUM]={
"1","2","3","4","5","6","7","8","9","0",
"A","B","C","D","E","F","G","H","I","J","K","L","M",
"N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
"ENTER", "CAPS SHIFT","SYMBOL SHIFT","SPACE"};


const char* TEmuOptions::psMachineTypeOptions[MT_NUM]={"48k Spectrum","128k Spectrum","128k Spectrum +2"};

const char* TEmuOptions::psCursorSizeOptions[TEmuOptions::CS_NUM]={"1","2","3","4","5","6","7","8"};

const char* TEmuOptions::psKeybTrans[TEmuOptions::KT_NUM]={"10%","20%","30%","40%","50%","60%","70%","80%","90%","100%"};

const char* TEmuOptions::psScreenSize[TEmuOptions::SS_NUM]={"1.0 x 1.0","1.0x1.0 NB","1.5 x 1.5","1.5 x 1.5 NB","FULL SCREEN","FULL SCREEN NB"};




bool TEmuOptions::SaveOptions(const char* sFileName)
{
	bool bResult=true;

	FILE* pFile=fopen(sFileName,"w");

	if(!pFile)
	{
		CMGELog::SysLog(CMGELog::lError,"Could not write config file %s", sFileName);
		bResult=false;
	}
	
	if(bResult)
	{
		fprintf(pFile,"<Config>\n\r");
		fprintf(pFile,"\t<Sound> %i </Sound>\n\r",eSound);
		fprintf(pFile,"\t<SpectrumType> %i </SpectrumType>\n\r",eSpectrumType);
		fprintf(pFile,"\t<JoyType> %i </JoyType>\n\r",eJoyType);
		fprintf(pFile,"\t<PSPSpeed> %i </PSPSpeed>\n\r",ePSPSpeed);
		fprintf(pFile,"\t<VSync> %i </VSync>\n\r",eVSync);
		fprintf(pFile,"\t<CursorSize> %i </CursorSize>\n\r",iCursorSize);
		fprintf(pFile,"\t<KeybTrans> %2.2f </KeybTrans>\n\r",fKeybTrans);
		fprintf(pFile,"\t<ScreenSize> %i </ScreenSize>\n\r",eScreenSize);
		fprintf(pFile,"\t<Autoload> %i </Autoload>\n\r",eAutoload);
		for(int i=0;i<4;++i)
			fprintf(pFile,"\t<Poke%i Active=%i> %s </Poke%i>\n\r",i,Pokes[i].ePokeActive,Pokes[i].sPoke,i);
		
		for(int i=0;i<UK_NUM; ++i)
			fprintf(pFile,"\t<Key%i>%s</Key%i>\n\r",i,Controls[i].m_Name,i);
		fprintf(pFile,"</Config>\n\r");
		fclose(pFile);
	
	}
	return bResult;

}

void TEmuOptions::SetDefaults()
{
	TEmuOptions* pOp=CPsPectrum::GetApp()->GetEmuOptions();

	pOp->eJoyType=TEmuOptions::JTO_KEMPSTON;
	pOp->eSound=TEmuOptions::BO_YES;
	pOp->eSpectrumType=MT_48K;
	pOp->ePSPSpeed=TEmuOptions::SO_233;
	pOp->eVSync=TEmuOptions::BO_YES;
	pOp->eScreenSize=SS_15_15;
	pOp->iCursorSize=4;
	pOp->fKeybTrans=0.8f;
	pOp->eAutoload=TEmuOptions::BO_YES;
	for(int i=0;i<4;++i)
	{	
		strcpy(Pokes[i].sPoke,"65535,255");
		Pokes[i].ePokeActive=TEmuOptions::BO_NO;
		Pokes[i].OldData=-1;
		Pokes[i].Data=255;
		Pokes[i].Dir=65535;
	}
	
	for(int i=0;i<UK_NUM; ++i)
		Controls[i]=*CKeyboard::GetKeyDefFromIndex(pOp->eSpectrumType,TEmuOptions::K48_1);

}

void TEmuOptions::ConvertPoke(const char* sPoke,u16& iDir, u8& iData)
{
	char sBuffer[256];

	char *pDir=strchr((char*)sPoke,',');
	strncpy(sBuffer,sPoke,pDir-sPoke);
	sBuffer[pDir-sPoke]='\0';
	iDir=atoi(sBuffer);
	strncpy(sBuffer,pDir+1,3);
	sBuffer[3]='\0';
	iData=atoi(sBuffer);

}


//Load only if file exists if not, do not touch TEmuOptions, it is supposed
//that have been defaulted in a previous call
bool TEmuOptions::LoadOptions(const char* sFileName, bool bShowError)
{
	bool bResult=true;

	TEmuOptions* pOp=CPsPectrum::GetApp()->GetEmuOptions();

	TiXmlDocument oDocument;
	
	if (! oDocument.LoadFile(sFileName)) 
	{
		CMGELog::SysLog(CMGELog::lError,"Error loading configuration file = \"%s\" : %s.", 
			sFileName,oDocument.ErrorDesc());

		bResult=false;
	}
	
	if(bResult)
	{
		//Solo poner defaults si realmete existe el cfg.
		SetDefaults();
		TiXmlElement* pRootEl=oDocument.FirstChildElement();
		TiXmlElement* pEl=pRootEl->FirstChildElement("Sound");
		if(pEl)
			pOp->eSound=(EBoolOption)(atoi(pEl->GetText()));
		pEl=pRootEl->FirstChildElement("SpectrumType");
		if(pEl)
			pOp->eSpectrumType=(EMachineType)(atoi(pEl->GetText()));
		pEl=pRootEl->FirstChildElement("JoyType");
		if(pEl)
			pOp->eJoyType=(EJoyTypeOption)(atoi(pEl->GetText()));
		pEl=pRootEl->FirstChildElement("PSPSpeed");
		if(pEl)
			pOp->ePSPSpeed=(ESpeedOption)(atoi(pEl->GetText()));
		pEl=pRootEl->FirstChildElement("VSync");
		if(pEl)
			pOp->eVSync=(EBoolOption)(atoi(pEl->GetText()));
		pEl=pRootEl->FirstChildElement("ScreenSize");
		if(pEl)
			pOp->eScreenSize=(EScreenSize)(atoi(pEl->GetText()));
		pEl=pRootEl->FirstChildElement("CursorSize");
		if(pEl)
			pOp->iCursorSize=(ECursorSizeOption)(atoi(pEl->GetText()));
		pEl=pRootEl->FirstChildElement("KeybTrans");
		if(pEl)
			pOp->fKeybTrans=(float)(atof(pEl->GetText()));
		pEl=pRootEl->FirstChildElement("Autoload");
		if(pEl)
			pOp->eAutoload=(EBoolOption)(atoi(pEl->GetText()));
		
		char sBuffer[256];

		for(int i=0;i<4;++i)
		{
			sprintf(sBuffer,"Poke%i",i);
			pEl=pRootEl->FirstChildElement(sBuffer);
			if(pEl)
			{
				strcpy(Pokes[i].sPoke,pEl->GetText());
				Pokes[i].ePokeActive=(EBoolOption)(atoi(pEl->Attribute("Active")));
				ConvertPoke(Pokes[i].sPoke,Pokes[i].Dir,Pokes[i].Data);
			}
			
		}
		
		
		
		
		for(int i=0;i<UK_NUM; ++i)
		{
			sprintf(sBuffer,"Key%i",i);
			pEl=pRootEl->FirstChildElement(sBuffer);
			//index is the enum order
			Controls[i]=*CKeyboard::GetKeyDefFromName(eSpectrumType,pEl->GetText());
			pEl=pEl->NextSiblingElement();
		}
	}
	
	//Sync emu options with GUI
	SetGUIOptions();

	return bResult;

}

//Refreshes GUI options (COption) to fit with pEmuOptions
void TEmuOptions::SetGUIOptions()
{
	CGUI* pOps=CPsPectrum::GetApp()->GetGUI();

	pOps->SetOptionValue("Sound",eSound);
	pOps->SetOptionValue("Spectrum Type",eSpectrumType);
	pOps->SetOptionValue("Autoload Tape",eAutoload);
	pOps->SetOptionValue("Fast Config",eJoyType);
	pOps->SetOptionValue("Speed",ePSPSpeed);

	//Set All keys
	for(int i=0;i<UK_NUM; ++i)
		pOps->SetOptionValue(psButtonOptions[i],GetKeyDefineFromName(Controls[i].m_Name));
	
	pOps->SetOptionValue("Screen Size",eScreenSize);
	pOps->SetOptionValue("Keyb Transparency",(int)(fKeybTrans*TEmuOptions::KT_NUM-1));
	pOps->SetOptionValue("Pointer Size",iCursorSize-1);
	
	char Buffer[256];
	for(int i=0;i<4;++i)
	{
		sprintf(Buffer,"Poke %i:",i+1);
		pOps->SetTextValue(Buffer,Pokes[i].sPoke);
		sprintf(Buffer,"Poke %i Active:",i+1);
		pOps->SetOptionValue(Buffer,Pokes[i].ePokeActive);
	}
}

//Given a Key Name you get the EKeyb48Option
TEmuOptions::EKeyb48Option TEmuOptions::GetKeyDefineFromName(const char* sKeyName)
{

	bool bKeyFound=false;
	for(int i=0;i<K48_NUM && !bKeyFound;++i) 
	{
		if(!strcmp(psKeys48Options[i],sKeyName))
			return (EKeyb48Option)i;
	}
	
	return (EKeyb48Option)-1;
}

//Refreshes all options getting things from the different systems
void TEmuOptions::Refresh()
{
	//Refresh Machine Type
	switch(machine_current->machine)
	{
		case LIBSPECTRUM_MACHINE_48:
		case LIBSPECTRUM_MACHINE_16:
			eSpectrumType=MT_48K;
			break;
		case LIBSPECTRUM_MACHINE_128:
		case LIBSPECTRUM_MACHINE_PLUS2:
		case LIBSPECTRUM_MACHINE_PLUS2A:
			eSpectrumType=MT_128K;
			break;
	}
}
