/* fuse.c: The Free Unix Spectrum Emulator
   Copyright (c) 1999-2004 Philip Kendall

   $Id: fuse.c,v 1.120 2004/05/31 17:38:19 pak21 Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

   Author contact information:

   E-mail: pak21-fuse@srcf.ucam.org
   Postal address: 15 Crescent Road, Wokingham, Berks, RG40 2DB, England

*/

#include <config.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#ifndef WIN32
	//#include <sys/utsname.h>
#else				/* #ifndef WIN32 */
#include <windows.h>
#include <direct.h>
#define getcwd _getcwd
#define snprintf _snprintf

#endif				/* #ifndef WIN32 */

//#include <unistd.h>

#ifdef UI_SDL
#include <SDL.h>		/* Needed on MacOS X and Windows */
#endif				/* #ifdef UI_SDL */

//#include "dck.h"
//#include "debugger/debugger.h"
#include "display.h"
#include "event.h"
#include "fuse.h"
#include "joystick.h"
#include "keyboard.h"
#include "machine.h"
#include "memory.h"
//#include "pokefinder/pokefinder.h"
#include "printer.h"
//#include "psg.h"
//#include "rzx.h"
#include "settings.h"
//#include "simpleide.h"
#include "snapshot.h"
#include "sound.h"
//#include "sound/lowlevel.h"
#include "spectrum.h"
#include "tape.h"
#include "timer.h"
//#include "trdos.h"
#include "ui/ui.h"
//#include "ui/scaler/scaler.h"
#include "utils.h"
//#include "zxatasp.h"
//#include "zxcf.h"

#ifdef USE_WIDGET
#include "widget/widget.h"
#endif                          /* #ifdef USE_WIDGET */

#include "z80/z80.h"

/* What name were we called under? */
char *fuse_progname;

/* Which directory were we started in? */
char fuse_directory[ PATH_MAX ];

/* A flag to say when we want to exit the emulator */
int fuse_exiting;

/* Is Spectrum emulation currently paused, and if so, how many times? */
int fuse_emulation_paused;

/* Are we going to try and use the sound card; this differs from
   sound.c:sound_enabled in that this gives a desire, whereas sound_enabled
   is an actual state; when the Spectrum emulation is not running, this
   stores whether we try to reenable the sound card afterwards */
int fuse_sound_in_use;

/* The creator information we'll store in file formats that support this */
libspectrum_creator *fuse_creator;

/* The earliest version of libspectrum we need */
static const char *LIBSPECTRUM_MIN_VERSION = "0.2.0.1";

/* The various types of file we may want to run on startup */
typedef struct start_files_t {

  const char *disk_plus3;
  const char *disk_trdos;
  const char *dock;
  const char *if2;
  const char *playback;
  const char *recording;
  const char *snapshot;
  const char *tape;

  const char *simpleide_master, *simpleide_slave;
  const char *zxatasp_master, *zxatasp_slave;
  const char *zxcf;

} start_files_t;

/*int fuse_init(int argc, char **argv);
int fuse_end(void);*/

//static int creator_init( void );
static void fuse_show_copyright(void);
static void fuse_show_version( void );
static void fuse_show_help( void );

static int setup_start_files( start_files_t *start_files );
static int parse_nonoption_args( int argc, char **argv, int first_arg,
				 start_files_t *start_files );
static int do_start_files( start_files_t *start_files );

void DoEmulation()
{
  fuse_exiting=0;
	while( !fuse_exiting ) {
    z80_do_opcodes();
	event_do_events();
  }
}



