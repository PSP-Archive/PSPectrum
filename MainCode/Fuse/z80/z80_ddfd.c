/* opcodes_ddfd.c Z80 {DD,FD}xx opcodes
   Copyright (c) 1999-2003 Philip Kendall

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
   Foundation, Inc., 49 Temple Place, Suite 330, Boston, MA 02111-1307 USA

   Author contact information:

   E-mail: pak21-fuse@srcf.ucam.org
   Postal address: 15 Crescent Road, Wokingham, Berks, RG40 2DB, England

*/

/* NB: this file is autogenerated by 'z80.pl' from 'opcodes_ddfd.dat',
   and included in 'z80_ops.c' */

    case 0x09:		/* ADD REGISTER,BC */
      ADD16(REGISTER,BC);
      break;
    case 0x19:		/* ADD REGISTER,DE */
      ADD16(REGISTER,DE);
      break;
    case 0x21:		/* LD REGISTER,nnnn */
      REGISTERL=readbyte(PC++);
      REGISTERH=readbyte(PC++);
      break;
    case 0x22:		/* LD (nnnn),REGISTER */
      LD16_NNRR(REGISTERL,REGISTERH);
      break;
    case 0x23:		/* INC REGISTER */
      tstates += 2;
      REGISTER++;
      break;
    case 0x24:		/* INC REGISTERH */
      INC(REGISTERH);
      break;
    case 0x25:		/* DEC REGISTERH */
      DEC(REGISTERH);
      break;
    case 0x26:		/* LD REGISTERH,nn */
      REGISTERH = readbyte( PC++ );
      break;
    case 0x29:		/* ADD REGISTER,REGISTER */
      ADD16(REGISTER,REGISTER);
      break;
    case 0x2a:		/* LD REGISTER,(nnnn) */
      LD16_RRNN(REGISTERL,REGISTERH);
      break;
    case 0x2b:		/* DEC REGISTER */
      tstates += 2;
      REGISTER--;
      break;
    case 0x2c:		/* INC REGISTERL */
      INC(REGISTERL);
      break;
    case 0x2d:		/* DEC REGISTERL */
      DEC(REGISTERL);
      break;
    case 0x2e:		/* LD REGISTERL,nn */
      REGISTERL = readbyte( PC++ );
      break;
    case 0x34:		/* INC (REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	libspectrum_word wordtemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	wordtemp = REGISTER + (libspectrum_signed_byte)offset;
	bytetemp = readbyte( wordtemp );
	contend_read( wordtemp, 1 );
	INC(bytetemp);
	writebyte(wordtemp,bytetemp);
      }
      break;
    case 0x35:		/* DEC (REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	libspectrum_word wordtemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	wordtemp = REGISTER + (libspectrum_signed_byte)offset;
	bytetemp = readbyte( wordtemp );
	contend_read( wordtemp, 1 );
	DEC(bytetemp);
	writebyte(wordtemp,bytetemp);
      }
      break;
    case 0x36:		/* LD (REGISTER+dd),nn */
      {
	libspectrum_byte offset, value;
	offset = readbyte( PC++ );
	value = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	writebyte( REGISTER + (libspectrum_signed_byte)offset, value );
      }
      break;
    case 0x39:		/* ADD REGISTER,SP */
      ADD16(REGISTER,SP);
      break;
    case 0x44:		/* LD B,REGISTERH */
      B=REGISTERH;
      break;
    case 0x45:		/* LD B,REGISTERL */
      B=REGISTERL;
      break;
    case 0x46:		/* LD B,(REGISTER+dd) */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	B = readbyte( REGISTER + (libspectrum_signed_byte)offset );
      }
      break;
    case 0x4c:		/* LD C,REGISTERH */
      C=REGISTERH;
      break;
    case 0x4d:		/* LD C,REGISTERL */
      C=REGISTERL;
      break;
    case 0x4e:		/* LD C,(REGISTER+dd) */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	C = readbyte( REGISTER + (libspectrum_signed_byte)offset );
      }
      break;
    case 0x54:		/* LD D,REGISTERH */
      D=REGISTERH;
      break;
    case 0x55:		/* LD D,REGISTERL */
      D=REGISTERL;
      break;
    case 0x56:		/* LD D,(REGISTER+dd) */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	D = readbyte( REGISTER + (libspectrum_signed_byte)offset );
      }
      break;
    case 0x5c:		/* LD E,REGISTERH */
      E=REGISTERH;
      break;
    case 0x5d:		/* LD E,REGISTERL */
      E=REGISTERL;
      break;
    case 0x5e:		/* LD E,(REGISTER+dd) */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	E = readbyte( REGISTER + (libspectrum_signed_byte)offset );
      }
      break;
    case 0x60:		/* LD REGISTERH,B */
      REGISTERH=B;
      break;
    case 0x61:		/* LD REGISTERH,C */
      REGISTERH=C;
      break;
    case 0x62:		/* LD REGISTERH,D */
      REGISTERH=D;
      break;
    case 0x63:		/* LD REGISTERH,E */
      REGISTERH=E;
      break;
    case 0x64:		/* LD REGISTERH,REGISTERH */
      break;
    case 0x65:		/* LD REGISTERH,REGISTERL */
      REGISTERH=REGISTERL;
      break;
    case 0x66:		/* LD H,(REGISTER+dd) */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	H = readbyte( REGISTER + (libspectrum_signed_byte)offset );
      }
      break;
    case 0x67:		/* LD REGISTERH,A */
      REGISTERH=A;
      break;
    case 0x68:		/* LD REGISTERL,B */
      REGISTERL=B;
      break;
    case 0x69:		/* LD REGISTERL,C */
      REGISTERL=C;
      break;
    case 0x6a:		/* LD REGISTERL,D */
      REGISTERL=D;
      break;
    case 0x6b:		/* LD REGISTERL,E */
      REGISTERL=E;
      break;
    case 0x6c:		/* LD REGISTERL,REGISTERH */
      REGISTERL=REGISTERH;
      break;
    case 0x6d:		/* LD REGISTERL,REGISTERL */
      break;
    case 0x6e:		/* LD L,(REGISTER+dd) */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	L = readbyte( REGISTER + (libspectrum_signed_byte)offset );
      }
      break;
    case 0x6f:		/* LD REGISTERL,A */
      REGISTERL=A;
      break;
    case 0x70:		/* LD (REGISTER+dd),B */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	writebyte( REGISTER + (libspectrum_signed_byte)offset, B );
      }
      break;
    case 0x71:		/* LD (REGISTER+dd),C */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	writebyte( REGISTER + (libspectrum_signed_byte)offset, C );
      }
      break;
    case 0x72:		/* LD (REGISTER+dd),D */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	writebyte( REGISTER + (libspectrum_signed_byte)offset, D );
      }
      break;
    case 0x73:		/* LD (REGISTER+dd),E */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	writebyte( REGISTER + (libspectrum_signed_byte)offset, E );
      }
      break;
    case 0x74:		/* LD (REGISTER+dd),H */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	writebyte( REGISTER + (libspectrum_signed_byte)offset, H );
      }
      break;
    case 0x75:		/* LD (REGISTER+dd),L */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	writebyte( REGISTER + (libspectrum_signed_byte)offset, L );
      }
      break;
    case 0x77:		/* LD (REGISTER+dd),A */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	writebyte( REGISTER + (libspectrum_signed_byte)offset, A );
      }
      break;
    case 0x7c:		/* LD A,REGISTERH */
      A=REGISTERH;
      break;
    case 0x7d:		/* LD A,REGISTERL */
      A=REGISTERL;
      break;
    case 0x7e:		/* LD A,(REGISTER+dd) */
      {
	libspectrum_byte offset;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	A = readbyte( REGISTER + (libspectrum_signed_byte)offset );
      }
      break;
    case 0x84:		/* ADD A,REGISTERH */
      ADD(REGISTERH);
      break;
    case 0x85:		/* ADD A,REGISTERL */
      ADD(REGISTERL);
      break;
    case 0x86:		/* ADD A,(REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	bytetemp = readbyte( REGISTER + (libspectrum_signed_byte)offset );
	ADD(bytetemp);
      }
      break;
    case 0x8c:		/* ADC A,REGISTERH */
      ADC(REGISTERH);
      break;
    case 0x8d:		/* ADC A,REGISTERL */
      ADC(REGISTERL);
      break;
    case 0x8e:		/* ADC A,(REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	bytetemp = readbyte( REGISTER + (libspectrum_signed_byte)offset );
	ADC(bytetemp);
      }
      break;
    case 0x94:		/* SUB A,REGISTERH */
      SUB(REGISTERH);
      break;
    case 0x95:		/* SUB A,REGISTERL */
      SUB(REGISTERL);
      break;
    case 0x96:		/* SUB A,(REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	bytetemp = readbyte( REGISTER + (libspectrum_signed_byte)offset );
	SUB(bytetemp);
      }
      break;
    case 0x9c:		/* SBC A,REGISTERH */
      SBC(REGISTERH);
      break;
    case 0x9d:		/* SBC A,REGISTERL */
      SBC(REGISTERL);
      break;
    case 0x9e:		/* SBC A,(REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	bytetemp = readbyte( REGISTER + (libspectrum_signed_byte)offset );
	SBC(bytetemp);
      }
      break;
    case 0xa4:		/* AND A,REGISTERH */
      AND(REGISTERH);
      break;
    case 0xa5:		/* AND A,REGISTERL */
      AND(REGISTERL);
      break;
    case 0xa6:		/* AND A,(REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	bytetemp = readbyte( REGISTER + (libspectrum_signed_byte)offset );
	AND(bytetemp);
      }
      break;
    case 0xac:		/* XOR A,REGISTERH */
      XOR(REGISTERH);
      break;
    case 0xad:		/* XOR A,REGISTERL */
      XOR(REGISTERL);
      break;
    case 0xae:		/* XOR A,(REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	bytetemp = readbyte( REGISTER + (libspectrum_signed_byte)offset );
	XOR(bytetemp);
      }
      break;
    case 0xb4:		/* OR A,REGISTERH */
      OR(REGISTERH);
      break;
    case 0xb5:		/* OR A,REGISTERL */
      OR(REGISTERL);
      break;
    case 0xb6:		/* OR A,(REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	bytetemp = readbyte( REGISTER + (libspectrum_signed_byte)offset );
	OR(bytetemp);
      }
      break;
    case 0xbc:		/* CP A,REGISTERH */
      CP(REGISTERH);
      break;
    case 0xbd:		/* CP A,REGISTERL */
      CP(REGISTERL);
      break;
    case 0xbe:		/* CP A,(REGISTER+dd) */
      {
	libspectrum_byte offset, bytetemp;
	offset = readbyte( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); contend_read( PC, 1 );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
	bytetemp = readbyte( REGISTER + (libspectrum_signed_byte)offset );
	CP(bytetemp);
      }
      break;
    case 0xcb:		/* shift DDFDCB */
      {
	libspectrum_word tempaddr; libspectrum_byte opcode3;
	contend_read( PC, 3 );
	tempaddr =
	    REGISTER + (libspectrum_signed_byte)readbyte_internal( PC );
	PC++; contend_read( PC, 3 );
	opcode3 = readbyte_internal( PC );
	contend_read( PC, 1 ); contend_read( PC, 1 ); PC++;
#ifdef HAVE_ENOUGH_MEMORY
	switch(opcode3) {
#include "z80_ddfdcb.c"
	}
#else			/* #ifdef HAVE_ENOUGH_MEMORY */
	z80_ddfdcbxx(opcode3,tempaddr);
#endif			/* #ifdef HAVE_ENOUGH_MEMORY */
      }
      break;
    case 0xe1:		/* POP REGISTER */
      POP16(REGISTERL,REGISTERH);
      break;
    case 0xe3:		/* EX (SP),REGISTER */
      {
	libspectrum_byte bytetempl, bytetemph;
	bytetempl = readbyte( SP );
	bytetemph = readbyte( SP + 1 ); tstates++;
	writebyte( SP,     REGISTERL  );
	writebyte( SP + 1, REGISTERH );
	contend_write( SP + 1, 1 ); contend_write( SP + 1, 1 );
	REGISTERL=bytetempl; REGISTERH=bytetemph;
      }
      break;
    case 0xe5:		/* PUSH REGISTER */
      tstates++;
      PUSH16(REGISTERL,REGISTERH);
      break;
    case 0xe9:		/* JP REGISTER */
      PC=REGISTER;		/* NB: NOT INDIRECT! */
      break;
    case 0xf9:		/* LD SP,REGISTER */
      tstates += 2;
      SP=REGISTER;
      break;
    default:		/* Instruction did not involve H or L, so backtrack
			   one instruction and parse again */
      PC--;		/* FIXME: will be contended again */
      R--;		/* Decrement the R register as well */
      break;
