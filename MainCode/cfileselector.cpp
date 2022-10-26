#include "CFileSelector.h"
#include "CPsPectrum.h"
#include "CMGELog.h"
#include "CMGETgaFile.h"
#include "string.h"
#include "stdlib.h"
#include "CMGEScreen.h"
#include "CMsgBox.h"

#include "zlib/zlib.h"

#include "fuse/utils.h"
#include "fuse.h"

#ifdef _WINDOWS
	#include <io.h>
	#include <direct.h>
#else
	#define MAX_PATH 264
#endif

#define kDELAY 0.2f

bool CFileSelector::Init(const char* psPath)
{
    End();
    
	
	m_pBG=new CMGETgaFile;
	m_bOk=m_pBG->Init("media/FileSelectorBG.tga");
	if(m_bOk)
	{	
		m_pPreview=new CMGETgaFile;
		m_bHasShot=false;
		m_bShowShot=true;
		m_iNumFiles=0;
		m_iCurrentFile=0;
		m_iCurrentCursorPos=0;
		m_iCurrentFirstFile=0;
		m_fAccumTime=0;
		m_lLastTime=0;
		m_sLastLoadedFile=0;
		m_pFiles=new TFileDesc[MAX_NUM_FILES];
		
		
		
		//Absolute linux path TODO:FIX FOR WIN
		if(psPath[0]=='/')
			strcpy(m_sPath,psPath);
		else
		{		
			strcpy(m_sPath,fuse_directory);
			strcat(m_sPath,psPath);
		}
		
		for(unsigned int i=0;i<strlen(m_sPath);++i)
			if(m_sPath[i]=='\\')
				m_sPath[i]='/';

		if(m_sPath[strlen(m_sPath)-1]!='/')
			strcat(m_sPath,"/");
		
		GetFiles(m_sPath);
		

		m_bOk=true;
	}

	
	if(m_bOk==false)
		CMGELog::SysLog(CMGELog::lError,"File Selector Initialization failed\n");
	else
		CMGELog::SysLog(CMGELog::lError,"File Selector Inited OK\n");
    
	return m_bOk;
}

void CFileSelector::End()
{
    if(m_bOk)
    {
        if(m_pFiles)
            delete [] m_pFiles;

		SafeDelete(m_pBG);
		SafeDelete(m_pPreview);
    }
    m_bOk=false;
    
    CMGELog::SysLog(CMGELog::lError,"File Selector Ended\n");
}


bool CFileSelector::CheckValidFile(TFileDesc& FileInfo)
{
	#define NUM_FORMATS 8
	static char* sSupportedFiles[NUM_FORMATS]={"SNA","SLT","Z80","TZX","TAP","SP","SNP","ZIP"};
	char Buffer[256];

	int i=0;

	//Estos se añaden siempre.
	if(FileInfo.iType==E_FT_DIR && strcmp(FileInfo.FileName,"."))
		return true;
	
	CPsPectrum::GetExtensionFromPath(FileInfo.FileName,Buffer);
	while(i<NUM_FORMATS)
	{
		if(!strcmp(sSupportedFiles[i],Buffer))
			return true;
		++i;
	}
	return false;
}

void CFileSelector::GetFileInfo(
			#if defined(_PSP)
				 SceIoDirent File
			#else
				 _finddata64i32_t FileData
			#endif
				 ,TFileDesc& FileSpec)
{
	
#if defined(_PSP)
	strcpy(FileSpec.FileName,File.d_name);
	strupr(FileSpec.FileName);

	if(File.d_stat.st_attr &   FIO_SO_IFDIR )
		FileSpec.iType=E_FT_DIR;
	else
		FileSpec.iType=E_FT_FILE;

#else

	strcpy(FileSpec.FileName,FileData.name);
	strupr(FileSpec.FileName);

	if(FileData.attrib&_A_SUBDIR)
		FileSpec.iType=E_FT_DIR;
	else
		FileSpec.iType=E_FT_FILE;

#endif

}


