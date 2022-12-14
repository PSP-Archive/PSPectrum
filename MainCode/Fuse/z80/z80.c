/* z80.c: z80 supplementary functions
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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

   Author contact information:

   E-mail: pak21-fuse@srcf.ucam.org
   Postal address: 15 Crescent Road, Wokingham, Berks, RG40 2DB, England

*/

#include <config.h>

#include <libspectrum.h>

#include "event.h"
#include "fuse.h"
#include "memory.h"
//#include "rzx.h"
//#include "scld.h"
#include "spectrum.h"
#include "ui/ui.h"
#include "z80.h"
#include "z80_macros.h"

/* Whether a half carry occured or not can be determined by looking at
   the 3rd bit of the two arguments and the result; these are hashed
   into this table in the form r12, where r is the 3rd bit of the
   result, 1 is the 3rd bit of the 1st argument and 2 is the
   third bit of the 2nd argument; the tables differ for add and subtract
   operations */
const libspectrum_byte halfcarry_add_table[] =
  { 0, FLAG_H, FLAG_H, FLAG_H, 0, 0, 0, FLAG_H };
const libspectrum_byte halfcarry_sub_table[] =
  { 0, 0, FLAG_H, 0, FLAG_H, 0, FLAG_H, FLAG_H };

/* Similarly, overflow can be determined by looking at the 7th bits; again
   the hash into this table is r12 */
const libspectrum_byte overflow_add_table[] = { 0, 0, 0, FLAG_V, FLAG_V, 0, 0, 0 };
const libspectrum_byte overflow_sub_table[] = { 0, FLAG_V, 0, 0, 0, 0, FLAG_V, 0 };

/* Some more tables; initialised in z80_init_tables() */

libspectrum_byte sz53_table[0x100]; /* The S, Z, 5 and 3 bits of the index */
libspectrum_byte parity_table[0x100]; /* The parity of the lookup value */
libspectrum_byte sz53p_table[0x100]; /* OR the above two tables together */

/* This is what everything acts on! */
processor z80;

static void z80_init_tables(void);

/* Set up the z80 emulation */
void z80_init(void)
{
  z80_init_tables();
}

/* Initalise the tables used to set flags */
static void z80_init_tables(void)
{
  int i,j,k;
  libspectrum_byte parity;

  for(i=0;i<0x100;i++) {
    sz53_table[i]= i & ( FLAG_3 | FLAG_5 | FLAG_S );
    j=i; parity=0;
    for(k=0;k<8;k++) { parity ^= j & 1; j >>=1; }
    parity_table[i]= ( parity ? 0 : FLAG_P );
    sz53p_table[i] = sz53_table[i] | parity_table[i];
  }

  sz53_table[0]  |= FLAG_Z;
  sz53p_table[0] |= FLAG_Z;

}

/* Reset the z80 */
void
z80_reset( void )
{
  AF =BC =DE =HL =0;
  AF_=BC_=DE_=HL_=0;
  IX=IY=0;
  I=R=R7=0;
  SP=PC=0;
  IFF1=IFF2=IM=0;
  z80.halted=0;

  z80.interrupts_enabled_at = -1;
}

/* Process a z80 maskable interrupt */
int
z80_interrupt( void )
{
  /* An interrupt will occur if IFF1 is set and the /INT line hasn't
     gone high again. On a Timex machine, we also need the SCLD's
     INTDISABLE to be clear */ /*PSECTRUM -> NO TIMEX SUPPORT */
  if( IFF1 && tstates < 48 /*&& !scld_last_dec.name.intdisable*/ ) {

    /* If interrupts have just been enabled, don't accept the interrupt now,
       but check after the next instruction has been executed */
    if( tstates == z80.interrupts_enabled_at ) {
      event_add( tstates + 1, EVENT_TYPE_INTERRUPT );
      return 0;
    }

    if( z80.halted ) { PC++; z80.halted = 0; }
    
    IFF1=IFF2=0;

    writebyte( --SP, PCH ); writebyte( --SP, PCL );

//    R++; rzx_instructions_offset--;

    switch(IM) {
      case 0: PC = 0x0038; tstates += 6; break;
      case 1: PC = 0x0038; tstates += 7; break;
      case 2: 
	{
	  libspectrum_word inttemp=(0x100*I)+0xff;
	  PCL = readbyte(inttemp++); PCH = readbyte(inttemp);
	  tstates += 7;
	  break;
	}
      default:
	ui_error( UI_ERROR_ERROR, "Unknown interrupt mode %d", IM );
	fuse_abort();
    }

    return 1;			/* Accepted an interrupt */

  } else {

    return 0;			/* Did not accept an interrupt */

  }
}