int fuse_init(int argc, char **argv)
{
  int error, first_arg;
  char *start_scaler;
  start_files_t start_files;

  fuse_progname=argv[0];
  //Use default libspectrum_error_function
  //libspectrum_error_function = ui_libspectrum_error;

  if( !getcwd( fuse_directory, PATH_MAX - 1 ) ) {
    ui_error( UI_ERROR_ERROR, "error getting current working directory: %s",
	      strerror( errno ) );
    return 1;
  }
  strcat( fuse_directory, "/" );

  if( settings_init( &first_arg, argc, argv ) ) return 1;

  /*if( settings_current.show_version ) {
    fuse_show_version();
    return 0;
  } else if( settings_current.show_help ) {
    fuse_show_help();
    return 0;
  }

  start_scaler = strdup( settings_current.start_scaler_mode );
  if( !start_scaler ) {
    ui_error( UI_ERROR_ERROR, "Out of memory at %s: %d", __FILE__, __LINE__ );
    return 1;
  }*/

  //fuse_show_copyright();

  /* FIXME: order of these initialisation calls. Work out what depends on
     what */
  /* FIXME FIXME 20030407: really do this soon. This is getting *far* too
     hairy */
  fuse_joystick_init ();
  fuse_keyboard_init();

#ifdef USE_WIDGET
  if( widget_init() ) return 1;
#endif				/* #ifdef USE_WIDGET */

  if( event_init() ) return 1;
  
  if( display_init(&argc,&argv) ) return 1;

  if( libspectrum_check_version( LIBSPECTRUM_MIN_VERSION ) ) {
    if( libspectrum_init() ) return 1;
  } else {
    ui_error( UI_ERROR_ERROR,
              "libspectrum version %s found, but %s required",
	      libspectrum_version(), LIBSPECTRUM_MIN_VERSION );
    return 1;
  }

  /* Must be called after libspectrum_init() so we can get the gcrypt
     version */
  //if( creator_init() ) return 1;

#ifdef HAVE_GETEUID
  /* Drop root privs if we have them */
  if( !geteuid() ) { setuid( getuid() ); }
#endif				/* #ifdef HAVE_GETEUID */

  if( memory_init() ) return 1;

//  if( debugger_init() ) return 1;

//  if( printer_init() ) return 1;
//  if( rzx_init() ) return 1;
//  if( psg_init() ) return 1;
//  if( trdos_init() ) return 1;
//  if( simpleide_init() ) return 1;
//  if( zxatasp_init() ) return 1;
//  if( zxcf_init() ) return 1;

//  error = pokefinder_clear(); if( error ) return error;

  z80_init();

  fuse_sound_in_use = 0;
  if( settings_current.sound && settings_current.emulation_speed == 100 )
    sound_init( settings_current.sound_device );

  /* Timing: if we're not producing sound or using sound routines which
     don't do timing control for us, use the timer_* routines */
#ifdef ALWAYS_USE_TIMER
  if( sound_enabled ) fuse_sound_in_use = 1;
  if( timer_init() ) return 1;
#else			/* #ifdef ALWAYS_USE_TIMER */
  if(sound_enabled) {
    fuse_sound_in_use = 1;
  } else {
    settings_current.sound = 0;
    if(timer_init()) return 1;
  }
#endif			/* #ifdef ALWAYS_USE_TIMER */

  error = timer_estimate_reset(); if( error ) return error;

  error = machine_init_machines();
  if( error ) return error;

  error = machine_select_id( settings_current.start_machine );
  if( error ) return error;

  error = tape_init(); if( error ) return error;

  //error = scaler_select_id( start_scaler ); free( start_scaler );
  //if( error ) return error;
    
  if( setup_start_files( &start_files ) ) return 1;
//  if( parse_nonoption_args( argc, argv, first_arg, &start_files ) ) return 1;
  if( do_start_files( &start_files ) ) return 1;

  fuse_emulation_paused = 0;

  return 0;

}
/* Tidy-up function called at end of emulation */
int fuse_end(void)
{
  /* Must happen before memory is deallocated as we read the character
     set from memory for the text output */
   printer_end();

//  psg_end();
//  rzx_end();
//  debugger_end();
//  simpleide_end();
//  zxatasp_end();
//  zxcf_end();

  machine_end();

#ifdef ALWAYS_USE_TIMER
  timer_end();
#else				/* #ifdef ALWAYS_USE_TIMER */
  if(!sound_enabled) timer_end();
#endif				/* #ifdef ALWAYS_USE_TIMER */

  sound_end();
  event_end();
  fuse_joystick_end ();
  ui_end();

#ifdef USE_WIDGET
  widget_end();
#endif                          /* #ifdef USE_WIDGET */

  settings_end();

  //libspectrum_creator_free( fuse_creator );

  return 0;
}