void CFileSelector::GetFiles(const char* pszInitialPath)
{
	
	m_iNumFiles=0;

#if defined(_PSP)
	SceIoDirent  File;
        
    int dfd;
    dfd = sceIoDopen(pszInitialPath);
    //memset(&m_Files[0],0,sizeof(TFileDesc)*MAX_NUM_FILES);
    if(dfd >= 0)
    { 
        memset(&File,0,sizeof(SceIoDirent));
        while(sceIoDread( dfd,&File)>=1)
        {    
            //Usamos el siguiente slot libre como temporal hasta encontrar unfichero ok
			GetFileInfo(File,m_pFiles[m_iNumFiles]);
			bool bValid=CheckValidFile(m_pFiles[m_iNumFiles]);
			if(bValid)
				++m_iNumFiles;
					
            memset(&File,0,sizeof(SceIoDirent));
        }

        sceIoDclose(dfd);
   
	} 
    else
        printf("Could not open directory\n");
#else 
	struct _finddata_t FileData;
	intptr_t iHandle;
	
	char sBuffer[256]={0};
	strcpy(sBuffer,pszInitialPath);
	strcat(sBuffer,"/*.*");
	if((iHandle=_findfirst(sBuffer,&FileData))!=-1)
	{
		GetFileInfo(FileData,m_pFiles[m_iNumFiles]);
		bool bValid=CheckValidFile(m_pFiles[m_iNumFiles]);
		if(bValid)
			++m_iNumFiles;
					
		
		while(_findnext(iHandle,&FileData)!=-1)
		{
			GetFileInfo(FileData,m_pFiles[m_iNumFiles]);
			bool bValid=CheckValidFile(m_pFiles[m_iNumFiles]);
			if(bValid)
				++m_iNumFiles;
		}
	}

#endif
	
	m_iCurrentFile=0;
	m_iCurrentCursorPos=0;
	m_iCurrentFirstFile=0;

	qsort(m_pFiles,m_iNumFiles,sizeof(TFileDesc),CFileSelector::Compare);
	FillFileOffsets();

}

void CFileSelector::Render(CMGEScreen* pScr)
{
	m_pBG->ScreenCopy(pScr,0,0);
	int iLastFile=m_iCurrentFirstFile+MAX_NUM_SCREEN_FILES;
    char Buffer[264];
    if(iLastFile>m_iNumFiles)
        iLastFile=m_iNumFiles;

    for(int i=m_iCurrentFirstFile,  j=0; i<iLastFile; ++i,++j)
    {
        if(m_pFiles[i].iType==E_FT_FILE)
			CPsPectrum::GetApp()->PrintText(pScr,m_pFiles[i].FileName,40,32+j*13);
        else
        {
            sprintf(Buffer,"<%s>",m_pFiles[i].FileName);
            CPsPectrum::GetApp()->CPsPectrum::PrintText(pScr,Buffer,40,32+j*13);
        }
    }

    //We need to reload shot
	if(m_bShotDirty==true)
	{
		m_bHasShot=GetShot("shots");
		m_bShotDirty=false;
	}
	
	//Render Snapshot with no border
	if(m_bHasShot && m_bShowShot)
		m_pPreview->ScreenCopy(32,24,255+32,191+24,pScr,190,40);

	CPsPectrum::GetApp()->PrintText(pScr,">",32,32+m_iCurrentCursorPos*13);
	CPsPectrum::GetApp()->PrintText( pScr,m_sPath,0,0);
	CPsPectrum::GetApp()->PrintText( pScr,"O Help",400,255);
    sprintf(Buffer,"%i Files",m_iNumFiles);
	CPsPectrum::GetApp()->PrintText( pScr,Buffer,0,255);
	if(m_bShowShot)
		CPsPectrum::GetApp()->PrintText( pScr,"ScreenShot:On",14*8,255);
	else
		CPsPectrum::GetApp()->PrintText( pScr,"ScreenShot:Off",14*8,255);
	
}

