/*

    Traveller-Trader: a space trader game
    Copyright (C) 2021 Robert Eaglestone

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
#include <stdint.h>
#include <conio.h>
#include <cbm.h>
#include <peekpoke.h>

#include "common.h"

#define LOGICAL_FILE_NUM        1
#define IGNORE_LFN              0
#define SD_CARD                 8
#define EMULATOR_FILE_SYSTEM    8
#define HOST_FILE_SYSTEM        1
#define SA_IGNORE_HEADER        0
#define LOAD_FLAG               0

int currentBank = -1;

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

byte parsecDistance(unsigned char col1,
   unsigned char row1,
	unsigned char col2,
	unsigned char row2)
{
   int aa, ab, da, db, d;

   aa = row1 + (col1/2);
   ab = row2 + (col2/2);
   da = abs(aa-ab);
   db = abs(col1-col2);
   d = abs(aa - ab - col1 + col2);
   if ((da >= db) && (da >= d)) d = da;
   if ((db >= da) && (db >= d)) d = db;
   return (byte)(d & 0xff);
}

void hr(byte color)
{
   int x=81;
   textcolor(color);
   while(--x)
   {
      //cbm_k_bsout(HORIZ_BAR);
      cputc(HORIZ_BAR);
   }
   cprintf("\r\n");
//   cbm_k_bsout(0x0d);
//   toDefaultColor();
}

void redline()
{
   hr(COLOR_RED);
   toDefaultColor();
}

void titleLine()
{
    gotoxy(0,3);
    redline();
}

void statusLine()
{
   gotoxy(0,56);
   redline();
}

void toDefaultColor()
{
//   cbm_k_bsout(COLOR_GREEN); 
   textcolor(COLOR_GREEN);
}

//
//  Loads a file into RAM.
//
void loadFile(char* name, unsigned address)
{
   cbm_k_setnam(name);
   cbm_k_setlfs(IGNORE_LFN,EMULATOR_FILE_SYSTEM,SA_IGNORE_HEADER);
   cbm_k_load(LOAD_FLAG, address);
}

//
//  Loads a file into banked RAM at (0xa000 - 0xbfff).
//
void loadFileToBank(char* name, byte bankNum, unsigned address)
{  
   POKE(0x9f61, bankNum); // r38
   POKE(0,bankNum);       // r39+
   loadFile(name, address);
   // cbm_k_setnam(name);
   // cbm_k_setlfs(IGNORE_LFN,EMULATOR_FILE_SYSTEM,SA_IGNORE_HEADER);
   // cbm_k_load(LOAD_FLAG, address);
}

void down(byte count)
{
   for(;count>0;--count) cbm_k_bsout(17);
}

void left(byte count)
{
   for(;count>0;--count) cbm_k_bsout(157);
}

char pressReturnAndClear()
{
   char c;

   cputsxy(3,58,"press <return> to continue");
   c = cgetc();
   clrscr();
   return c;
}

byte diceRoll2d()
{
   return (rand() % 6)
        + (rand() % 6)
        + 2;
}
