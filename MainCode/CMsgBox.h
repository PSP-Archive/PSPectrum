#if !defined(__CMsgBox__)
#define __CMsgBox__

class CMsgBox
{
public:
	enum EMsgBoxType
	{
		MBOX_NORMAL,
		MBOX_YES_NO
	};
	
	CMsgBox(): m_iNumLines(0) {}
	void AddLine(const char* sLine);
	void Reset();
	bool Run(EMsgBoxType eType=MBOX_NORMAL);
	bool ShowMsg(const char* sLine,EMsgBoxType eMsgBox=MBOX_NORMAL);
private:
	char m_Lines[256][256];
	int m_iNumLines;
	int m_iMaxWidth;
	
};

#endif
