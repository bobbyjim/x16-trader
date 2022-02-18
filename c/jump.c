/*

    Traveller-Trader: a space trader game
    Copyright (C) 2021 Robert Eaglestone

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

#include <cbm.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>

#include "jump.h"
#include "common.h"

extern byte astrogator;
//extern byte playerAchievementLevel;

void jump()
{
   char colors[10] = { CH_BLACK, CH_BLACK, CH_RED, CH_BLUE, CH_GREEN, CH_LIGHTRED, CH_ORANGE, CH_YELLOW };
   word days = 8;
   byte mod_astrogator = astrogator;
   word x;
  
   // if (playerAchievementLevel > 1)
   //    ++mod_astrogator;

   clrscr();
   textcolor(COLOR_LIGHTRED);
   cputsxy(5,2,"entering jumpspace.  please stand by.");
   titleLine();

   sleep(10/(mod_astrogator+1));
   
   while(--days)
   {
      x = 6000 / (mod_astrogator+1);
      while(--x)
      {
         cbm_k_bsout( colors[days] );
         switch(days)
         {
            case 4:
            case 3:
            case 2:
            case 1:
               cputcxy( 
	               rand() % 80, 
	               rand() % 60,
	               177+rand()/8192);
               break;

            //   cputcxy( 
	    //     rand() % 80, 
	    //     rand() % 60,
	    //     209+rand()/5461);
            //   break;
               
            default: 
               cputcxy( 
	         rand() % 80, 
	         rand() % 60,
	         205+rand()/16384);
               break;
         }
         
      }
   }
}

