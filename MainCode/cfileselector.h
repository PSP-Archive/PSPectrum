#ifndef _FILESELECTOR_
#define _FILESELECTOR_

#include "types.h"

class CMGETgaFile;
class CMGEScreen;
struct _finddata64i32_t;
struct SceIoDirent;

class CFileSelector
{
public:
    #define MAX_NUM_FILES 10000
    #define MAX_NUM_SCREEN_FILES 16
        
	enum EAction   { AT_EXIT, AT_SELECTED, AT_NONE };
	enum EFileType { E_FT_FILE, E_FT_DIR };
    struct TFileDesc
    {
        EFileType iType;
        char FileName[264];
    };

    bool    Init(const char* sPath);
    void    End ();
    EAction	Run();
    void    Render(CMGEScreen*);
	void	ShotDirty(bool bShotDirty) {m_bShotDirty=bShotDirty;}

	
    const char*   GetCurrentPath() { return m_sPath;}
	const char*   GetLastLoadedFile();
	
	CFileSelector() : m_bOk(false) {}
	~CFileSelector() { End(); }

private:
    int GetNextFileOffset(unsigned char cChar);
	int GetPrevFileOffset(unsigned char cChar);
	void FillFileOffsets();
	bool CheckValidFile(TFileDesc& FileInfo);
	void GetFiles(const char* pszInitialPath);
    static int Compare(const void *elem1, const void *elem2 );
    bool GetShot(const char* sPath);
	void GoDirUp();
	void GoDirDown(const char* sDirName);
	void GetFileInfo(
					#if defined(_PSP)
						SceIoDirent File
					 #else
						_finddata64i32_t FileData
					 #endif
						,TFileDesc& FileSpec);
		
	TFileDesc *m_pFiles;
    int     m_iNumFiles;   
    int     m_iCurrentFile; //On the full vector
    int     m_iCurrentFirstFile; //Showing
    int     m_iCurrentCursorPos;
    char    m_sPath[264];
    CMGETgaFile*	 m_pBG;
	CMGETgaFile*	 m_pPreview;
	bool	m_bHasShot;
	bool	m_bShowShot;
	bool	m_bShotDirty; //We saved a new shot for the current selected game
	bool    m_bOk;
    long    m_lLastTime; //To have a better file sliding controls
    float   m_fAccumTime;
	const char* m_sLastLoadedFile;
	int		m_FileOffsets[256];//contains the index where each files with letter X, starts
};

#endif
