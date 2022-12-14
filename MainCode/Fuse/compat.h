/* compat.h: various compatbility bits
   Copyright (c) 2003 Philip Kendall

   $Id: compat.h,v 1.6 2004/04/11 10:13:22 pak21 Exp $

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

#ifndef FUSE_COMPAT_H
#define FUSE_COMPAT_H

#include "../types.h"

/* Remove the gcc-specific incantations if we're not using gcc */
#ifdef __GNUC__

#define GCC_UNUSED __attribute__ ((unused))
#define GCC_PRINTF( fmtstring, args ) __attribute__ ((format( printf, fmtstring, args )))
#define GCC_NORETURN __attribute__ ((noreturn))

#else				/* #ifdef __GNUC__ */

#define GCC_UNUSED
#define GCC_PRINTF( fmtstring, args ) ui_error(fmtstring,args);
#define GCC_NORETURN

#endif				/* #ifdef __GNUC__ */

/* Certain brain damaged operating systems (DOS/Windows) treat text
   and binary files different in open(2) and need to be given the
   O_BINARY flag to tell them it's a binary file */
#ifndef O_BINARY
#define O_BINARY 0
#endif				/* #ifndef O_BINARY */

/* Replacement functions */
#ifndef HAVE_DIRNAME
char *dirname( char *path );
#endif				/* #ifndef HAVE_DIRNAME */

/*#ifndef HAVE_GETOPT_LONG
#include "compat/getopt.h"
#endif*/				/* #ifndef HAVE_GETOPT_LONG */

/*#ifndef HAVE_MKSTEMP
int mkstemp( char * temp );
#endif*/				/* #ifndef HAVE_MKSTEMP */

#define PATH_MAX  256

#ifdef WIN32
	#define snprintf _snprintf
	#define mkstemp _mktemp
	typedef s32 ssize_t;
#endif

#endif				/* #ifndef FUSE_COMPAT_H */
