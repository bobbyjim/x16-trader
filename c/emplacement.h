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

#ifndef _EMPLACEMENT_H_
#define _EMPLACEMENT_H_

#include "common.h"

char* getEmplacementName(byte encoded);
char* getWeaponName(byte encoded);
unsigned char getMinHits(byte encoded);
unsigned char getMaxHits(byte encoded);
unsigned char getHits(byte encoded);
unsigned char hitsInNextTurn(byte encoded);

#endif
