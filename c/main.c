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
#include <cx16.h>

#include "common.h"
#include "trade.h"
#include "jump.h"
#include "trade.h"
#include "ship.h"
#include "world.h"
#include "shipyard.h"
#include "starport.h"
#include "hiring_hall.h"
#include "jump-map.h"
#include "sprite.h"
#include "maneuver-map.h"
#include "wilderness.h"
#include "menu.h"
#include "name.h"
#include "panel.h"
#include "survey.h"
#include "bankedText.h"

#define  VERSION     "0.75"

//
//  Player data
//
byte              playerAchievementLevel = 1; 
Starship          ship;
byte              shipState[SHIP_COMPONENT_COUNT];
byte              shipDamage[SHIP_COMPONENT_COUNT];
long hcr 	      = 5000; // in hundreds of cr (because of how trade works)
long mortgage_cr  = 0;    
byte pay_period;   // in days between jump.  7 = normal schedule.  insystem jaunts increase this.
Cargo cargo[20];

//
//  These are the only skills that matter.
//
byte admin	      = 0;
byte astrogator   = 0;
byte engineer     = 0;
byte gunner       = 0;
byte liaison      = 0;
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
   liaison    = findHighestSkillFor(SKILL_LIAISON);
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

#define  START_OPTION_COUNT   3
byte experienceLevel = 0;
char* startingExperienceLevel[] = {
   "new recruit", "trader", "scout"
};

void init()
{
   loadFileToBank("bt-component.bin", TEXT_BANK_1,       TEXT_SHIP_BASE_ADDRESS);
   loadFileToBank("bt-hexgrid.bin",   MISC_BANK,         PETSCII_JUMP_GRID);
   loadFileToBank("bd-name.bin",      MISC_BANK,         PETSCII_NAME_MORAS);
   loadFileToBank("bt-exposit.bin",   MISC_BANK,         PETSCII_INTRO);
   loadFileToBank("bt-advice.bin",    MISC_BANK,         PETSCII_ADVICE); // 2k
//   loadFileToBank("bd-market.bin",   TRADE_MATRIX_BANK, 0xa000);
   loadFileToBank("b3-trade.bin",     TRADE_MATRIX_BANK, 0xa000); // replaces the T5 trade matrix
   loadFileToBank("bd-ships.bin",     SHIP_BANK,         0xa000);
   loadFileToBank("bd-map64.bin",     MAP_BANK_BEGIN,    0xa000);

   sprite_loadToVERA("aia-far.bin",  0x4000);
//   sprite_loadToVERA("aig-enforc.bin",  0x4000);
//   sprite_loadToVERA("bi-worlds.bin",  0x6000);
}



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
   common_loadCharacterSet("petfont.bin");

   // load Jamison
   sprite_loadToVERA("bi-jamison.bin", 0x5000);

   //
   //  Print the Title Banner
   //
   loadFileToBank("bt-title.bin",    MISC_BANK, 0xa100); // title splash
   titleLine();
   textcolor(COLOR_CYAN);
   gotoxy(15,4);
   setBank(MISC_BANK); 
   for(i=0; i<14; ++i)
      cputsxy(15, i+4, ((char*) 0xa100 + i*52));
   cputsxy(49,16,VERSION);

   textcolor(COLOR_LIGHTBLUE);

   gotoxy(0,22);
   printBankedText(PETSCII_INTRO, 181);

   vera_sprites_enable(1); // cx16.h
   jamisonShow();

   textcolor(COLOR_YELLOW);
   drawPanel(10, 37, 58, 12, " select your experience level " );
   experienceLevel = menu_run(12, 38, START_OPTION_COUNT, startingExperienceLevel);

   playerAchievementLevel = 1 + experienceLevel * 4;
   switch (experienceLevel)
   {
      case 0: // beginner
         ship_init(SHIP_INDEX_MARAVA, &ship);
         ship.component[ O_QDP_BATTERY(0) ] = SHIP_BATTERY_T1 + SHIP_WEAPON_PULSE_LASER;
         ship.component[ O_QDP_BATTERY(1) ] = SHIP_BATTERY_T1 + SHIP_WEAPON_PULSE_LASER;
         break;

      case 1: // merchant
         ship_init(SHIP_INDEX_MARAVA, &ship);
         break;

      case 2: // scout
         ship_init(SHIP_INDEX_MURPHY, &ship);
         sprite_loadToVERA("ais-sc.bin",  0x4000);
         break;

      // case 3: // trader 7 or 8
      //    ship_init(SHIP_INDEX_BEOWULF, &ship);
      //    sprite_loadToVERA("aia-beo.bin",  0x4000);
      //    break;

      // case 4: // hunter
      //    ship_init(SHIP_INDEX_KILAALUM, &ship);
      //    sprite_loadToVERA("aie-kil.bin", 0x4000);
      //    break;
   }

   jamisonHide();
}

void main() 
{
   int range;
   int i;

   init();
   splash();

   current.col = destination.col = 19;
   current.row = destination.row = 10;

   getWorld(&current);

   for(;;)
   {
      // bookkeeping required to handle fuel consumption.
      range = ship.component[ O_QSP_J ] - shipState[ O_STATE_JUMP_FUEL_USED ];
      showCurrentLocation();
      statusLine();
      switch(maneuver())
      {
         case ASTROGATION_OPTION:
            //
            // Astrogation
            //
            jumpmapShow();
            shipiconShow(335, 235);
            i = jumpmapSetDestination();
            shipiconHide();
            if (playerAchievementLevel < 2)
               playerAchievementLevel = 2;

            if (i != 'j') break;  // capital J
            // else, fall through for immediate jump

	      case JUMP_OPTION:
  	         jump();
            trade_calculateMarketPrices();
		      break;

         case WILDERNESS_REFUEL_OPTION:
            wilderness_refuel();
            break;

         case TRADE_OPTION:
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

         case SURVEY_OPTION:
            survey();
            break;
      }
   }
}

