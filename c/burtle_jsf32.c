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

#include <conio.h>
#include <time.h>
#include "burtle_jsf32.h"

#define randrot(x,k) (((x)<<(k))|((x)>>(32-(k))))

ranctx rrgenerator;

u4 ran32val( ranctx *x ) 
{
    u4 e = x->a - randrot(x->b, 27);
    x->a = x->b ^ randrot(x->c, 17);
    x->b = x->c + x->d;
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}

void ran32init( ranctx *x, u4 seed ) 
{
    u4 i;
    x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
    for (i=0; i<20; ++i) {
        (void)ran32val(x);
    }
    x->initialized = 1;
}

int burtle32_srand(u4 seed)
{
   ran32init(&rrgenerator, seed);
   return 1;
}

unsigned long burtle32()
{
   if ( rrgenerator.initialized == 0 )
      burtle32_srand(clock());

   return ran32val(&rrgenerator); // / 0xffffffff; 
}
