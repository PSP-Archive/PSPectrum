/* specplus2a.c: Spectrum +2A specific routines
   Copyright (c) 1999-2004 Philip Kendall

   $Id: specplus2a.c,v 1.28 2004/06/09 10:55:07 pak21 Exp $

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

#include <libspectrum.h>

#include "joystick.h"
#include "periph.h"
#include "printer.h"
#include "settings.h"
#include "spec128.h"
#include "specplus3.h"
#include "ula.h"

static int specplus2a_reset( void );

const static periph_t peripherals[] = {
  { 0x0001, 0x0000, ula_read, ula_write },
  { 0x00e0, 0x0000, joystick_kempston_read, NULL },
  { 0xc002, 0xc000, ay_registerport_read, ay_registerport_write },
  { 0xc002, 0x8000, NULL, ay_dataport_write },
  { 0xc002, 0x4000, NULL, spec128_memoryport_write },
  { 0xf002, 0x1000, NULL, specplus3_memoryport2_write },
  { 0xf002, 0x0000, printer_parallel_read, printer_parallel_write },
};

const static size_t peripherals_count =
  sizeof( peripherals ) / sizeof( periph_t );

int
specplus2a_init( fuse_machine_info *machine )
{
  machine->machine = LIBSPECTRUM_MACHINE_PLUS2A;
  machine->id = "plus2a";

  machine->reset = specplus2a_reset;

  machine->timex = 0;
  machine->ram.port_contended	     = specplus3_port_contended;
  machine->ram.contend_delay	     = specplus3_contend_delay;

  machine->unattached_port = specplus3_unattached_port;

  machine->shutdown = NULL;

  machine->memory_map = specplus3_memory_map;

  return 0;

}

static int
specplus2a_reset( void )
{
  int error;

  error = machine_load_rom( 0, 0, settings_current.rom_plus2a_0, 0x4000 );
  if( error ) return error;
  error = machine_load_rom( 2, 1, settings_current.rom_plus2a_1, 0x4000 );
  if( error ) return error;
  error = machine_load_rom( 4, 2, settings_current.rom_plus2a_2, 0x4000 );
  if( error ) return error;
  error = machine_load_rom( 6, 3, settings_current.rom_plus2a_3, 0x4000 );
  if( error ) return error;

  error = periph_setup( peripherals, peripherals_count,
			PERIPH_PRESENT_OPTIONAL,
			PERIPH_PRESENT_NEVER );
  if( error ) return error;

  return specplus3_plus2a_common_reset();
}

