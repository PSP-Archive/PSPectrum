/* machines.h: machine specific routines
   Copyright (c) 1999-2004 Philip Kendall

   $Id: machines.h,v 1.7 2004/05/21 11:02:17 pak21 Exp $

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

#ifndef FUSE_MACHINES_H
#define FUSE_MACHINES_H

#include <libspectrum.h>

#include "machine.h"

int pentagon_init( fuse_machine_info *machine );
int pentagon_port_contended( libspectrum_word port );

int spec16_init( fuse_machine_info *machine );
int spec_se_init( fuse_machine_info *machine );
int specplus2_init( fuse_machine_info *machine );
int specplus2a_init( fuse_machine_info *machine );
int specplus3e_init( fuse_machine_info *machine );

int tc2048_init( fuse_machine_info *machine );
int tc2048_port_contended( libspectrum_word port );

int tc2068_init( fuse_machine_info *machine );
int tc2068_tc2048_common_reset( void );
libspectrum_byte tc2068_unattached_port( void );
int tc2068_memory_map( void );

#endif			/* #ifndef FUSE_MACHINES_H */
