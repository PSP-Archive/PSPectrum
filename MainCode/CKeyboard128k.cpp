#include "CKeyboard128k.h"
#include "CMGELog.h"

#define m_pMachine 0

//iXPos and iYPos are screen coordinates
bool  CKeyboard128k::Init(int iXPos, int iYPos)
{
	m_bOK=CKeyboard::Init(iXPos,iYPos,"media/keyboard128_350x170.tga");
	
	
    if(m_bOK)
    {
		//ESTO ES NECESARIO HACERLO SINO LAS BUSQUEDAS EN EL VECTOR NO FUNCIONARAN
        m_iNumKeys=MAX_KEYS_128K;
        
        //Init KeyMapping
        m_pKeyMapping=new TKeyBox[m_iNumKeys];
        //1
        m_pKeyMapping[0].m_Box=CBox(55,3,17,24);
        //2
        m_pKeyMapping[1].m_Box=CBox(80,3,17,24);
        //3
        m_pKeyMapping[2].m_Box=CBox(106,3,17,24);
        //4
        m_pKeyMapping[3].m_Box=CBox(133,3,17,24);
        //5
        m_pKeyMapping[4].m_Box=CBox(158,3,17,24);
        //6
        m_pKeyMapping[5].m_Box=CBox(185,3,17,24);
		//7
        m_pKeyMapping[6].m_Box=CBox(211,3,17,24);
		//8
        m_pKeyMapping[7].m_Box=CBox(238,3,17,24);
        //9
        m_pKeyMapping[8].m_Box=CBox(263,3,17,24);
        //0
        m_pKeyMapping[9].m_Box=CBox(290,3,17,24);
        //q
        m_pKeyMapping[10].m_Box=CBox(67,38,17,24);
		//w
        m_pKeyMapping[11].m_Box=CBox(94,38,17,24);
		//e
        m_pKeyMapping[12].m_Box=CBox(120,38,17,24);
		//r
        m_pKeyMapping[13].m_Box=CBox(146,38,17,24);
		//t
        m_pKeyMapping[14].m_Box=CBox(172,38,17,24);
		//y
        m_pKeyMapping[15].m_Box=CBox(198,38,17,24);
		//u
        m_pKeyMapping[16].m_Box=CBox(224,38,17,24);
		//i
        m_pKeyMapping[17].m_Box=CBox(250,38,17,24);
		//o
        m_pKeyMapping[18].m_Box=CBox(276,38,17,24);
		//p
        m_pKeyMapping[19].m_Box=CBox(303,38,17,24);            
		//a
        m_pKeyMapping[20].m_Box=CBox(74,73,17,24);            
		//s
        m_pKeyMapping[21].m_Box=CBox(100,73,17,24);            
		//d
        m_pKeyMapping[22].m_Box=CBox(126,73,17,24);            
		//f
        m_pKeyMapping[23].m_Box=CBox(152,73,17,24);            
		//g
        m_pKeyMapping[24].m_Box=CBox(178,73,17,24);            
		//h
        m_pKeyMapping[25].m_Box=CBox(205,73,17,24);
		//j
        m_pKeyMapping[26].m_Box=CBox(231,73,17,24);
		//k
        m_pKeyMapping[27].m_Box=CBox(257,73,17,24);
		//l
        m_pKeyMapping[28].m_Box=CBox(283,73,17,24);
		//ENTER (son 2 cajas)
        m_pKeyMapping[29].m_Box=CBox(309,73,39,25); 
        m_pKeyMapping[29].m_pNext=new TKeyBox;
		m_pKeyMapping[29].m_pNext->m_Box=CBox(329,37,20,60); 
        //CAPS SHIFT
        m_pKeyMapping[30].m_Box=CBox(2,108,49,24);
		//z
        m_pKeyMapping[31].m_Box=CBox(86,108,17,24);
		//x
        m_pKeyMapping[32].m_Box=CBox(113,108,17,24);
		//c
        m_pKeyMapping[33].m_Box=CBox(139,108,17,24);
		//v
        m_pKeyMapping[34].m_Box=CBox(165,108,17,24);
		//b
        m_pKeyMapping[35].m_Box=CBox(192,108,17,24);
		//n
        m_pKeyMapping[36].m_Box=CBox(218,108,17,24);
		//m
        m_pKeyMapping[37].m_Box=CBox(244,108,17,24);
		//SIMBOL SHIFT IZQUIERDO
        m_pKeyMapping[38].m_Box=CBox(2,141,16,24);
		//SPACE KEY
        m_pKeyMapping[39].m_Box=CBox(132,141,109,24);


		// EXTRA 128 KEYS ******************************************	

        //TRUE VIDEO (CAPS + 3)
        m_pKeyMapping[40].m_Box=CBox(2,3,17,24);
        //INV VIDEO  (CAPS + 4)
        m_pKeyMapping[41].m_Box=CBox(28,3,17,24);
        //BREAK      (CAPS + 3) --
        m_pKeyMapping[42].m_Box=CBox(316,3,30,24);
        //DELETE    (CAPS + 0) 
        m_pKeyMapping[43].m_Box=CBox(2,38,30,24);
        //GRAPH (CAPS + 9)
        m_pKeyMapping[44].m_Box=CBox(41,38,17,24);
        //EXTEND MODE   (CAPS + 3) --
        m_pKeyMapping[45].m_Box=CBox(2,73,36,24);
        //EDIT  (CAPS + 1)
        m_pKeyMapping[46].m_Box=CBox(48,73,17,24);
        //CAPS LOCK  (CAPS + 2) 
        m_pKeyMapping[47].m_Box=CBox(61,107,17,24);
        //CAPS SHIFT DERECHO 
        m_pKeyMapping[48].m_Box=CBox(296,107,49,24);
        //;
        m_pKeyMapping[49].m_Box=CBox(28,141,17,24);
        //"
        m_pKeyMapping[50].m_Box=CBox(54,141,17,24);
        //CURSOR IZQUIERDA (CAPS + 5)
        m_pKeyMapping[51].m_Box=CBox(80,141,17,24);
        //CURSOR DERECHA (CAPS + 8)
        m_pKeyMapping[52].m_Box=CBox(106,141,17,24);
        //CURSOR ARRIBA (CAPS + 7)
        m_pKeyMapping[53].m_Box=CBox(250,141,17,24);
        //CURSOR ABAJO (CAPS + 6)
        m_pKeyMapping[54].m_Box=CBox(277,141,17,24);
        //,
        m_pKeyMapping[55].m_Box=CBox(303,141,17,24);
        //SYMBOL SHIFT DERECHO
        m_pKeyMapping[56].m_Box=CBox(329,141,17,24);
        
		CMGELog::SysLog(CMGELog::lError,"Keyboard 128 Inited\n");
    }
	else
		CMGELog::SysLog(CMGELog::lError,"Keyboard 128 Failed to Init\n");

    return m_bOK;
}



void CKeyboard128k::End()
{
    if(m_bOK)
    {
        if(m_pKeyMapping)
			 delete [] m_pKeyMapping;

		CMGELog::SysLog(CMGELog::lError,"Releasing 128k keyb\n");
        CKeyboard::End();
    }
}

/*
//Ask every Box if coordinates are in
//return index of key info in vector. -1 if no key pressed
void CKeyboard128k::CheckKeyPress(bool bFirstPress)
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
				//Mandar la pulsacion de la tecla
				keyboard_press(m_KeyDefs[i].m_Key1);
				keyboard_press(m_KeyDefs[i].m_Key2);

				m_iLastKeyPressedIndex=i;
				bExit=true;

	           
			}                
			else
				++i;
		
			pKeyBox=pKeyBox->m_pNext;
		}
	}
}

void CKeyboard128k::CheckKeyRelease()
{
    //We processed a key release
    if(m_iLastKeyPressedIndex!=-1)
    {
		keyboard_release(m_KeyDefs[m_iLastKeyPressedIndex].m_Key1);       
		keyboard_release(m_KeyDefs[m_iLastKeyPressedIndex].m_Key2);
	}
}

*/