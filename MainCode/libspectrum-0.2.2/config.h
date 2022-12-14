/* config.h.  Generated by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <gcrypt.h> header file. */
/* #undef HAVE_GCRYPT_H */

/* Define to 1 if you have the <inttypes.h> header file. */
#ifdef WIN32
	#undef HAVE_INTTYPES_H
#else
	#define HAVE_INTTYPES_H 1
#endif

/* Define to 1 if you have the `bz2' library (-lbz2). */
#undef HAVE_LIBBZ2

/* Defined if we've got glib */
/* #undef HAVE_LIB_GLIB */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#ifdef WIN32
	#undef HAVE_STDINT_H
#else
	#define HAVE_STDINT_H 1
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#ifdef WIN32
	#undef HAVE_STRCASECMP
#else
	#define HAVE_STRCASECMP 1
#endif



/* Define to 1 if you have the <strings.h> header file. */

#ifdef WIN32
	#undef HAVE_STRINGS_H
#else
	#undef HAVE_STRINGS_H
#endif

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#ifdef WIN32
	#undef HAVE_UNISTD_H 
#else
	#define HAVE_UNISTD_H 1
#endif



/* Define to 1 if you have the <zlib.h> header file. */
#define HAVE_ZLIB_H 1

/* Define to 1 if you have the `_strcmpi' function. */
#ifdef WIN32
	#define HAVE__STRCMPI 1	
#else
	#undef HAVE__STRCMPI	
#endif


/* Name of package */
#define PACKAGE "libspectrum"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "0.2.2"

#define FUSEDATADIR "roms"

//Change to Log in MGE
//#define ui_error(x) ;

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */
