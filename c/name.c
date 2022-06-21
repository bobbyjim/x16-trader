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

#include "common.h"
#include "name.h"

//
//  Name fragments start at $b000 of MISC_BANK
//
#define NAME_FRAGMENT_ADDRESS   0xb000
#define NAME_FRAGMENT(x)        ((char*)(0xb000 + x * 3))

char* name_generate(char* buf)
{
    char* row;
    int j;

    int bufpos = 0;
    int syl = 2 + rand() % 2;

    setBank(MISC_BANK);
    while(--syl)
    {
        row = NAME_FRAGMENT( rand() % 192 );
        for(j=0; j<3; ++j)
        {
           if (row[j] != ' ')
              buf[bufpos++] = row[j];
        }
    }
    buf[bufpos] = 0;
    return buf;
}
