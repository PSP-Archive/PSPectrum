/* specplus2.c: Spectrum +2 specific routines
   Copyright (c) 1999-2004 Philip Kendall

   $Id: specplus2.c,v 1.44 2004/05/14 12:35:02 pak21 Exp $

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

#include <stdio.h>

#include <libspectrum.h>

#include "machines.h"
#include "settings.h"
#include "spec128.h"
#include "spec48.h"

/* The +2 emulation just uses the 128K routines */

static int specplus2_reset( void );

int specplus2_init( fuse_machine_info *machine )
{
  machine->machine = LIBSPECTRUM_MACHINE_PLUS2;
  machine->id = "plus2";

  machine->reset = specplus2_reset;

  machine->timex = 0;
  machine->ram.port_contended	     = spec48_port_contended;
  machine->ram.contend_delay	     = spec128_contend_delay;

  machine->unattached_port = spec128_unattached_port;

  machine->shutdown = NULL;

  machine->memory_map = spec128_memory_map;

  return 0;

}

static int
specplus2_reset( void )
{
  int error;

  error = machine_load_rom( 0, 0, settings_current.rom_plus2_0, 0x4000 );
  if( error ) return error;
  error = machine_load_rom( 2, 1, settings_current.rom_plus2_1, 0x4000 );
  if( error ) return error;

  error = periph_setup( spec128_peripherals, spec128_peripherals_count,
			PERIPH_PRESENT_OPTIONAL,
			PERIPH_PRESENT_OPTIONAL );
  if( error ) return error;

  return spec128_common_reset( 1 );
}
