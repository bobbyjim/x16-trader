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
#include <conio.h>
#include <stdio.h>

#include "shipyard.h"
#include "ship.h"
#include "common.h"
#include "alarm.h"
#include "world.h"

extern Starship ship; // your ship
extern long hcr;      // your cash
extern World current;

byte starshipCount;

int getNextShip(int curIndex, char a)
{
   Starship* ships = (Starship*) 0xa040;
   setBank(SHIP_BANK);

   while(++curIndex < STARSHIP_COUNT)
      if (ships[curIndex].allegiance == a)
         return curIndex;

   return -1;
}

int getPrevShip(int curIndex, char a)
{
   Starship* ships = (Starship*) 0xa040;
   setBank(SHIP_BANK);

   while(--curIndex > 0)
      if (ships[curIndex].allegiance == a)
         return curIndex;

   return -1;
}

byte showStarships()
{
   int index = getNextShip(-1, current.data.allegiance);

   while(1)
   {
      clrscr();
      titleLine();
      setBank(SHIP_BANK);
      textcolor(COLOR_LIGHTBLUE);

      if (index == 255)
         return 255;

      ship_describe( &STARSHIP_DATA[index] );

      cputsxy(15,40, "<up>/<down> to scroll through ship list");
      cputsxy(15,42, "<space> to consider a ship");
      cputsxy(15,44, "<return> to exit shipyard");

      switch(cgetc())
      {
         case 0x11: 
         case 0x1d:
            index = getNextShip(index, current.data.allegiance);
            if (index == -1) 
               index = getPrevShip(STARSHIP_COUNT, current.data.allegiance);
            break;

         case 0x91:
         case 0x9d: 
            index = getPrevShip(index, current.data.allegiance);
            if (index == -1)
               index = getNextShip(-1, current.data.allegiance);
//            if (index > 0) // && shipIsNotHere(index-1, current.data.allegiance)) 
  //             --index;
            break;

         case 32:
            return index;

         case 13:
            return 255; // exit
      }
   }
}


void landAtShipyard()
{
   byte index = showStarships();

   if (index == 255) return;

   cputs("\r\n\r\n     do you wish to buy this ship?\r\n\r\n");

   switch(cgetc())
   {
      case 'y': // congrats!
         readShip( index, &ship );
         cputs("congratulations!\r\n\r\n");
         cputs("press <return> to continue.\r\n\r\n");
         cgetc();
         break;

      default:
         break; // no
   }
}
