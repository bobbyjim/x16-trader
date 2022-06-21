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

#include "map.h"

void makeMap(byte siz) // siz = 0 to 15 or so.
{
   int rows;
   int cols;
   int i,j;

   rows = siz * 3; // up to 45 chars tall
   cols = siz * 5; // up to 75 chars wide

   for(i=0; i<10; ++i)
   {
      x = 5 + rand() % (cols-10);
      y = 5 + rand() % (rows-10);

      j = rand() % (r * c) / 10;
      while(--j)
      {
         x += -1 + 2*rand()%2;
         y += -1 + 2*rand()%2;

         if (x>0 && x<cols && y>0 && y<rows)
            map[x][y] = map[x][y]+1;
      }
   }
}

