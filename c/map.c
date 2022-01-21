
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

