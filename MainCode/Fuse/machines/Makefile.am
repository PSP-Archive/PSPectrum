## Process this file with automake to produce Makefile.in
## Copyright (c) 2004 Philip Kendall

## $Id: Makefile.am,v 1.4 2004/05/13 14:29:13 pak21 Exp $

## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##
## Author contact information:
##
## E-mail: pak21-fuse@srcf.ucam.org
## Postal address: 15 Crescent Road, Wokingham, Berks, RG40 2DB, England

AUTOMAKE_OPTIONS = foreign

INCLUDES = @LIBSPEC_CFLAGS@

noinst_LIBRARIES = libmachines.a

libmachines_a_SOURCES = pentagon.c \
			scorpion.c \
			spec128.c \
			spec16.c \
			spec48.c \
			specplus2.c \
			spec_se.c \
			specplus2a.c \
			specplus3.c \
			specplus3e.c \
			tc2048.c \
			tc2068.c

noinst_HEADERS = machines.h \
		 scorpion.h \
		 spec128.h \
		 spec48.h \
		 specplus3.h
