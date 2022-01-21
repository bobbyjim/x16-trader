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

#include <conio.h>
#include <cbm.h>
#include <peekpoke.h>
#include <stdlib.h>

#include "common.h"

#define LOGICAL_FILE_NUM        1
#define IGNORE_LFN              0
#define SD_CARD                 8
#define EMULATOR_FILE_SYSTEM    8
#define HOST_FILE_SYSTEM        1
#define SA_IGNORE_HEADER        0
#define LOAD_FLAG               0

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

void loadFile(char* name, byte bankNum)
{  
   cbm_k_setnam(name);
   cbm_k_setlfs(IGNORE_LFN,EMULATOR_FILE_SYSTEM,SA_IGNORE_HEADER);
   POKE(0x9f61, bankNum); // r38
   POKE(0,bankNum);       // r39+
   cbm_k_load(LOAD_FLAG, 0xa000);
}

void loadFileAtB800(char* name, byte bankNum)
{
   cbm_k_setnam(name);
   cbm_k_setlfs(IGNORE_LFN,EMULATOR_FILE_SYSTEM,SA_IGNORE_HEADER);
   POKE(0x9f61, bankNum); // r38
   POKE(0,bankNum);       // r39+
   cbm_k_load(LOAD_FLAG, 0xb800);
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
