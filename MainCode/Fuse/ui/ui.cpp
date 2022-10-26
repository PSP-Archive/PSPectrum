#include <libspectrum.h>
#include <CMGELog.h>
#include <CMGESoundGenerator.h>
#include <..\CPsPectrum.h>

#ifdef __cplusplus
	extern "C" {
#endif



int ui_init(int *argc, char ***argv){ return 0;}
int ui_end(void){return 0;}

int ui_error( int severity, const char *format, ... )
{
	
	//local vars
	va_list lArg;		
	char sBuffer[LOG_BUFFER_SIZE]={0};     	

	//format for variable formating arguments
	va_start(lArg, format);	
	vsnprintf(sBuffer, LOG_BUFFER_SIZE, format, lArg);
	va_end(lArg);

	
	CMGELog::SysLog(CMGELog::lError,sBuffer);
	return 0;
}



int sound_lowlevel_init( const char *device, int *freqptr, int *stereoptr )
{
	return 0;
}
void sound_lowlevel_end( void )
{

}
void sound_lowlevel_frame( libspectrum_signed_word *data, int len )
{
	CMGESoundGenerator* pSoundGen=CPsPectrum::GetApp()->GetSoundGen();
	for(int i=0;i<len;++i)
		pSoundGen->Write(data[i]);
}
#ifdef __cplusplus
	}
#endif
