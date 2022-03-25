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
#include "world.h"
#include "ship.h"
#include "common.h"
#include "passengers.h"
#include "piracy.h"

extern byte astrogator;
extern World current, destination;
extern byte shipState[22];
extern byte playerAchievementLevel;
extern byte pay_period;

#define     ADVICE_COUNT      16
char* advicePerPlayerAchievementLevel[] = {
   "",
   "",
   "wilderness refueling is a cheap way to refuel, but takes time.",
   "    e-class starports are simply marked landing fields.",
   "           beware of amber and red zone worlds!",
   "     hire an astrogator... these jumps take forever!",
   "       buy and sell speculative cargo at the market.",
   "         upgrade defenses at a and b starports.",
   "        a- and b-class starports have shipyards",
   "            get weapons, then hire a gunner.",
   "       buy a liner if you want longer trade legs.",
   "       buy a cruiser if you want to hunt pirates.",
   "          special systems have special ships.",
   "         time spent in mining produces cargo.",
   "       sometimes, mining asteroids produces fuel.",
   "      sometimes, mining asteroids produces salvage."
};

void burnJumpFuel()
{
   int distance = parsecDistance(current.col, current.row, destination.col, destination.row);
   shipState[ O_STATE_JUMP_FUEL_USED ] += distance; // burn fuel
}

void specialEffects()
{
   char colors[10] = { CH_BLACK, CH_BLACK, CH_RED, CH_BLUE, CH_GREEN, CH_LIGHTRED, CH_ORANGE, CH_YELLOW };
   unsigned days = 8;
   unsigned x;
  
   clrscr();
   titleLine();
   //cputsxy(5,2,"jumpspace transition");
   textcolor(COLOR_LIGHTRED);
   cputsxy(22,20,"entering jumpspace.  please stand by.");
   textcolor(COLOR_YELLOW);

   if (playerAchievementLevel < ADVICE_COUNT)
   {
      cputsxy(30,26,"** game hints **");
      cputsxy(10,30,advicePerPlayerAchievementLevel[playerAchievementLevel]);
   }

   sleep(10/(astrogator+1));
   
   while(--days)
   {
      x = 6000 / (astrogator+1);
      if (days == 1) x *= 2;

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

void updateLocation()
{
   current.col = destination.col;
   current.row = destination.row;
   getWorld(&current);
}

void jump()
{
   if ((destination.row == current.row) && (destination.col == current.col)) return;

   pay_period += 7;  // one week in jump
   bookPassengersAndPayCrew();   

   burnJumpFuel();

   specialEffects();
   updateLocation();
   ++playerAchievementLevel;
   piracy();
}