/*
static
int creator_init( void )
{
  size_t i;
  unsigned int version[4] = { 0, 0, 0, 0 };
  char *custom;
  
#ifndef WIN32
  struct utsname buf;
#else				// #ifndef WIN32 
  OSVERSIONINFO buf;
  char *windows_name;
#endif				// #ifndef WIN32 

  libspectrum_error error; int sys_error;

  const char *gcrypt_version;

  sscanf( VERSION, "%u.%u.%u.%u",
	  &version[0], &version[1], &version[2], &version[3] );

  for( i=0; i<4; i++ ) if( version[i] > 0xff ) version[i] = 0xff;

#ifndef WIN32
  sys_error = uname( &buf );
  if( sys_error == -1 ) {
    ui_error( UI_ERROR_ERROR, "error getting system information: %s",
	      strerror( errno ) );
    return 1;
  }
#else				// #ifndef WIN32 
  buf.dwOSVersionInfoSize = sizeof( buf );
  sys_error = GetVersionEx( &buf );
  if( sys_error == 0 ) {
    ui_error( UI_ERROR_ERROR, "error getting system information." );
    return 1;
  }
#endif				// #ifndef WIN32 

  //error = libspectrum_creator_alloc( &fuse_creator ); if( error ) return error;

  error = libspectrum_creator_set_program( fuse_creator, "Fuse" );
  if( error ) { libspectrum_creator_free( fuse_creator ); return error; }

  error = libspectrum_creator_set_major( fuse_creator,
					 version[0] * 0x100 + version[1] );
  if( error ) { libspectrum_creator_free( fuse_creator ); return error; }

  error = libspectrum_creator_set_minor( fuse_creator,
					 version[2] * 0x100 + version[3] );
  if( error ) { libspectrum_creator_free( fuse_creator ); return error; }

  custom = malloc( 256 );
  if( !custom ) {
    ui_error( UI_ERROR_ERROR, "out of memory at %s:%d", __FILE__, __LINE__ );
    libspectrum_creator_free( fuse_creator );
    return 1;
  }

  gcrypt_version = libspectrum_gcrypt_version();
  if( !gcrypt_version ) gcrypt_version = "not available";

#ifndef WIN32	    

  snprintf( custom, 256, "gcrypt: %s\nlibspectrum: %s\nuname: %s %s %s",
	    gcrypt_version, libspectrum_version(),
	    buf.sysname, buf.machine, buf.release );

#else				// #ifndef WIN32 

  switch( buf.dwPlatformId ) {
  case VER_PLATFORM_WIN32_NT:	   windows_name = "NT";      break;
  case VER_PLATFORM_WIN32_WINDOWS: windows_name = "95/98";   break;
  case VER_PLATFORM_WIN32s:	   windows_name = "3.1";     break;
  default:			   windows_name = "unknown"; break;
  }

  snprintf( custom, 256,
	    "gcrypt: %s\nlibspectrum: %s\nuname: Windows %s %d.%d build %d %s",
	    gcrypt_version, libspectrum_version(),
	    windows_name, buf.dwMajorVersion, buf.dwMinorVersion,
	    buf.dwBuildNumber, buf.szCSDVersion );

#endif				// #ifndef WIN32 

  error = libspectrum_creator_set_custom( fuse_creator,
					  custom, strlen( custom ) );
  if( error ) {
    free( custom ); libspectrum_creator_free( fuse_creator );
    return error;
  }

  return 0;
}*/

