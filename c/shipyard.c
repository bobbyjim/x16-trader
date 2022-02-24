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
#include <peekpoke.h>

#include "shipyard.h"
#include "ship.h"
#include "common.h"
#include "alarm.h"
#include "world.h"

extern Starship ship; // your ship
extern long hcr;      // your cash

byte showStarships()
{
   byte index = 0;
   while(1)
   {
      clrscr();
      titleLine();
      setBank(SHIP_BANK);
      textcolor(COLOR_LIGHTBLUE);

      ship_describe( STARSHIP_DATA(index) );

      switch(cgetc())
      {
         case 0x11: 
           if (STARSHIP_DATA((int)(index+1))->index != 0)
               ++index; 
            break;

         case 0x91: 
            if (index > 0) 
               --index; 
            break;

         case 13:
            return index;
      }
   }
}


void landAtShipyard()
{
   byte index = showStarships();

   cputs("\r\ndo you wish to buy this ship?\r\n\r\n");

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
