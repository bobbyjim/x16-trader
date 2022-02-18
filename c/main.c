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

#include <stdlib.h>
#include <string.h>
#include <cbm.h>
#include <time.h>
#include <conio.h>
#include <cx16.h>

#include "alarm.h"
#include "common.h"
#include "hold.h"
#include "jump.h"
#include "passengers.h"
#include "trade.h"
#include "ship.h"
#include "world.h"
#include "shipyard.h"
#include "starport.h"
#include "hiring_hall.h"
#include "jump-map.h"
#include "sprite.h"
#include "maneuver-map.h"
#include "console.h"

//
//  Player data
//
byte              playerAchievementLevel = 1; 
Starship          ship;
byte              shipState[22];
byte              shipDamage[22];
long hcr 	      = 5000; // in hundreds of cr (because of how trade works)
long mortgage_cr  = 0;    

word hold  	      = 64;   // should actually be ship.cargo, but whatever
Cargo cargo[20];

//
//  These are the only skills that matter.
//
byte admin	      = 0;
byte astrogator   = 0;
byte engineer     = 0;
byte gunner       = 0;
byte medic        = 0;
byte pilot        = 0;
byte steward      = 0;
byte streetwise   = 0;

//
//   World data
//
World current;
World destination;
byte  distance;
Cargo starport[20];
Cargo temp[20];

//
//   system data
//
World localSystem[14];
byte  localSystemCount;

//
//  Iterate through the crew, noting the highest useful skill levels.
//
void updateShipSkills()
{
   admin      = findHighestSkillFor(SKILL_ADMIN);
   astrogator = findHighestSkillFor(SKILL_ASTROGATOR);
   engineer   = findHighestSkillFor(SKILL_ENGINEER);
   gunner     = findHighestSkillFor(SKILL_GUNNER);
   medic      = findHighestSkillFor(SKILL_MEDIC);
   pilot      = findHighestSkillFor(SKILL_PILOT);
   steward    = findHighestSkillFor(SKILL_STEWARD);
   streetwise = findHighestSkillFor(SKILL_STREETWISE);
}

SpriteDefinition shipicon, worldicon, jamison;

void shipiconShow(int x, int y)
{
   shipicon.block =  0x4000;
   shipicon.mode  =  SPRITE_MODE_8BPP;
   shipicon.layer =  SPRITE_LAYER_1;
   shipicon.dimensions = SPRITE_64_BY_64;
   shipicon.x     = SPRITE_X_SCALE(x);
   shipicon.y     = SPRITE_Y_SCALE(y);

   sprite_define(1, &shipicon);
}

void shipiconHide()
{
   shipicon.layer = SPRITE_DISABLED;
   sprite_define(1, &shipicon);
}

// void worldiconShow()
// {
//    worldicon.block =  0x6000;
//    worldicon.mode  =  SPRITE_MODE_8BPP;
//    worldicon.layer =  SPRITE_LAYER_BACKGROUND;
//    worldicon.dimensions = SPRITE_32_BY_32;
//    worldicon.x     = SPRITE_X_SCALE(363);
//    worldicon.y     = SPRITE_Y_SCALE(240);

//    sprite_define(2, &worldicon);
// }

void jamisonShow()
{
   jamison.block =  0x5000;
   jamison.mode  =  SPRITE_MODE_8BPP;
   jamison.layer =  SPRITE_LAYER_1;
   jamison.dimensions = SPRITE_64_BY_64;
   jamison.x     = SPRITE_X_SCALE(440);
   jamison.y     = SPRITE_Y_SCALE(70);

   sprite_define(3, &jamison);
}

void jamisonHide()
{
   jamison.layer = SPRITE_DISABLED;
   sprite_define(3, &jamison);
}

char *expository_text[] = {
   "congratulations, captain!  you have full ownership of your",
   "very own type a2 marava-class far trader.",
   "",
   "earn your fortune by jumping from system to system."
   "",
   "good luck!",
   "END"
};

