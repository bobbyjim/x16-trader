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

#include <stdint.h>
#include <peekpoke.h>
#include <stdio.h>

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

////////////////////////////////////////////////////////////////////
//
//  This is designed to work sort of like fgets.
//
////////////////////////////////////////////////////////////////////
int bankgets(char* toBuffer, int maxlen, int startpos)
{
   uint8_t i;
   for(i=0; i<maxlen; ++i)
   {
      char c = PEEK(0xa000 + startpos + i);
      toBuffer[i] = c;
      if (c=='\0') break;
   }
   toBuffer[maxlen] = '\0'; // EOL
   return i;
}

void bankputs(const char* string, int len, int startpos)
{
   int i;
   for(i=0; i<len; ++i)
   {
      POKE(0xa000 + startpos + i, string[i]);
   }
   POKE(0xa000 + startpos + len, 0);  // EOF
}

char beek(int position)
{
   return (char) PEEK(0xa000 + position);
}

char beeknext(int position)
{
   return beek(position + 1); // '\0' on EOF is automatic
}

int memcmpBank(int sourcepos, const char* string, int length)
{
   int i;
   for(i=0; i<length; i++)
   {
      int res = (string[i] != beek(sourcepos + i));
      if (res != 0)
         return res;
   }
   return 0;
}

void bankputbyte(uint8_t b, int pos)
{
   POKE(0xa000 + pos, b);
}

void bankputint(int b, int pos)
{
   POKE(0xa000 + pos, b & 0xff);
   POKE(0xa001 + pos, b >> 8);
}

int bankgetbyte(int pos)
{
   return PEEK(0xa000 + pos);
}

int bankgetint(int pos)
{
   return PEEK(0xa000 + pos)
        + (PEEK(0xa001 + pos) << 8);
}

