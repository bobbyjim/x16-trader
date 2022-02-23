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

void setBank(int bank);
int  getBank();

byte parsecDistance(
        unsigned char col1, 
 		unsigned char row1,
		unsigned char col2,
		unsigned char row2 );
        
void redline();
void titleLine();
void statusLine();

void toDefaultColor();
void loadFile(char* name, unsigned address);
void loadFileToBank(char* name, byte bankNum, unsigned address);
void down(byte count);
void left(byte count);
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
#define PLAYER_STATE_BANK       6


// MISC BANK CONTENTS
//
// Item            Address
// --------------  ----------------
// Mission Labels   $a000 - $a0ff
// Banner           $a100 - $a64f
// Owner Labels     $a650 - $a902
// Hexgrid          $aa00 - $ac60
// Trig table       $ad00 - $ae41 more or less
//
#define MISC_BANK		        7

// trade and trig share a bank
#define TRADE_MATRIX_BANK       8
#define TRIG_BANK		        8

// ships take up 8K
#define SHIP_BANK       	    9

// the map takes up many banks
#define MAP_BANK_BEGIN		    10


#define ADDRESS_START	0xA000

// #define NW_CORNER       0x75
// #define NE_CORNER       0x69
// #define SW_CORNER       0x6a
// #define SE_CORNER       0x6b

// #define HORIZ_BAR       0xc0
// #define VERT_BAR        0xdd

// #define T_LEFT          179
// #define T_RIGHT         171

#define TO_BACKGROUND	0x01

// #define RVS_ON          18
// #define RVS_OFF         146

#endif