//
//   print splash screen
//
void splash()
{
   int i;

   cbm_k_bsout(CH_BLACK);
   cbm_k_bsout(TO_BACKGROUND);
   clrscr();

   // load PET font
   cbm_k_bsout(0x8E); // revert to primary case
   cbm_k_setnam("petfont.bin");
   cbm_k_setlfs(0,8,0);
   cbm_k_load(2, 0x0f800);

   //
   //  Print the banner
   //
   loadFileToBank("bt-misc.bin",  MISC_BANK, 0xa000);
   loadFileToBank("bt-title.bin", MISC_BANK, 0xa100);
   titleLine();
   setBank(MISC_BANK); 
   textcolor(COLOR_CYAN);

   for(i=0; i<14; ++i)
      cputsxy(15, i+4, ((char*) 0xa100 + i*52));

   textcolor(COLOR_LIGHTBLUE);

   for(i=0;; ++i)
   {
      if (!strcmp(expository_text[i], "END"))
         break;

      gotoxy(10,22+i*2);
      cputs(expository_text[i]);
   }

   textcolor(COLOR_YELLOW);
   cputsxy(5,26 + i*2,"                       press <space> to begin");

   jamisonShow();
   i = cgetc();
   _randomize(); // while we're here.
   jamisonHide();

   //
   //  shortcut the hands-on mode
   //
   if (i == 'x')
      playerAchievementLevel = 10;
}

void init()
{
   byte i;

   loadFileToBank("bt-hexgrid.bin", MISC_BANK,         JUMP_GRID_ADDRESS);
   loadFileToBank("bd-market.bin",  TRADE_MATRIX_BANK, 0xa000);
   loadFileToBank("bd-ships.bin",   SHIP_BANK,         0xa000);
   loadFileToBank("bd-map64.bin",  MAP_BANK_BEGIN,    0xa000);

   sprite_loadToVERA("aia-far.bin",  0x4000);
   sprite_loadToVERA("bi-jamison.bin", 0x5000);
   sprite_loadToVERA("bi-worlds.bin",  0x6000);
   vera_sprites_enable(1); // cx16.h

   ship_init(&ship);
}

void burnFuel()
{
   shipState[ O_STATE_JUMP_FUEL_USED ] += distance;
   if (shipState[ O_STATE_JUMP_FUEL_USED ] > 0) // ship.component[ O_QSP_J ])
      shipState[ O_STATE_JUMP_FUEL_USED ] = STATUS_LOW;
}

void main() 
{
   int range;
   int i;

   init();
   splash();

   //
   //  Start at Regina
   //
   current.col = destination.col = 19;
   current.row = destination.row = 10;

   getWorld(&current);

   for(;;)
   {
      // bookkeeping required to handle fuel consumption.
      range = ship.component[ O_QSP_J ] - shipState[ O_STATE_JUMP_FUEL_USED ];
      showCurrentLocation();
      switch(maneuver())
      {
         case ASTROGATION_OPTION:
            //
            // Astrogation
            //
            titleLine();

            printAlarmBar();
            
            jumpmapShow();
            jumpmapShowWorldData(current.col,current.row);
            shipiconShow(335, 235);
            i = jumpmapSetDestination();
            shipiconHide();
            if (playerAchievementLevel < 2)
               playerAchievementLevel = 2;
            if (i != 'j') break;
            // fall through for immediate jump

	      case JUMP_OPTION:
		      if ((destination.row != current.row) || (destination.col != current.col))
		      {
 		         bookPassengers();
	    	      jump();
               distance = parsecDistance(current.col, current.row, destination.col, destination.row);
		         burnFuel();
      		   current.col = destination.col;
      		   current.row = destination.row;
      		   getWorld(&current);
               ++playerAchievementLevel;
		      }
		      break;

	      case MARKET_OPTION:
            trade_speculate();
		      break;

	      case STARPORT_OPTION:
      		landAtStarport();
		      break;

	      case SHIPYARD_OPTION:
		      landAtShipyard();
		      break;

	      case HIRING_HALL_OPTION:
            hire();
            updateShipSkills();
		      break;
      }
   }
}

