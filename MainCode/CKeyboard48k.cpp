#include "CKeyboard48k.h"
#include "CMGELog.h"


bool CKeyboard48k::Init(int iXPos, int iYPos)
{
    m_bOK=CKeyboard::Init(iXPos,iYPos,"media/keyboard48_350x170.tga");

	if(m_bOK)
    {
		m_iNumKeys=MAX_KEYS_48K;

        //Init KeyMapping
        m_pKeyMapping=new TKeyBox[m_iNumKeys];
        //1
        m_pKeyMapping[0].m_Box=CBox(5,18,25,21);
		//2
        m_pKeyMapping[1].m_Box=CBox(38,18,25,21);
		//3
        m_pKeyMapping[2].m_Box=CBox(71,18,25,21);
		//4
        m_pKeyMapping[3].m_Box=CBox(104,18,25,21);
		//5
        m_pKeyMapping[4].m_Box=CBox(137,18,25,21);
		//6
        m_pKeyMapping[5].m_Box=CBox(170,18,25,21);
		//7
        m_pKeyMapping[6].m_Box=CBox(203,18,25,21);
		//8
        m_pKeyMapping[7].m_Box=CBox(236,18,25,21);
		//9
        m_pKeyMapping[8].m_Box=CBox(269,18,25,21);
		//0
        m_pKeyMapping[9].m_Box=CBox(302,18,25,21);
		//q
        m_pKeyMapping[10].m_Box=CBox(19,57,25,21);
		//w
        m_pKeyMapping[11].m_Box=CBox(53,57,25,21);
		//e
        m_pKeyMapping[12].m_Box=CBox(87,57,25,21);
		//r
        m_pKeyMapping[13].m_Box=CBox(121,57,25,21);
		//t
        m_pKeyMapping[14].m_Box=CBox(155,57,25,21);
		//y
        m_pKeyMapping[15].m_Box=CBox(188,57,25,21);
		//u
        m_pKeyMapping[16].m_Box=CBox(221,57,25,21);
		//i
        m_pKeyMapping[17].m_Box=CBox(253,57,25,21);
		//o
        m_pKeyMapping[18].m_Box=CBox(285,57,25,21);
		//p
        m_pKeyMapping[19].m_Box=CBox(316,57,25,21);
		//a
        m_pKeyMapping[20].m_Box=CBox(28,99,25,21);
		//s
        m_pKeyMapping[21].m_Box=CBox(62,99,25,21);
		//d
        m_pKeyMapping[22].m_Box=CBox(95,99,25,21);
		//f
        m_pKeyMapping[23].m_Box=CBox(129,99,25,21);
		//g
        m_pKeyMapping[24].m_Box=CBox(161,99,25,21);
		//h
        m_pKeyMapping[25].m_Box=CBox(196,99,25,21);
		//j
        m_pKeyMapping[26].m_Box=CBox(230,99,25,21);
		//k
        m_pKeyMapping[27].m_Box=CBox(262,99,25,21);
		//l
        m_pKeyMapping[28].m_Box=CBox(293,99,25,21);
		//ENTER
        m_pKeyMapping[29].m_Box=CBox(324,99,25,21);
		//CAPS SHIFT
		m_pKeyMapping[30].m_Box=CBox(4,139,31,21);
		//z
        m_pKeyMapping[31].m_Box=CBox(45,139,25,21);
		//x
        m_pKeyMapping[32].m_Box=CBox(78,139,25,21);
		//c
        m_pKeyMapping[33].m_Box=CBox(112,139,25,21);
		//v
        m_pKeyMapping[34].m_Box=CBox(145,139,25,21);
		//b
        m_pKeyMapping[35].m_Box=CBox(178,139,25,21);
		//n
        m_pKeyMapping[36].m_Box=CBox(211,139,25,21);
		//m
        m_pKeyMapping[37].m_Box=CBox(245,139,25,21);
		//SIMBOL SHIFT
        m_pKeyMapping[38].m_Box=CBox(278,139,25,21);
		//SPACE KEY
		m_pKeyMapping[39].m_Box=CBox(306,139,44,21);

		CMGELog::SysLog(CMGELog::lError,"Keyboard 48 Inited\n");
    }
	else
		CMGELog::SysLog(CMGELog::lError,"Keyboard 48 Failed to Init\n");
    
		return m_bOK;
}

void CKeyboard48k::End()
{
    if(m_bOK)
    {
        CMGELog::SysLog(CMGELog::lError,"Releasing 48k keyb\n");
		delete [] m_pKeyMapping;       
		CKeyboard::End();
    }
}

