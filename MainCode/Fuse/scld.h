/* scld.h: Routines for handling the Timex SCLD
   Copyright (c) 2002-2004 Fredrick Meunier, Witold Filipczyk

   $Id: scld.h,v 1.13 2004/06/06 22:31:14 pak21 Exp $

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

   E-mail: fredm@spamcop.net

*/

#ifndef FUSE_SCLD_H
#define FUSE_SCLD_H

#ifndef FUSE_MEMORY_H
#include "memory.h"
#endif				/* #ifndef FUSE_MEMORY_H */

#ifdef __cplusplus
	extern "C" {
#endif

#define STANDARD        0x00 /* standard Spectrum */
#define ALTDFILE        0x01 /* the same in nature as above, but using second
                                display file */
#define EXTCOLOUR       0x02 /* extended colours (data taken from first screen,
                                attributes 1x8 taken from second display. */
#define EXTCOLALTD      0x03 /* similar to above, but data is taken from second
                                screen */
#define HIRESATTR       0x04 /* hires mode, data in odd columns is taken from
                                first screen in standard way, data in even
                                columns is made from attributes data (8x8) */
#define HIRESATTRALTD   0x05 /* similar to above, but data taken from second
                                display */
#define HIRES           0x06 /* true hires mode, odd columns from first screen,
                                even columns from second screen.  columns
                                numbered from 1. */
#define HIRESDOUBLECOL  0x07 /* data taken only from second screen, columns are
                                doubled */
#define HIRESCOLMASK    0x38

#define WHITEBLACK      0x00
#define YELLOWBLUE      0x01
#define CYANRED         0x02
#define GREENMAGENTA    0x03
#define MAGENTAGREEN    0x04
#define REDCYAN         0x05
#define BLUEYELLOW      0x06
#define BLACKWHITE      0x07

#define ALTDFILE_OFFSET 0x2000

#ifdef WORDS_BIGENDIAN


typedef struct
{
  unsigned altmembank : 1;  /* ALTMEMBANK : 0 = cartridge, 1 = exrom */
  unsigned intdisable : 1;  /* INTDISABLE */
  unsigned b5  : 1;  /* */
  unsigned b4  : 1;  /* */
  unsigned b3  : 1;  /* */
  unsigned hires  : 1;  /* SCLD HIRES mode */
  unsigned b1     : 1;  /* */
  unsigned altdfile : 1;  /* SCLD use ALTDFILE */
} scld_names;

typedef struct
{
  unsigned b7  : 1;  /* */
  unsigned b6  : 1;  /* */
  unsigned hirescol  : 3;  /* HIRESCOLMASK */
  unsigned scrnmode  : 3;  /* SCRNMODEMASK */
} scld_masks;

#else				/* #ifdef WORDS_BIGENDIAN */

typedef struct
{
  unsigned altdfile : 1;  /* SCLD use ALTDFILE */
  unsigned b1     : 1;  /* */
  unsigned hires  : 1;  /* SCLD HIRES mode */
  unsigned b3  : 1;  /* */
  unsigned b4  : 1;  /* */
  unsigned b5  : 1;  /* */
  unsigned intdisable : 1;  /* INTDISABLE */
  unsigned altmembank : 1;  /* ALTMEMBANK : 0 = cartridge, 1 = exrom */
} scld_names;

typedef struct
{
  unsigned scrnmode  : 3;  /* SCRNMODEMASK */
  unsigned hirescol  : 3;  /* HIRESCOLMASK */
  unsigned b6  : 1;  /* */
  unsigned b7  : 1;  /* */
} scld_masks;

#endif				/* #ifdef WORDS_BIGENDIAN */

typedef union
{
  libspectrum_byte byte;
  scld_masks mask;
  scld_names name;
} scld; 

extern scld scld_last_dec;           /* The last byte sent to Timex DEC port */

extern libspectrum_byte scld_last_hsr; /* Last byte sent to Timex HSR port */

extern memory_page timex_exrom[8];
extern memory_page timex_dock[8];

void scld_reset( void );
void scld_dec_write( libspectrum_word port, libspectrum_byte b );
libspectrum_byte scld_dec_read( libspectrum_word port, int *attached );

void scld_hsr_write( libspectrum_word port, libspectrum_byte b );
libspectrum_byte scld_hsr_read( libspectrum_word port, int *attached );

void scld_memory_map( void );

int scld_from_snapshot( libspectrum_snap *snap, int capabilities );
int scld_to_snapshot( libspectrum_snap *snap );

libspectrum_byte hires_get_attr( void );
libspectrum_byte hires_convert_dec( libspectrum_byte attr );

#ifdef __cplusplus
	}
#endif


#endif                  /* #ifndef FUSE_SCLD_H */