//Returns if exit was pressed
CFileSelector::EAction CFileSelector::Run()
{

	CMGEControl* pControl=CMGEApp::GetApp()->GetControl();
	
	long lMicroSecs=CPsPectrum::GetTicks();
	m_fAccumTime+=(lMicroSecs-m_lLastTime)/ CPsPectrum::GetTimeDivisor();
    m_lLastTime=lMicroSecs;
	
	//Show image? if we´re moving fast no
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_TRIANGLE].bePushed	)
		m_fAccumTime=kDELAY;
		

	if(!pControl->m_cuResponse.buttons[CPsPectrum::BT_UP].bePushed &&
		!pControl->m_cuResponse.buttons[CPsPectrum::BT_DOWN].bePushed)
	{	
		m_fAccumTime=kDELAY;
	}
	
	
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_RIGHT].wasPressed)
	{
		int iNextOffset=GetNextFileOffset(m_pFiles[m_iCurrentFile].FileName[0]);
		if(iNextOffset!=-1)
		{	
			m_iCurrentFile=iNextOffset;
			m_iCurrentCursorPos=0;
			m_iCurrentFirstFile=iNextOffset;
		}
	}
	else if(pControl->m_cuResponse.buttons[CPsPectrum::BT_LEFT].wasPressed)
	{
		int iPrevOffset=GetPrevFileOffset(m_pFiles[m_iCurrentFile].FileName[0]);
		if(iPrevOffset!=-1)
		{
			m_iCurrentFile=iPrevOffset;
			m_iCurrentCursorPos=0;
			m_iCurrentFirstFile=iPrevOffset;
		}
	}
	
	if(pControl->m_cuResponse.up)
    {    
        //Si paso el tiempo necesario mover
        if(m_fAccumTime>=kDELAY)
        {
            --m_iCurrentCursorPos;
            if (m_iCurrentCursorPos<0)
            {
                m_iCurrentCursorPos=0;
                //Si no estabamos sobre el primer fichero
                if(m_iCurrentFile)
                    --m_iCurrentFirstFile;
            }
           
            if(m_iCurrentFile)
                --m_iCurrentFile;

            
            m_fAccumTime=0;
        }   
    
		
		ShotDirty(true);
	}
    
	if(pControl->m_cuResponse.down)
    {    
        //Si paso el tiempo necesario mover
        if(m_fAccumTime>=kDELAY)
        {
    
            ++m_iCurrentCursorPos;
            
            //Si ya estamos mostrando toda la parte del final
            if(m_iCurrentFirstFile+MAX_NUM_SCREEN_FILES >=m_iNumFiles)
            {
                
                
                //No aumentamos el first file donde esta esta ok
                //Ademas tenemos que ver si el cursorpos paasa de los ficheros
                //que mostramos que no tiene porque ser MAX_NUM_SCREEN_FILES
                if(m_iCurrentCursorPos>=m_iNumFiles-m_iCurrentFirstFile)
                {   
                    //Reposicionamos el cursor
                    --m_iCurrentCursorPos;
                }
                else //No estamos en el ultimo fichero
                   ++m_iCurrentFile; 
            }
            else
            {
                //Si no estamos mostrando todos los ficheros no estamos apuntand
                //al ultimo en  iCurrentFIle
                ++m_iCurrentFile;
            
                //Reposicionamos el cursor
                if(m_iCurrentCursorPos>=MAX_NUM_SCREEN_FILES)
                {
                    --m_iCurrentCursorPos;
                    ++m_iCurrentFirstFile;
                }
            }
            m_fAccumTime=0;

        }
        
		ShotDirty(true);
    }
    
    
    //Selection
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_CROSS].wasPressed)
	{    
		if(!strcmp(m_pFiles[m_iCurrentFile].FileName,".."))
			GoDirUp();
		else if(m_pFiles[m_iCurrentFile].iType==E_FT_DIR)
			GoDirDown(m_pFiles[m_iCurrentFile].FileName);
		else
		{
			m_sLastLoadedFile=(const char*)m_pFiles[m_iCurrentFile].FileName;
			return AT_SELECTED;
		}
	}
	
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_SQUARE].wasPressed)
	{
		char Buffer[512]; 
		CMsgBox* pMsgBox=CPsPectrum::GetApp()->GetMsgBox();
		
		sprintf(Buffer,"Delete File %s?",m_pFiles[m_iCurrentFile].FileName);
		if(pMsgBox->ShowMsg(Buffer,CMsgBox::MBOX_YES_NO)==true)
		{
			strcpy(Buffer,m_sPath);
			strcat(Buffer,m_pFiles[m_iCurrentFile].FileName);
			if(!strcmp(m_pFiles[m_iCurrentFile].FileName,".."))
			{
				sprintf(Buffer,"Directory .. Can´t be deleted",m_pFiles[m_iCurrentFile].FileName);
				pMsgBox->ShowMsg(Buffer);
			}
			else
			{
				bool bErased=false;
				#ifdef _WINDOWS
					if(DeleteFile(Buffer))
						bErased=true;
				#elif _PSP
					if(sceIoRemove(Buffer)>=0)
						bErased=true;
				#endif

				if(bErased)
				{
					sprintf(Buffer,"File %s was deleted",m_pFiles[m_iCurrentFile].FileName);
					pMsgBox->ShowMsg(Buffer);
					//Fill the gap of the deleted file
					if(m_iCurrentFile==(m_iNumFiles-1)) //Last One
					{	
						--m_iCurrentCursorPos;
						--m_iCurrentFile;
					}
					else //First one?
						memcpy(&m_pFiles[m_iCurrentFile],&m_pFiles[m_iCurrentFile+1],sizeof(TFileDesc)*(m_iNumFiles-m_iCurrentFile-1));

					--m_iNumFiles;
				}
			}
		}
	}
	
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_RTRIGGER].wasPressed)
	{
		m_bShowShot=!m_bShowShot;
		m_bShotDirty=true;
	}
	
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_CIRCLE].wasPressed)
	{	
		CMsgBox* pMsgBox=CPsPectrum::GetApp()->GetMsgBox();
		pMsgBox->Reset();
		pMsgBox->AddLine("DPad Up/Down -> Select File");
		pMsgBox->AddLine("DPad Left/Right -> Jump To Letter");
		pMsgBox->AddLine("Cross -> Select File");
		pMsgBox->AddLine("Triangle -> Fast Scroll");
		pMsgBox->AddLine("Square -> Delete File");
		pMsgBox->AddLine("R Trigger -> Show/Hide Shot");
		pMsgBox->Run();
	}

	//Exit
	if(pControl->m_cuResponse.buttons[CPsPectrum::BT_SELECT].wasPressed)
        return AT_EXIT;
    
    return AT_NONE;
}

