/* display.h: Routines for printing the Spectrum's screen
   Copyright (c) 1999-2003 Philip Kendall

   $Id: display.h,v 1.21 2003/07/11 11:03:25 pak21 Exp $

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

#ifndef FUSE_DISPLAY_H
#define FUSE_DISPLAY_H

#include <stddef.h>
#include <libspectrum.h>


#ifdef __cplusplus
	extern "C" {
#endif

/* The width and height of the Speccy's screen */
#define DISPLAY_WIDTH_COLS  32
#define DISPLAY_HEIGHT_ROWS 24

/* The width and height of the Speccy's screen */
/* Each main screen column can produce 16 pixels in hires mode */
#define DISPLAY_WIDTH         ( DISPLAY_WIDTH_COLS * 16 )
/* Each main screen row can produce only 8 pixels in any mode */
#define DISPLAY_HEIGHT        ( DISPLAY_HEIGHT_ROWS * 8 )

/* The width and height of the (emulated) border */
#define DISPLAY_BORDER_WIDTH_COLS  4
#define DISPLAY_BORDER_HEIGHT_COLS 3

/* The width and height of the (emulated) border */
/* Each main screen column can produce 16 pixels in hires mode */
#define DISPLAY_BORDER_WIDTH  ( DISPLAY_BORDER_WIDTH_COLS * 16 )
/* Aspect corrected border width */
#define DISPLAY_BORDER_ASPECT_WIDTH  ( DISPLAY_BORDER_WIDTH_COLS * 8 )
/* Each main screen column can produce only 8 pixels in any mode */
#define DISPLAY_BORDER_HEIGHT ( DISPLAY_BORDER_HEIGHT_COLS * 8 )

/* The width and height of the window we'll be displaying */
#define DISPLAY_SCREEN_WIDTH  ( DISPLAY_WIDTH  + 2 * DISPLAY_BORDER_WIDTH  )
#define DISPLAY_SCREEN_HEIGHT ( DISPLAY_HEIGHT + 2 * DISPLAY_BORDER_HEIGHT )

/* And the width in columns */
#define DISPLAY_SCREEN_WIDTH_COLS ( DISPLAY_WIDTH_COLS + 2 * DISPLAY_BORDER_WIDTH_COLS )

/* The aspect ratio corrected display width */
#define DISPLAY_ASPECT_WIDTH  ( DISPLAY_SCREEN_WIDTH / 2 )

extern int display_ui_initialised;

extern libspectrum_word
  display_image[ /*2 * */DISPLAY_SCREEN_HEIGHT ][ DISPLAY_SCREEN_WIDTH ];
extern ptrdiff_t display_pitch;

extern libspectrum_byte display_lores_border;
extern libspectrum_byte display_hires_border;

/* Offsets as to where the data and the attributes for each pixel
   line start */
extern libspectrum_word display_line_start[ DISPLAY_HEIGHT ];
extern libspectrum_word display_attr_start[ DISPLAY_HEIGHT ];

int display_init(int *argc, char ***argv);
void display_line(void);

void display_dirty( libspectrum_word address );
void display_putpixel( int x, int y, int colour );
void display_plot8( int x, int y, libspectrum_byte data, libspectrum_byte ink,
		    libspectrum_byte paper );
void display_plot16( int x, int y, libspectrum_word data, libspectrum_byte ink,
		     libspectrum_byte paper);

void display_parse_attr( libspectrum_byte attr, libspectrum_byte *ink,
			 libspectrum_byte *paper );

void display_set_lores_border(int colour);
void display_set_hires_border(int colour);
int display_dirty_border(void);

int display_frame(void);
void display_refresh_all(void);

libspectrum_word display_get_addr( int x, int y );

#ifdef __cplusplus
	}
#endif

#endif			/* #ifndef FUSE_DISPLAY_H */
