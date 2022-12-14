/* machine.h: Routines for handling the various machine types
   Copyright (c) 1999-2004 Philip Kendall

   $Id: machine.h,v 1.46 2004/05/14 12:35:01 pak21 Exp $

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

#ifndef FUSE_MACHINE_H
#define FUSE_MACHINE_H

#include <stdlib.h>

#include <libspectrum.h>

#include "ay.h"
#include "display.h"
#include "memory.h"
#include "spectrum.h"

#ifdef __cplusplus
	extern "C" {
#endif

typedef libspectrum_byte (*spectrum_unattached_port_fn)( void );

/* How long do things take to happen; fields are pulled from libspectrum
   via the libspectrum_timings_* functions */
typedef struct machine_timings {

  /* Processor speed */
  libspectrum_dword processor_speed;

  /* Line timings in tstates */
  libspectrum_word left_border, horizontal_screen, right_border;
  libspectrum_word tstates_per_line;

  /* Frame timing */
  libspectrum_dword tstates_per_frame;

} machine_timings;

typedef struct fuse_machine_info {

  libspectrum_machine machine; /* which machine type is this? */
  const char *id;	/* Used to select from command line */
  int capabilities;	/* Capabilities of this machine */

  int (*reset)(void);	/* Reset function */

  int timex;      /* Timex machine (keyboard emulation/loading sounds etc.) */

  machine_timings timings; /* How long do things take to happen? */
  /* Redraw line y this many tstates after interrupt */
  libspectrum_dword line_times[DISPLAY_SCREEN_HEIGHT+1];

  spectrum_raminfo ram; /* How do we access memory, and what's currently
			   paged in */

  spectrum_unattached_port_fn unattached_port; /* What to return if we read
						  from a port which isn't
						  attached to anything */

  ayinfo ay;		/* The AY-8-3912 chip */

  int (*shutdown)( void );

  int (*memory_map)( void );

} fuse_machine_info;

extern fuse_machine_info **machine_types;	/* All available machines */
extern int machine_count;		/* of which there are this many */

extern fuse_machine_info *machine_current;	/* The currently selected machine */

int machine_init_machines( void );

int machine_select( libspectrum_machine type );
int machine_select_id( const char *id );
const char* machine_get_id( libspectrum_machine type );

int machine_set_timings( fuse_machine_info *machine );

int machine_load_rom_bank( memory_page* bank_map, size_t which, int page_num,
			   const char *filename, size_t expected_length );
int machine_load_rom( size_t which, int page_num, const char *filename,
		      size_t expected_length );

int machine_reset( void );
int machine_end( void );

#ifdef __cplusplus
	}
#endif

#endif			/* #ifndef FUSE_MACHINE_H */
