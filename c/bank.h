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

#ifndef _bank_h
#define _bank_h

#include <stdint.h>

void setBank(int bank);
int  getBank();

int bankgets(char* toBuffer, int len, int pos);
int bankgetbyte(int pos);
int bankgetint(int pos);

void bankputs(const char* string, int len, int pos);
void bankputbyte(uint8_t b, int pos);
void bankputint(int b, int pos);

char beek(int position);
char beeknext(int position);
int memcmpBank(int sourcepos, const char* string, int length);

#endif