/* Process a z80 non-maskable interrupt */
void
z80_nmi( void )
{
  /* FIXME: what happens if the z80 is HALTed? */

  IFF1 = 0;

  writebyte( --SP, PCH ); writebyte( --SP, PCL );

  /* FIXME: how is R affected? */

  /* FIXME: how does contention apply here? */
  tstates += 11; PC = 0x0066;
}

/* Routines for transferring the Z80 contents to and from snapshots */
int
z80_from_snapshot( libspectrum_snap *snap )
{
  A  = libspectrum_snap_a ( snap ); F  = libspectrum_snap_f ( snap );
  A_ = libspectrum_snap_a_( snap ); F_ = libspectrum_snap_f_( snap );

  BC  = libspectrum_snap_bc ( snap ); DE  = libspectrum_snap_de ( snap );
  HL  = libspectrum_snap_hl ( snap ); BC_ = libspectrum_snap_bc_( snap );
  DE_ = libspectrum_snap_de_( snap ); HL_ = libspectrum_snap_hl_( snap );

  IX = libspectrum_snap_ix( snap ); IY = libspectrum_snap_iy( snap );
  I  = libspectrum_snap_i ( snap ); R = R7 = libspectrum_snap_r( snap );
  SP = libspectrum_snap_sp( snap ); PC = libspectrum_snap_pc( snap );

  IFF1 = libspectrum_snap_iff1( snap ); IFF2 = libspectrum_snap_iff2( snap );
  IM = libspectrum_snap_im( snap );

  z80.halted = libspectrum_snap_halted( snap );

  z80.interrupts_enabled_at =
    libspectrum_snap_last_instruction_ei( snap ) ? tstates : -1;

  return 0;
}
  
int
z80_to_snapshot( libspectrum_snap *snap )
{
  libspectrum_byte r_register;

  r_register = ( R7 & 0x80 ) | ( R & 0x7f );

  libspectrum_snap_set_a  ( snap, A   ); libspectrum_snap_set_f  ( snap, F   );
  libspectrum_snap_set_a_ ( snap, A_  ); libspectrum_snap_set_f_ ( snap, F_  );

  libspectrum_snap_set_bc ( snap, BC  ); libspectrum_snap_set_de ( snap, DE  );
  libspectrum_snap_set_hl ( snap, HL  ); libspectrum_snap_set_bc_( snap, BC_ );
  libspectrum_snap_set_de_( snap, DE_ ); libspectrum_snap_set_hl_( snap, HL_ );

  libspectrum_snap_set_ix ( snap, IX  ); libspectrum_snap_set_iy ( snap, IY  );
  libspectrum_snap_set_i  ( snap, I   );
  libspectrum_snap_set_r  ( snap, r_register );
  libspectrum_snap_set_sp ( snap, SP  ); libspectrum_snap_set_pc ( snap, PC  );

  libspectrum_snap_set_iff1( snap, IFF1 );
  libspectrum_snap_set_iff2( snap, IFF2 );
  libspectrum_snap_set_im( snap, IM );

  libspectrum_snap_set_halted( snap, z80.halted );
  libspectrum_snap_set_last_instruction_ei(
    snap, z80.interrupts_enabled_at == tstates
  );

  return 0;
}