static void fuse_show_copyright(void)
{
  printf( "\n" );
  fuse_show_version();
  printf(
   "Copyright (c) 1999-2004 Philip Kendall <pak21-fuse@srcf.ucam.org>\n"
   "and others; see the file 'AUTHORS' for more details.\n"
   "\n"
   "This program is distributed in the hope that it will be useful,\n"
   "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
   "GNU General Public License for more details.\n\n");
}

static void fuse_show_version( void )
{
  printf( "The Free Unix Spectrum Emulator (Fuse) version " VERSION ".\n" );
}

static void fuse_show_help( void )
{
  printf( "\n" );
  fuse_show_version();
  printf(
   "\nAvailable command-line options:\n\n"
   "Boolean options (use `--no-<option>' to turn off):\n\n"
   "--auto-load            Automatically load tape files when opened.\n"
   "--beeper-stereo        Add fake stereo to beeper emulation.\n"
   "--compress-rzx         Write RZX files out compressed.\n"
   "--double-screen        Write screenshots out as double size.\n"
   "--issue2               Emulate an Issue 2 Spectrum.\n"
   "--kempston             Emulate the Kempston joystick on QAOP<space>.\n"
   "--loading-sound        Emulate the sound of tapes loading.\n"
   "--separation           Use ACB stereo for the AY-3-8912 sound chip.\n"
   "--sound                Produce sound.\n"
   "--sound-force-8bit     Generate 8-bit sound even if 16-bit is available.\n"
   "--slt                  Turn SLT traps on.\n"
   "--traps                Turn tape traps on.\n\n"
   "Other options:\n\n"
   "--help                 This information.\n"
   "--machine <type>       Which machine should be emulated?\n"
   "--playback <filename>  Play back RZX file <filename>.\n"
   "--record <filename>    Record to RZX file <filename>.\n"
   "--snapshot <filename>  Load snapshot <filename>.\n"
   "--speed <percentage>   How fast should emulation run?\n"
   "--svga-mode <mode>     Which mode should be used for SVGAlib?\n"
   "--tape <filename>      Open tape file <filename>.\n"
   "--version              Print version number and exit.\n\n" );
}

/* Stop all activities associated with actual Spectrum emulation */
int fuse_emulation_pause(void)
{
  //int error;

  /* If we were already paused, just return. In any case, increment
     the pause count */
  if( fuse_emulation_paused++ ) return 0;

  /* Stop recording any competition mode RZX file */
/*  if( rzx_recording && rzx_competition_mode ) {
    ui_error( UI_ERROR_INFO, "Stopping competition mode RZX recording" );
    error = rzx_stop_recording(); if( error ) return error;
  }*/
      
  /* If we had sound enabled (and hence doing the speed regulation),
     turn it off */
  if( sound_enabled ) sound_end();

  return 0;
}

/* Restart emulation activities */
int fuse_emulation_unpause(void)
{
  int error;

  /* If this doesn't start us running again, just return. In any case,
     decrement the pause count */
  if( --fuse_emulation_paused ) return 0;

  /* If we now want sound, enable it */
  if( sound_enabled_ever && settings_current.sound &&
      settings_current.emulation_speed == 100 ) {

    /* If sound has just started providing the timing, remove the old
       SIGALRM timer */
#ifndef ALWAYS_USE_TIMER
    if( !fuse_sound_in_use ) timer_end();
#endif				/* #ifndef ALWAYS_USE_TIMER */

    sound_init( settings_current.sound_device );

    /* If the sound code couldn't re-initialise, fall back to the
       signal based routines */
    if( !sound_enabled ) {
      /* Increment pause_count, report, decrement pause_count
       * (i.e. avoid the effects of fuse_emulation_{,un}pause).
       * Otherwise, we may be recursively reporting this error. */
      fuse_emulation_paused++;
      fuse_emulation_paused--;
      settings_current.sound = fuse_sound_in_use = 0;
      /* FIXME: How to deal with error return here? */
#ifndef ALWAYS_USE_TIMER
      timer_init();
#endif				/* #ifndef ALWAYS_USE_TIMER */

    }
#ifdef ALWAYS_USE_TIMER
    timer_init();
#endif				/* #ifdef ALWAYS_USE_TIMER */
    fuse_sound_in_use = sound_enabled;
  }
#ifndef ALWAYS_USE_TIMER
  /* If we don't want sound any more, put previously did, start the SIGALRM
     timer */
  else if( fuse_sound_in_use ) {
    timer_init();
    fuse_sound_in_use = 0;
  }
#endif				/* #ifndef ALWAYS_USE_TIMER */

  /* Restart speed estimation with no information */
  error = timer_estimate_reset(); if( error ) return error;

  return 0;
}

