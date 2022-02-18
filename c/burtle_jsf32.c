/*
  
    8SH: a command interpreter for 8 bit 'retro' systems.
    Copyright (C) 2020 Robert Eaglestone

    This file is part of 8SH.

    8SH is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    8SH is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 8SH.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <conio.h>
#include <time.h>
#include "burtle_jsf32.h"

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

u4 burtle32_random()
{
   if ( rrgenerator.initialized == 0 )
      burtle32_srand(clock());

   return ran32val(&rrgenerator); // / 0xffffffff; 
}

// void main() // int argc, char* argv[])
// {
//    int i = 100;
//    burtle_srand(100);
//    while(--i > 0)
//    {
//       int r = burtle_random();
//       cprintf("%d\n", r);
//    }
// }

