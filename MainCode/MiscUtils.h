#ifndef _MISC_UTILS_
#define _MISC_UTILS_

class CBox
{

public:
    CBox::CBox(){}
    CBox::CBox(int iTopLeftX,int iTopLeftY,int iWidth,int iHeight)
    {
        m_iTopLeftX=iTopLeftX;
        m_iTopLeftY=iTopLeftY;
        m_iBottomRightX=iTopLeftX+iWidth;
        m_iBottomRightY=iTopLeftY+iHeight;
    }
    
    //x and y must be relative to keyboard posiition
    bool  IsPointIn(int x, int y)
    {
        if( (x>=m_iTopLeftX && x<=m_iBottomRightX) && \
            (y>=m_iTopLeftY && y<=m_iBottomRightY))
            return  true;
        else
            return false;
    }
    
    int m_iTopLeftX,m_iTopLeftY;
    int m_iBottomRightX, m_iBottomRightY;
    
    

};

#endif