int CFileSelector::Compare(const void *elem1, const void *elem2 )
{
	//First Put Files, then directories
	TFileDesc* pElem1=(TFileDesc*) elem1;
	TFileDesc* pElem2=(TFileDesc*) elem2;
	
	//ordering -> first between dirs and files, then sort every type alphabetically
    //strcmp returns [-1..1]
    return (pElem1->iType-pElem1->iType)*2 + strcmp(pElem1->FileName,pElem2->FileName);   
    
}

const char*   CFileSelector::GetLastLoadedFile()
{ 
	return m_sLastLoadedFile;
}

bool CFileSelector::GetShot(const char* sPath)
{
	bool bResult=true;
	const char* sFile=m_pFiles[m_iCurrentFile].FileName;
	char sBuffer[256];

	strcpy(sBuffer,sPath);
	strcat(sBuffer,"/");
	strcat(sBuffer,sFile);
	
	gzFile f = NULL;

	if ((f=gzopen(sBuffer,"rb"))==NULL)
		bResult=false;
	else
	{
		gzclose(f);
		m_pPreview->Init(sBuffer);
	}

	return bResult;
}

//Run Through all the table and get where each letter/number/simbol starts in array
//Must be used on the sorted array
void CFileSelector::FillFileOffsets()
{
	//Init file offsets
	for(int i=0;i<256;++i)
		m_FileOffsets[i]=-1;

	
	//I hope no file estars with char 0x0
	unsigned char cCurrent=0x0;
	for(int i=0;i<m_iNumFiles;++i)
	{
		if(cCurrent!=m_pFiles[i].FileName[0])
		{
			cCurrent=m_pFiles[i].FileName[0];
			m_FileOffsets[cCurrent]=i;
		}
	}
}

int CFileSelector::GetNextFileOffset(unsigned char cChar)
{
	bool bFound=false;
	
	//This is last char, no more offsets
	if(cChar==0xFF)
		return -1;
	
	int iStart=cChar+1;
		
	while(!bFound)
	{
		if(m_FileOffsets[iStart]!=-1)
			bFound=true;
		else
		{
			if(iStart==0xFF)
				return -1;
			else
				++iStart;
		}
	}

	return m_FileOffsets[iStart];
}

int CFileSelector::GetPrevFileOffset(unsigned char cChar)
{
	bool bFound=false;

	//This is last char, no more offsets
	if(cChar==0)
		return -1;

	int iStart=cChar-1;

	while(!bFound)
	{
		if(m_FileOffsets[iStart]!=-1)
			bFound=true;
		else
		{
			if(iStart==0x0)
				return -1;
			else
				--iStart;
		}
	}

	return m_FileOffsets[iStart];
}


void CFileSelector::GoDirUp()
{
	char Buffer[MAX_PATH];
	CPsPectrum::GetPathUpDir(m_sPath,Buffer);
	strcpy(m_sPath,Buffer);
	GetFiles(m_sPath);
}

void CFileSelector::GoDirDown(const char* sDirName)
{
	char Buffer[MAX_PATH];
	CPsPectrum::GetPathDownDir(m_sPath,sDirName,Buffer);
	strcpy(m_sPath,Buffer);
	GetFiles(m_sPath);
}