static int
setup_start_files( start_files_t *start_files )
{
  start_files->disk_plus3 = settings_current.plus3disk_file;
  start_files->disk_trdos = settings_current.trdosdisk_file;
  start_files->dock = settings_current.dck_file;
  start_files->if2 = settings_current.if2_file;
  start_files->playback = settings_current.playback_file;
  start_files->recording = settings_current.record_file;
  start_files->snapshot = settings_current.snapshot;
  start_files->tape = settings_current.tape_file;

  start_files->simpleide_master = settings_current.simpleide_master_file;
  start_files->simpleide_slave  = settings_current.simpleide_slave_file;

  start_files->zxatasp_master = settings_current.zxatasp_master_file;
  start_files->zxatasp_slave  = settings_current.zxatasp_slave_file;

  start_files->zxcf = settings_current.zxcf_pri_file;

  return 0;
}

/* Make 'best guesses' as to what to do with non-option arguments */
static int
parse_nonoption_args( int argc, char **argv, int first_arg,
		      start_files_t *start_files )
{
  size_t i;
  const char *filename;
  utils_file file;
  libspectrum_id_t type;
  libspectrum_class_t class;
  int error;

  for( i = first_arg; i < (size_t)argc; i++ ) {

    filename = argv[i];

    error = utils_read_file( filename, &file );
    if( error ) return error;

    error = libspectrum_identify_file_with_class( &type, &class, filename,
						  file.buffer, file.length );
    if( error ) return error;

    switch( class ) {

    case LIBSPECTRUM_CLASS_CARTRIDGE_TIMEX:
      start_files->dock = filename; break;

    case LIBSPECTRUM_CLASS_CARTRIDGE_IF2:
      start_files->if2 = filename; break;

    case LIBSPECTRUM_CLASS_HARDDISK:
      if( settings_current.zxcf_active ) {
	start_files->zxcf = filename;
      } else if( settings_current.zxatasp_active ) {
	start_files->zxatasp_master = filename;
      } else if( settings_current.simpleide_active ) {
	start_files->simpleide_master = filename;
      } else {
	/* No IDE interface active, so activate the ZXCF */
	settings_current.zxcf_active = 1;
	start_files->zxcf = filename;
      }
      break;

    case LIBSPECTRUM_CLASS_DISK_PLUS3:
      start_files->disk_plus3 = filename; break;

    case LIBSPECTRUM_CLASS_DISK_TRDOS:
      start_files->disk_trdos = filename; break;

    case LIBSPECTRUM_CLASS_RECORDING:
      start_files->playback = filename; break;

    case LIBSPECTRUM_CLASS_SNAPSHOT:
      start_files->snapshot = filename; break;

    case LIBSPECTRUM_CLASS_TAPE:
      start_files->tape = filename; break;

    case LIBSPECTRUM_CLASS_UNKNOWN:
      ui_error( UI_ERROR_WARNING, "couldn't identify '%s'; ignoring it",
		filename );
      break;

    default:
      ui_error( UI_ERROR_ERROR, "parse_nonoption_args: unknown file class %d",
		class );
      break;

    }
  }

  return 0;
}

