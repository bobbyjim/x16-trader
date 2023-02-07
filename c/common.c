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

#include <stdlib.h>
#include <conio.h>
#include <peekpoke.h>

#include "common.h"
#include "burtle_jsf32.h"

#define  TITLE_LINE_Y         56
#define  STATUS_LINE_Y        0

#define LOGICAL_FILE_NUM        1
#define IGNORE_LFN              0
#define SD_CARD                 8
#define EMULATOR_FILE_SYSTEM    8
#define HOST_FILE_SYSTEM        1
#define SA_IGNORE_HEADER        0
#define LOAD_FLAG               0

//
// #define X16_R39
// #undef  X16_R38
// and so on

#define  CHARACTER_MAP_ADDRESS_R38    0x0f800
#define  CHARACTER_MAP_ADDRESS_R39    0x0f000
#define  LOAD_SECONDARY_ADDRESS_R38   2
#define  LOAD_SECONDARY_ADDRESS_R39   3

#define  CHARACTER_MAP_ADDRESS         CHARACTER_MAP_ADDRESS_R39
#define  LOAD_SECONDARY_ADDRESS        LOAD_SECONDARY_ADDRESS_R39

int currentBank = -1;

extern byte playerAchievementLevel; // from main.c

byte parsecDistance(byte col1,
   byte row1,
	byte col2,
	byte row2)
{
   byte aa = row1 + (col1/2);
   byte ab = row2 + (col2/2);
   byte da = abs(aa-ab);
   byte db = abs(col1-col2);
   byte d  = abs(aa - ab - col1 + col2);

   //
   //  e.g. 1810 -> 1910
   //

   //
   //  At some point I had separated the variable decl from the assignments,
   //  probably to debug, but now that it's working I can put them back.
   //
   //aa = row1 + (col1/2);            
   //ab = row2 + (col2/2);            
   //da = abs(aa-ab);                 
   //db = abs(col1-col2);             
   //d = abs(aa - ab - col1 + col2);  

   if ((da >= db) && (da >= d)) d = da;
   if ((db >= da) && (db >= d)) d = db;
   return (byte)(d & 0xff);
}

void redline()
{
   textcolor(COLOR_RED);
   chline(80);
}

void greenline()
{
   textcolor(COLOR_GREEN);
   chline(80);
}

void titleLine()
{
    textcolor(COLOR_RED);
    chlinexy(0,TITLE_LINE_Y,80);
}


void statusLine()
{
   textcolor(COLOR_GRAY2);
   chlinexy(0,STATUS_LINE_Y,80);
   gotoxy(1,STATUS_LINE_Y);
   cprintf(" %s ", GAME_VERSION);
   cputsxy(22,STATUS_LINE_Y," t r a v e l l e r   t r a d e r ");
   gotox(68);
   cprintf(" r%-3u :%u ", playerAchievementLevel, _heapmemavail());
}

void toDefaultColor()
{
   textcolor(COLOR_LIGHTBLUE);
}

//
//  Loads a file into RAM.
//
//void loadFile(char* name, unsigned address)
//{
//   cbm_k_setnam(name);
//   cbm_k_setlfs(IGNORE_LFN,EMULATOR_FILE_SYSTEM,SA_IGNORE_HEADER);
//   cbm_k_load(LOAD_FLAG, address);
//}

//
//  Loads a file into banked RAM at (0xa000 - 0xbfff).
//
void loadFileToBank(char* name, byte bankNum, unsigned address)
{  
   POKE(0x9f61, bankNum); // r38
   POKE(0,bankNum);       // r39+
    cbm_k_setnam(name);
    cbm_k_setlfs(IGNORE_LFN,EMULATOR_FILE_SYSTEM,SA_IGNORE_HEADER);
    cbm_k_load(LOAD_FLAG, address);
}

char pressReturnAndClear()
{
   char c;

   textcolor(COLOR_YELLOW);
   cputsxy(27,50,"press <return> to continue");
   c = cgetc();
   clrscr();
   return c;
}

byte diceRoll2d()
{
   return (burtle32() % 6)
        + (burtle32() % 6)
        + 2;
}

byte diceRoll3d()
{
   return (burtle32() % 6)
        + (burtle32() % 6)
        + (burtle32() % 6)
        + 3;
}

byte diceRoll(byte dice, char dm)
{
   while(dice--)
   {
      dm += 1 + (burtle32() % 6);
   }
   return dm;
}

void common_loadCharacterSet(char* filename)
{
   cbm_k_setnam(filename);
   cbm_k_setlfs(0,8,0);
   cbm_k_load(LOAD_SECONDARY_ADDRESS, CHARACTER_MAP_ADDRESS);
}

void setBank(int bank)
{
   if (bank < 1) bank = 1;

   if (currentBank != bank)
   {
      POKE(0x9f61, bank); // r38
      POKE(0,bank);       // r39+
      currentBank = bank;
   }
}

int getBank()
{
   return currentBank;
}
