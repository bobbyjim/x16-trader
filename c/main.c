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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cbm.h>
#include <peekpoke.h>
#include <time.h>
#include <conio.h>
#include <cx16.h>

#include "alarm.h"
#include "bank.h"
#include "common.h"
#include "hexmap.h"
#include "hold.h"
#include "jump.h"
#include "passengers.h"
#include "pilot.h"
#include "trade.h"
#include "ship.h"
#include "world.h"
#include "shipyard.h"
#include "starport.h"
#include "hiring_hall.h"

//
//  Player data
//
Starship          ship;
byte              shipState[22];
byte              shipDamage[22];

word hold  	      = 64;   // should actually be ship.cargo, but whatever
long hcr 	      = 5000; // in hundreds of cr (because of how trade works)
long mortgage_cr  = 0;    
Cargo cargo[20];

byte admin	    = 0;
byte astrogator = 0;
byte engineer   = 0;
byte gunner     = 0;
byte medic      = 0;
byte pilot      = 0;
byte steward    = 0;
byte streetwise = 0;

//
//   world data
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

//
//   universe data
//
void splash()
{
   int i;

   loadFile("misc.bin", MISC_BANK);   
   loadFile("market.bin", TRADE_MATRIX_BANK);
//   loadFileAtB800("trig.bin", TRIG_BANK);
   loadFile("ships.bin", SHIP_BANK);
   loadFile("spinward-map-64b.bin",MAP_BANK_BEGIN);

//   readShip(43, &ship);
//   ship_debug(&ship);

   cbm_k_bsout(CH_BLACK);
   cbm_k_bsout(TO_BACKGROUND);

   clrscr();

   cbm_k_bsout(0x8E); // revert to primary case
   
   cbm_k_setnam("petfont.bin");
   cbm_k_setlfs(0,8,0);
   cbm_k_load(2, 0x0f800);

   titleLine();

   setBank(MISC_BANK); 
   cbm_k_bsout(CH_GRAY3);
   for(i=0; i<17; ++i)
      cputsxy(0, i+5, ((char*) 0xa100 + i*80));

   cbm_k_bsout(CH_GREEN);
   cputsxy(5,35,"                       press <space> to begin");
   cgetc();
   _randomize();
}

void init()
{
   byte i;

   ship_init(&ship);

   for(i=1; i<11; ++i)
   {
      cargo[i].index = rand() % 50;
      cargo[i].tl    = rand() % 20;
      cargo[i].tons  = 8;
   }
}

void burnFuel()
{
   shipState[ O_STATE_JUMP_FUEL_USED ] += distance;
   if (shipState[ O_STATE_JUMP_FUEL_USED ] == ship.component[ O_QSP_J ])
      shipState[ O_STATE_JUMP_FUEL_USED ] = STATUS_LOW;
}

void main() 
{
   int range;

   splash();
   init();

   current.col = 19;
   current.row = 10;
   getWorld(&current);

   for(;;)
   {
      range = ship.component[ O_QSP_J ] - shipState[ O_STATE_JUMP_FUEL_USED ];
      switch(doPilot())
      {
         case ASTROGATION_OPTION:
		      pickDestination(current.col,current.row,range);
		      break;

	      case MARKET_OPTION:
            moveCargo();
		      break;

	      case STARPORT_OPTION:
      		landAtStarport();
		      break;

	      case SHIPYARD_OPTION:
		      landAtShipyard(
			      current.starport,
			      current.bases,
			      current.zone,
			      current.alleg
		      );
		      break;

	      case HIRING_HALL_OPTION:
            hire();
            updateShipSkills();
		      break;

	      case JUMP_OPTION:
		      if (destination.bank > 0)
		      {
 		         bookPassengers();
	    	      jump();
		         burnFuel();
      		   current.col = destination.col;
      		   current.row = destination.row;
      		   getWorld(&current);
               destination.bank = 0; // null out
		      }
		      break;

      }

   }
}