static int
do_start_files( start_files_t *start_files )
{
  int autoload, error;

  /* Can't do both input recording and playback */
  if( start_files->playback && start_files->recording ) {
    ui_error(
      UI_ERROR_WARNING,
      "can't do both input playback and recording; recording disabled"
    );
    start_files->recording = NULL;
  }

  /* Can't use both +3 and TR-DOS disks simultaneously */
  if( start_files->disk_plus3 && start_files->disk_trdos ) {
    ui_error(
      UI_ERROR_WARNING,
      "can't use +3 and TR-DOS disks simultaneously; +3 disk ignored"
    );
    start_files->disk_plus3 = NULL;
  }

  /* Can't use disks and the dock simultaneously */
  if( ( start_files->disk_plus3 || start_files->disk_trdos ) &&
      start_files->dock                                         ) {
    ui_error(
      UI_ERROR_WARNING,
      "can't use disks and the dock simultaneously; dock cartridge ignored"
    );
    start_files->dock = NULL;
  }

  /* Can't use disks and the Interface II simultaneously */
  if( ( start_files->disk_plus3 || start_files->disk_trdos ) &&
      start_files->if2                                          ) {
    ui_error(
      UI_ERROR_WARNING,
      "can't use disks and the Interface II simultaneously; cartridge ignored"
    );
    start_files->if2 = NULL;
  }

  /* If a snapshot has been specified, don't autoload tape, disks etc */
  autoload = start_files->snapshot ? 0 : settings_current.auto_load;

  /* Load in each of the files. Input recording must be done after
     snapshot loading such that the right snapshot is embedded into
     the file; input playback being done after snapshot loading means
     any embedded snapshot in the input recording will override any
     specified snapshot */

  if( start_files->disk_plus3 ) {
    error = utils_open_file( start_files->disk_plus3, autoload, NULL );
    if( error ) return error;
  }

  if( start_files->disk_trdos ) {
    error = utils_open_file( start_files->disk_trdos, autoload, NULL );
    if( error ) return error;
  }

  if( start_files->dock ) {
    error = utils_open_file( start_files->dock, autoload, NULL );
    if( error ) return error;
  }

  if( start_files->if2 ) {
    error = utils_open_file( start_files->if2, autoload, NULL );
    if( error ) return error;
  }

  if( start_files->snapshot ) {
    error = utils_open_file( start_files->snapshot, autoload, NULL );
    if( error ) return error;
  }

  if( start_files->tape ) {
    error = utils_open_file( start_files->tape, autoload, NULL );
    if( error ) return error;
  }

  /* IDE hard disk images */

/*  if( start_files->simpleide_master ) {
    error = simpleide_insert( start_files->simpleide_master,
			      LIBSPECTRUM_IDE_MASTER );
    simpleide_reset();
    if( error ) return error;
  }

  if( start_files->simpleide_slave ) {
    error = simpleide_insert( start_files->simpleide_slave,
			      LIBSPECTRUM_IDE_SLAVE );
    simpleide_reset();
    if( error ) return error;
  }

  if( start_files->zxatasp_master ) {
    error = zxatasp_insert( start_files->zxatasp_master,
			    LIBSPECTRUM_IDE_MASTER );
    if( error ) return error;
  }

  if( start_files->zxatasp_slave ) {
    error = zxatasp_insert( start_files->zxatasp_slave,
			    LIBSPECTRUM_IDE_SLAVE );
    if( error ) return error;
  }

  if( start_files->zxcf ) {
    error = zxcf_insert( start_files->zxcf ); if( error ) return error;
  }*/

  /* Input recordings */

  /*if( start_files->playback ) {
    error = utils_open_file( start_files->playback, autoload, NULL );
    if( error ) return error;
  }

  if( start_files->recording ) {
    error = rzx_start_recording( start_files->recording,
				 settings_current.embed_snapshot );
    if( error ) return error;
  }*/

  return 0;
}



/* Emergency shutdown */
void
fuse_abort( void )
{
  fuse_end();
  abort();
}
