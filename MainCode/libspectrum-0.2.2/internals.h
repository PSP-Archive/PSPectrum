/* internals.h: functions which need to be called inter-file by libspectrum
                routines, but not by user code
   Copyright (c) 2001-2002 Philip Kendall, Darren Salt

   $Id: internals.h,v 1.19 2004/06/02 13:38:10 pak21 Exp $

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

#ifndef LIBSPECTRUM_INTERNALS_H
#define LIBSPECTRUM_INTERNALS_H

#ifdef HAVE_LIB_GLIB		/* Only if we've got the real glib */
#include <glib.h>
#endif				/* #ifdef HAVE_LIB_GLIB */

#ifndef LIBSPECTRUM_LIBSPECTRUM_H
#include "libspectrum.h"
#endif				/* #ifndef LIBSPECTRUM_LIBSPECTRUM_H */

#ifdef __GNUC__
#define GCC_UNUSED __attribute__ ((unused))
#define GCC_PRINTF( fmtstring, args ) __attribute__ ((format( printf, fmtstring, args )))
#else				/* #ifdef __GNUC__ */
#define GCC_UNUSED
#define GCC_PRINTF( fmtstring, args )
#endif				/* #ifdef __GNUC__ */

/* Win32 systems have _strcmpi, but not strcasecmp */
#if !defined(HAVE_STRCASECMP) && defined(HAVE__STRCMPI)
#define strcasecmp _strcmpi
#endif		/* #if !defined(HAVE_STRCASECMP) && defined(HAVE__STRCMPI) */

/* Print using the user-provided error function */
libspectrum_error
libspectrum_print_error( libspectrum_error error, const char *format, ... )
     GCC_PRINTF( 2, 3 );

/* Acquire more memory for a buffer */
int libspectrum_make_room( libspectrum_byte **dest, size_t requested,
			   libspectrum_byte **ptr, size_t *allocated );

/* Read and write (d)words */
libspectrum_word libspectrum_read_word( const libspectrum_byte **buffer );
libspectrum_dword libspectrum_read_dword( const libspectrum_byte **buffer );
int libspectrum_write_word( libspectrum_byte **buffer, libspectrum_word w );
int libspectrum_write_dword( libspectrum_byte **buffer, libspectrum_dword d );

/* (de)compression routines */

libspectrum_error
libspectrum_uncompress_file( unsigned char **new_buffer, size_t *new_length,
			     char **new_filename, libspectrum_id_t type,
			     const unsigned char *old_buffer,
			     size_t old_length, const char *old_filename );

libspectrum_error
libspectrum_zlib_inflate( const libspectrum_byte *gzptr, size_t gzlength,
			  libspectrum_byte **outptr, size_t *outlength );
libspectrum_error
libspectrum_zlib_compress( const libspectrum_byte *data, size_t length,
			   libspectrum_byte **gzptr, size_t *gzlength );

libspectrum_error
libspectrum_gzip_inflate( const libspectrum_byte *gzptr, size_t gzlength,
			  libspectrum_byte **outptr, size_t *outlength );

libspectrum_error
libspectrum_bzip2_inflate( const libspectrum_byte *bzptr, size_t bzlength,
			   libspectrum_byte **outptr, size_t *outlength );

libspectrum_error
libspectrum_zip_inflate( const char *sFile,libspectrum_byte **outptr, size_t *outlength );

/* Convert a 48K memory dump into separate RAM pages */

int libspectrum_split_to_48k_pages( libspectrum_snap *snap,
				    const libspectrum_byte* data );

/* Sizes of some of the arrays in the snap structure */
#define SNAPSHOT_RAM_PAGES 16
#define SNAPSHOT_SLT_PAGES 256
#define SNAPSHOT_ZXATASP_PAGES 32
#define SNAPSHOT_ZXCF_PAGES 64
#define SNAPSHOT_DOCK_EXROM_PAGES 8

/* Get memory for a snap */

libspectrum_error libspectrum_snap_alloc_internal( libspectrum_snap **snap );

/* Format specific snapshot routines */

libspectrum_error
libspectrum_plusd_read( libspectrum_snap *snap,
			const libspectrum_byte *buffer, size_t buffer_length );
libspectrum_error
libspectrum_sna_write( libspectrum_byte **buffer, size_t *length,
		       int *out_flags, libspectrum_snap *snap, int in_flags );
libspectrum_error
libspectrum_snp_read( libspectrum_snap *snap,
		      const libspectrum_byte *buffer, size_t buffer_length );
libspectrum_error
libspectrum_sp_read( libspectrum_snap *snap,
		     const libspectrum_byte *buffer, size_t buffer_length );
/*libspectrum_error
libspectrum_szx_read( libspectrum_snap *snap,
		      const libspectrum_byte *buffer, size_t buffer_length );
libspectrum_error
libspectrum_szx_write( libspectrum_byte **buffer, size_t *length,
		       int *out_flags, libspectrum_snap *snap,
		       libspectrum_creator *creator, int in_flags );*/
libspectrum_error
libspectrum_z80_write2( libspectrum_byte **buffer, size_t *length,
			int *out_flags, libspectrum_snap *snap, int in_flags );
libspectrum_error
libspectrum_zxs_read( libspectrum_snap *snap,
		      const libspectrum_byte *buffer, size_t buffer_length );

/* Crypto functions */

  //DELETED

#endif				/* #ifndef LIBSPECTRUM_INTERNALS_H */
