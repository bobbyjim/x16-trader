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

#ifndef bcpm_rand_h
#define bcpm_rand_h

typedef unsigned long u4;
typedef struct ranctx 
{ 
   u4 a; 
   u4 b;  
   u4 c; 
   u4 d; 
   unsigned char initialized;
} ranctx;


u4 ran32val( ranctx *x );
void ran32init( ranctx *x, u4 seed );
int burtle32_srand(u4 seed);
unsigned long burtle32(); // "burtle32_random"

#endif
