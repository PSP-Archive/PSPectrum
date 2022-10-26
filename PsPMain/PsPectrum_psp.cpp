//////////////////////////////////////////////////////////////////////////
// PSPectrum by HexDump^NewOlds
//
// This file its the entry point of the application
//////////////////////////////////////////////////////////////////////////

//include our app
#include "../game/CPsPectrum.h"
#include "CMGELog.h"

#ifdef __cplusplus
extern "C" {
#endif 
	/* If this flag is set to 1, the _init() function was called and all
	global/static constructors have been called. */
	static int init_was_called = 0;

	/* A function with the ((constructor)) attribute its a static
	constructor and will be call at crt start (init func)
	*/
	__attribute__ ((constructor))
		void loaderInit()
	{
		//init was called
		init_was_called = 1;
	}

	//when crt start init func was invoked
	extern void _init(void); 

#ifdef __cplusplus
}
#endif


// app entry point
int main(int argc, char **argv)  
{
	/* Fanjita's EBOOT loader can be configured to skip the call to _init().
	Since we need _init() for C++, we check to see if _init() has been
	called.  If it hasn't we call it manually
	*/
	if (!init_was_called) {
		_init();
	} 

	
	//create app
	CPsPectrum* app = new CPsPectrum();

	//Init , run & destroy app
	if (app->Init(argv[0]))
	{	
		app->Run();
		app->End();
	}

	//free app
	delete app;

	//exit
	sceKernelExitGame();
	
	return 0;
}
