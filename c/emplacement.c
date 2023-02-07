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
#include "emplacement.h"

char* weaponName[32] = {
    "-",
    "-",
    "mining las",       
    "pulse las"  ,       
    "beam las"    ,      
    "plasma gun"     ,     
    "fusion gun"      ,    
    "salvo rack"       ,   
    "missile"           ,  
    "k-missile"         , 
    "a-missile"          ,
    "damper"         ,
    "tractor"     ,
    "inducer"             ,
    "disruptor"           ,
    "stasis"              ,
    "sandcaster"          ,
    "hybrid lsm"        ,
    "pa",
    "meson gun"               
};

char* getEmplacementName(byte encoded)
{
    if (encoded == 0) return "-";

    switch(encoded>>5)
    {
        case 0: return "t1";
        case 1: return "t2";
        case 2: return "t3";
        case 3: return "b1";
        case 4: return "b2";
        case 5: return "bay";
        case 6: return "lbay";
        case 7: return "main";
    }
    return "-";
}

char* getWeaponName(byte encoded)
{
    return weaponName[encoded & 31];
}

//                           T1 T2 T3 B1  B2   B  LB   M 
unsigned char minHits[8] = {  1, 1, 1, 0,  0,  0,  0,  0 };
unsigned char maxHits[8] = {  1, 2, 3, 5,  8, 15, 20, 30 };

unsigned char getMinHits(byte encoded)
{
    if ((encoded & 31) == 0) return 0;
    return minHits[ encoded >> 5 ];
}

unsigned char getMaxHits(byte encoded)
{
    if ((encoded & 31) == 0) return 0;
    if ((encoded & 31) >= 7 && (encoded & 31) <= 10) return maxHits[ encoded >> 5] * 3;
    return maxHits[ encoded >> 5 ];
}

unsigned char getHits(byte encoded)
{
   unsigned char hits = rand() % getMaxHits(encoded);
   unsigned char minHits = getMinHits(encoded);
   if (hits < minHits)
      return minHits;
   return hits;
}

unsigned char hitsInNextTurn(byte encoded)
{
    int value = encoded & 31;
    if (value >= 7 && value <= 10 ) return 1;
    return 0;
}
