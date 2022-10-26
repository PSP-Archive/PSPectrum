#ifndef __TYPES__
#define __TYPES__


	#if defined(_WINDOWS)
		typedef	signed		char		s8;
		typedef	unsigned	char		u8; 
		typedef	unsigned	short int	u16;
		typedef	short		int			s16;
		typedef	unsigned	int			u32;
		typedef signed      int			s32;
		typedef unsigned long long int	u64;
		typedef signed long long int	s64;
	#elif _PSP 
		#include "psptypes.h"
	#elif _GP2X	
        typedef	signed		char		s8;
		typedef	unsigned	char		u8; 
		typedef	unsigned	short int	u16;
		typedef	short		int			s16;
		typedef	unsigned	int			u32;
		typedef signed      int			s32;
		typedef unsigned long long int	u64;
		typedef signed long long int s64;
	
  #endif

#endif
