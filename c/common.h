#ifndef _common_h_
#define _common_h_
/*

    Traveller-Trader: a space trader game
    Copyright (C) 2022 Robert Eaglestone

    This file is part of Traveller-Trader.
        
    Traveller-Trader is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
        
    Traveller-Trader is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Traveller-Trader.  If not, see <https://www.gnu.org/licenses/>.

*/      
typedef unsigned char byte;

#define  GAME_VERSION     "0.85"

#define  PETSCII_BALL      209
#define  PETSCII_DIAMOND   218
#define  PETSCII_SAIL1     223
#define  PETSCII_SAIL2     169
#define  PETSCII_SQUARE    172

#define  STAR_TRADER_ASTERISK_CHARACTER     PETSCII_DIAMOND

void common_loadCharacterSet(char* name);

void setBank(int bank);
int  getBank();

byte parsecDistance(
        unsigned char col1, 
 		unsigned char row1,
		unsigned char col2,
		unsigned char row2 );
        
void redline();
void greenline();
void titleLine();
void statusLine();

void toDefaultColor();
//void loadFile(char* name, unsigned address);
void loadFileToBank(char* name, byte bankNum, unsigned address);
//void down(byte count);
//void left(byte count);
char pressReturnAndClear();
byte diceRoll2d();
byte diceRoll3d();
byte diceRoll(byte dice, char dm);


// PLAYER STATE BANK CONTENTS
//
// Item            Address
// --------------- -----------------
// Ship Icon       $b000 - $bfff
//
#define PLAYER_STATE_BANK       1

// TEXT BANK 1 CONTENTS
//
// Item            Address
// --------------  ----------------
// Component list  $a000 - $a15f    16 chars each
//
#define TEXT_BANK_1             6
#define TEXT_SHIP_BASE_ADDRESS  0xa000
#define TEXT_SHIP_COMPONENT(x)  ((char*) 0xa000 + x * 16)
#define TEXT_SHIP_DAMAGE(x)     ((char*) 0xa168 + x * 34)

// MISC BANK CONTENTS
//
// Item            Address
// --------------  ----------------
// Mission Labels   $a000 - 
// Title            $a100 - $a3d8
// Hexgrid          $ad00 - $af60
// Name moras       $b000 - $b240   576 bytes
// Expository Intro $b245 - $b2ef   170 bytes
// Astrogation Help $b2fa - $b34d   77 bytes
// BT-ADVICE        $b400 - $bbff   2048 bytes, 64 chars each
//
#define MISC_BANK		         7
#define PETSCII_JUMP_GRID        0xad00
#define PETSCII_NAME_MORAS       0xb000
#define PETSCII_INTRO            0xb245
#define PETSCII_ASTROGATION_HELP 0xb2fa
#define PETSCII_ADVICE           0xb400

// trade and trig share a bank
#define TRADE_MATRIX_BANK       8

// SHIP BANK CONTENTS
// 
// Item             Address          Notes
// ---------------  -------------    --------------------------------------------
// Mission Labels   $a000 - $a12f    12 bytes, 25 entries (a-y).
// Owner Labels     $a130 - $a26f    12 bytes, 26 entries (a-z).
// Hull configs     $a270 - $a2df    16 bytes, 7 entries.
// Emplacements     $a2e0 - $a31f    8 bytes, 8 entries.
// Weapon Labels    $a320 - $a40f    12 bytes, 20 entries.
// Problems         $a410 - $a6cf    32 bytes, 22 entries (one per component).
// Ship count       $a710
// Ship Designs     $a720 - $bfff    48 bytes, max = 132 ships.
//
#define SHIP_BANK       	        9
#define SHIP_MISSION_LABEL(x)       ((char*)(0xa000 + (int)(x-65) * 12))
#define SHIP_OWNER_LABEL(x)         ((char*)(0xa130 + (int)(x-65) * 12))
#define SHIP_CFG_LABEL(x)           ((char*)(0xa270 + x * 16))
#define SHIP_EMPLACEMENT_LABEL(x)   ((char*)(0xa2e0 + x * 8))
#define SHIP_WEAPON_LABEL(x)        ((char*)(0xa320 + x * 12))
#define SHIP_PROBLEM(x)             ((char*)(0xa410 + x * 32))
#define SHIP_DESIGN_COUNT           0xa710
#define SHIP_DESIGNS_BEGIN          0xa720

// the map takes up many banks
#define MAP_BANK_BEGIN		    10

#define ADDRESS_START	0xA000

#endif
