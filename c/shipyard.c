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

byte shipIsHere(Starship* thisShip)
{
   // 
   // If this current system is IMPERIAL and has BASES, then we determine visibility by BASES ONLY.
   //
   if (current.data.allegiance == 'i' && current.data.bases != ' ')
   {
      if (thisShip->allegiance == 'n' && WORLD_HAS_NAVAL_BASE(&current)) // n 'navy'
         return 1;

      if (thisShip->allegiance == 'e' && WORLD_HAS_SCOUT_BASE(&current)) // e 'exploration'
         return 1;
   }  
   return thisShip->allegiance == current.data.allegiance; 
}

int getNextShip(int curIndex)
{
   Starship* ships = STARSHIP_DATA; // (Starship*) 0xa040;
   setBank(SHIP_BANK);

   while(++curIndex < STARSHIP_COUNT)
      if (shipIsHere(&ships[curIndex])) //.allegiance == current.data.allegiance)
         return curIndex;

   return -1;
}

int getPrevShip(int curIndex)
{
   Starship* ships = STARSHIP_DATA; // (Starship*) 0xa040;
   setBank(SHIP_BANK);

   while(--curIndex > 0)
      if (shipIsHere(&ships[curIndex])) //.allegiance == current.data.allegiance)
         return curIndex;

   return -1;
}

byte showStarships()
{
   int index = getNextShip(-1);

   while(1)
   {
      setBank(SHIP_BANK);
      clrscr();
      titleLine();
      statusLine();
      textcolor(COLOR_LIGHTBLUE);

      if (index == 255)
         return 255;

      gotoxy(4, 2);
      showShipSummary( &STARSHIP_DATA[index] );
      ship_describe( &STARSHIP_DATA[index] );

//      gotoxy(15,38);
//      cprintf("  %c / %c / %u ", STARSHIP_DATA[index].allegiance, current.data.allegiance, shipIsHere(&STARSHIP_DATA[index]));

      cputsxy(15,40, "<up>/<down> to scroll through ship list");
      cputsxy(15,42, "<space> to consider a ship");
      cputsxy(15,44, "<return> to exit shipyard");

      switch(cgetc())
      {
         case 0x11: 
         case 0x1d:
            index = getNextShip(index);
            if (index == -1) 
               index = getPrevShip(STARSHIP_COUNT);
            break;

         case 0x91:
         case 0x9d: 
            index = getPrevShip(index);
            if (index == -1)
               index = getNextShip(-1);
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
         ship_loadImageFromIndex(index);
         break;

      default:
         break; // no
   }
}
