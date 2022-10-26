#include "types.h"
#include "CPsPectrum.h"
#include "CMGERenderDevice.h"
#include "display.h"
#include "fuse/ui/uidisplay.h"

#ifdef __cplusplus
	extern "C" {
#endif

/* The colour palette */
u16 rgb_colors[16];
static CMGERenderDevice* pRenderer=0;
static CMGEScreen*	pEmuScr; //Place where we render emu Scr
static libspectrum_byte force_full_refresh = 1;

int uidisplay_init( int width, int height )
{
	pRenderer=CPsPectrum::GetApp()->GetRenderDevice();
	pEmuScr=CPsPectrum::GetApp()->GetEmuScr();

    /* Normal colors */
	rgb_colors[0]=pRenderer->GetColor(0x00,0x00,0x00,0xFF);
	rgb_colors[1]=pRenderer->GetColor(0x00,0x00,0xCF,0xFF);
	rgb_colors[2]=pRenderer->GetColor(0xCF,0x00,0x00,0xFF);
	rgb_colors[3]=pRenderer->GetColor(0xCF,0x00,0xCF,0xFF);
	rgb_colors[4]=pRenderer->GetColor(0x00,0xCF,0x00,0xFF);
	rgb_colors[5]=pRenderer->GetColor(0x00,0xCF,0xCF,0xFF);
	rgb_colors[6]=pRenderer->GetColor(0xCF,0xCF,0x00,0xFF);
	rgb_colors[7]=pRenderer->GetColor(0xCF,0xCF,0xCF,0xFF);
	/* Bring Colors */
	rgb_colors[8]=pRenderer->GetColor(0x00,0x00,0x00,0xFF);
	rgb_colors[9]=pRenderer->GetColor(0x00,0x00,0xFF,0xFF);
	rgb_colors[10]=pRenderer->GetColor(0xFF,0x00,0x00,0xFF);
	rgb_colors[11]=pRenderer->GetColor(0xFF,0x00,0xFF,0xFF);
	rgb_colors[12]=pRenderer->GetColor(0x00,0xFF,0x00,0xFF);
	rgb_colors[13]=pRenderer->GetColor(0x00,0xFF,0xFF,0xFF);
	rgb_colors[14]=pRenderer->GetColor(0xFF,0xFF,0x00,0xFF);
	rgb_colors[15]=pRenderer->GetColor(0xFF,0xFF,0xFF,0xFF);


	return 0;
}

void uidisplay_area( int x, int y, int w, int h )
{
	int disp_x,disp_y;
	WORD pix_colour;

	CMGEScreen* pEmu=CPsPectrum::GetApp()->GetEmuScr();
	//CMGELog::SysLog(CMGELog::lError,"Area");
	for(disp_x=x;disp_x < x+w;disp_x++)
	{
		for(disp_y=y;disp_y < y+h;disp_y++)
		{
		  pix_colour = rgb_colors[display_image[disp_y][disp_x]];
		  pEmu/*Scr*/->PutPixel(disp_x, disp_y, pix_colour);
		}
	}
	//CMGELog::SysLog(CMGELog::lError,"--");
	return;   
}

void uidisplay_frame_end( void )
{

}

void uidisplay_hotswap_gfx_mode( void )
{

}

int uidisplay_end(void)
{
	return 0;
}

int ui_print(int x,int y, const char* text)
{
	CPsPectrum::GetApp()->PrintText(pRenderer->GetWorkingScreen(),text,0,250);
	return 0;
}

#ifdef __cplusplus
	}
#endif
