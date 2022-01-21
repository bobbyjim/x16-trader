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

#ifndef _hexmap_h_
#define _hexmap_h_

#include "common.h"

typedef struct 
{
   byte row;
   byte col;
   byte distance;
} Hex;

void putstring(char* str);

void topRow(unsigned char x, unsigned char y);
void middle(unsigned char x, unsigned char y);
void middleConnector(unsigned char x, unsigned char y);
void endConnector(unsigned char x, unsigned char y);
void bottomRow(unsigned char x, unsigned char y);

void drawHexmap();
void showWorlds(unsigned char col, unsigned char row);
byte pickDestination(unsigned char col, unsigned char row, unsigned char range);


#endif
