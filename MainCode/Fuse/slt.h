/* slt.h: SLT data handling routines
   Copyright (c) 2004 Philip Kendall

   $Id: slt.h,v 1.2 2004/06/09 10:13:59 pak21 Exp $

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

#ifndef FUSE_SLT_H
#define FUSE_SLT_H

#include <libspectrum.h>

int slt_trap( libspectrum_word address, libspectrum_byte level );

int slt_from_snapshot( libspectrum_snap *snap );
int slt_to_snapshot( libspectrum_snap *snap );

#endif				/* #ifndef FUSE_SLT_H */
