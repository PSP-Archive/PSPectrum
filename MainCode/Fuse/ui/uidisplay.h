/* uidisplay.h: Low-level display routines
   Copyright (c) 2000-2003 Philip Kendall

   $Id: uidisplay.h,v 1.21 2003/07/11 11:03:25 pak21 Exp $

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

#ifndef FUSE_UIDISPLAY_H
#define FUSE_UIDISPLAY_H

#include <libspectrum.h>

/* User interface specific functions */

#ifdef __cplusplus
	extern "C" {
#endif

/* The colour palette */
extern u16 rgb_colors[16];

int uidisplay_init( int width, int height );
void uidisplay_area( int x, int y, int w, int h );
void uidisplay_frame_end( void );
void uidisplay_hotswap_gfx_mode( void );

int uidisplay_end(void);

/* General functions */

//void uidisplay_spectrum_screen( const libspectrum_byte *screen, int border );

#ifdef __cplusplus
	}
#endif

#endif			/* #ifndef FUSE_UIDISPLAY_H */
