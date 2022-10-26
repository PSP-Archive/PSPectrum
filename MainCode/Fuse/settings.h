/* settings.h: Handling configuration settings
   Copyright (c) Copyright (c) 2001-2003 Philip Kendall

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

#include <config.h>

#include <sys/types.h>

#ifndef FUSE_SETTINGS_H
#define FUSE_SETTINGS_H

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct settings_info {

   int aspect_hint;
   int auto_load;
   int autosave_settings;
   int bw_tv;
   int competition_code;
   int competition_mode;
   int confirm_actions;
  char *dck_file;
   int embed_snapshot;
   int emulation_speed;
   int frame_rate;
   int full_screen;
  char *if2_file;
   int interface2;
   int issue2;
   int joy_kempston;
  char *joystick_1;
   int joystick_1_fire_1;
   int joystick_1_fire_10;
   int joystick_1_fire_2;
   int joystick_1_fire_3;
   int joystick_1_fire_4;
   int joystick_1_fire_5;
   int joystick_1_fire_6;
   int joystick_1_fire_7;
   int joystick_1_fire_8;
   int joystick_1_fire_9;
   int joystick_1_output;
  char *joystick_2;
   int joystick_2_fire_1;
   int joystick_2_fire_10;
   int joystick_2_fire_2;
   int joystick_2_fire_3;
   int joystick_2_fire_4;
   int joystick_2_fire_5;
   int joystick_2_fire_6;
   int joystick_2_fire_7;
   int joystick_2_fire_8;
   int joystick_2_fire_9;
   int joystick_2_output;
   int joystick_keyboard_output;
  char *playback_file;
  char *plus3disk_file;
   int printer;
  char *printer_graphics_filename;
  char *printer_text_filename;
  char *record_file;
  char *rom_128_0;
  char *rom_128_1;
  char *rom_16;
  char *rom_48;
  char *rom_pentagon_0;
  char *rom_pentagon_1;
  char *rom_pentagon_2;
  char *rom_plus2_0;
  char *rom_plus2_1;
  char *rom_plus2a_0;
  char *rom_plus2a_1;
  char *rom_plus2a_2;
  char *rom_plus2a_3;
  char *rom_plus3_0;
  char *rom_plus3_1;
  char *rom_plus3_2;
  char *rom_plus3_3;
  char *rom_plus3e_0;
  char *rom_plus3e_1;
  char *rom_plus3e_2;
  char *rom_plus3e_3;
  char *rom_scorpion_0;
  char *rom_scorpion_1;
  char *rom_scorpion_2;
  char *rom_scorpion_3;
  char *rom_spec_se_0;
  char *rom_spec_se_1;
  char *rom_tc2048;
  char *rom_tc2068_0;
  char *rom_tc2068_1;
   int rzx_compression;
   int simpleide_active;
  char *simpleide_master_file;
  char *simpleide_slave_file;
   int slt_traps;
   int snaps_as_z80;
  char *snapshot;
   int sound;
  char *sound_device;
   int sound_force_8bit;
   int sound_load;
  char *start_machine;
  char *start_scaler_mode;
   int statusbar;
   int stereo_ay;
   int stereo_beeper;
   int svga_mode;
  char *tape_file;
   int tape_traps;
  char *trdosdisk_file;
   int writable_roms;
   int zxatasp_active;
  char *zxatasp_master_file;
  char *zxatasp_slave_file;
   int zxatasp_upload;
   int zxatasp_wp;
   int zxcf_active;
  char *zxcf_pri_file;
   int zxcf_upload;

  int show_help;
  int show_version;

} settings_info;

extern settings_info settings_current;

int settings_init( int *first_arg, int argc, char **argv );
int settings_defaults( settings_info *settings );
int settings_copy( settings_info *dest, settings_info *src );

#define SETTINGS_ROM_COUNT 30
char **settings_get_rom_setting( settings_info *settings, size_t which );

int settings_set_string( char **string_setting, const char *value );

int settings_free( settings_info *settings );

int settings_write_config( settings_info *settings );

int settings_end( void );

#ifdef __cplusplus
	}
#endif

#endif				/* #ifndef FUSE_SETTINGS_H */
