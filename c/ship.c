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

#include <conio.h>
#include <peekpoke.h>

#include "common.h"
#include "ship.h"
#include "alarm.h"

char* cfgCode  = "cbpusal";
char* hullCode = "0abcdefghjklmnpqrstuvwxyz?????????";
char* tlCode   = "0123456789abcdefghjklmnpqrstuvwxyz";
char* emplacement[] = {
   "t1", "t2", "t3", "b1", "b2", "bay", "lbay", "main"
};
char* weapon[] = {
   "none",
   "empty",
   "mining laser",   "pulse laser",       "beam laser",
   "plasma gun",     "fusion gun",
   "salvo rack",     "missile",           "kk missile",     "am missile",
   "jump damper",    "tractor/pressor",   "inducer",        "disruptor",   "stasis",
   "sandcaster",
   "hyrbid l-s-m",   "particle accelerator",    "meson gun" 

};

extern byte playerAchievementLevel;


void showShipSummary(Starship* ship)
{
   cprintf("(%c) %s %c-%c%c%u%u  %s  hi:%d (+%d) lo:%d mcr %d\r\n", 
         ship->allegiance,
         ship->name,
         ship->mission,
         hullCode[ship->size],
         cfgCode[ship->cfg],
         ship->component[O_QSP_M],
         ship->component[O_QSP_J],
         shipMissionFromCode(ship->mission),
         ship->sr,
         ship->demand,
         ship->lb,
         ship->mcrp * ship->size );
}

void ship_init(Starship* ship)
{
   readShip(SHIP_INDEX_MARAVA, ship); // Marava
}

byte readShip(byte index, Starship* ship)
{
   Starship* tmp = ((Starship*)(0xa040));
	setBank(SHIP_BANK);
   *ship = tmp[index];
	return ship->index; // 0 = done
}

char* shipName(byte index)
{
   Starship tmp;
   readShip(index, &tmp);
   return tmp.name;
}

char* shipMissionFromCode(char code)
{
   switch(code)
   {
	   case 'a': return "trader";
	   case 'c': return "cruiser";
	   case 'e': return "escort";
	   case 'f': return "freighter"; 
	   case 'g': return "frigate";
	   case 'j': return "seeker";
	   case 'k': return "safari";
	   case 'l': return "lab ship";
	   case 'm': return "liner";
	   case 'p': return "corsair";
	   case 'r': return "merchant";
	   case 's': return "scout";
	   case 't': return "transport";
	   case 'u': return "packet";
	   case 'v': return "corvette";
	   case 'w': return "barge";
	   case 'y': return "yacht";
   }
   return "unknown";
}

char* shipBasicMission(byte index)
{
   Starship tmp;
   readShip(index, &tmp);
   return shipMissionFromCode( tmp.mission );
}

char* shipOwner(byte index)
{
   Starship tmp;
   readShip(index, &tmp);
   
   switch(tmp.mission)
   {
      case 'a': return "aslan";
      case 'b': return "baraccai technum";
      case 'd': return "droyne";
      case 'h': return "humbolt";
      case 'i': return "imperial";
      case 'j': return "mc&s";
      case 'l': return "delta";
      case 'm': return "al morai";
      case 'p': return "delta";
      case 'r': return "republic";
      case 's': return "sword worlds";
      case 't': return "tukera";
      case 'v': return "vargr";
      case 'x': return "exotic";
      case 'y': return "daystar";
      case 'z': return "zhodani";
   }
   return "unknown";
}

byte shipOwnerMatchesAllegiance(Starship* ship, char alleg)
{
   return ship->allegiance == ' ' 
       || ship->allegiance == alleg;
}

byte shipMatchesBaseRequirements(Starship* ship, char bases)
{
   byte ok = 1;
   byte no = 0;

   // sell navy and scout ships with navy and scout bases
   switch(bases)
   {
	case 'n': 
	case 'd': 

	   // allow navy ships
	   if (ship->mission == 'e' || ship->mission == 'g' 
	    || ship->mission == 't' || ship->mission == 'v')
	      return ok;

 	   if (ship->mission == 'p') return no; // prohibit corsairs

	   break;

	case 's': 
	   if (ship->mission == 's') return ok; // allow scouts
	   break;

	case 'w': 
	   if (ship->mission == 's') return ok; // allow scouts
 	   if (ship->mission == 'p') return no; // prohibit corsairs
           break;

	// allow all ships except corsairs
        case 'a':
	case 'b':
 	   if (ship->mission == 'p') return no; // prohibit corsairs
	   return ok;
   }
   // default: no scout or navy ships (corsairs OK)
   if ( ship->mission == 's' || ship->mission == 'e'
     || ship->mission == 'g' || ship->mission == 't'
     || ship->mission == 'v' )
      return no;

   return ok;
}

byte  shipMatchesZoneRequirements(Starship* ship, char zone)
{
   // sell corsairs in amber and red zones only
   switch(zone)
   {
 	case 'a':
	case 'r':
	   if (ship->mission == 'p') return 1;
        default:
	   if (ship->mission == 'p') return 0;
   }
   return 1;
}

byte  shipMatchesStarportRequirements(Starship* ship, char starport)
{
   char* owner;
   owner = shipOwner(ship->index);

   // don't sell ships in really poor-quality starports
   switch(starport)
   {
	case 'a':
	case 'b':
	case 'c':
	   return 1; // always sell ships here

	case 'd':
	case 'e':
	   // scouts, traders, seekers, safaris, barges, corsairs sold here
	   if (ship->mission == 's' || ship->mission == 'a'
	    || ship->mission == 'j' || ship->mission == 'k'
	    || ship->mission == 'w' || ship->mission == 'p')
	      return 1;

  	   break;
	   
	case 'x':
	   return 0; // no ships normally sold here
   }
   return 0;
}

void ship_describe(Starship* ship)
{
   cprintf("         ship class      : %s\r\n\r\n", ship->name);

   if (playerAchievementLevel > 1)
   {

   }
}

// void ship_debug( Starship* ship )
// {
//    int i;
//    byte hardpoint;

//    cprintf("index: %u\r\n", ship->index);
//    cprintf("%c %s %c-%c%c%u%u-%c mcr %u\r\n", 
//          ship->allegiance,
//          ship->name,
//          ship->mission,
//          hullCode[ship->size],
//          ship->cfg,
//          ship->component[O_QSP_M],
//          ship->component[O_QSP_J],
//          tlCode[ship->tl],
//          ship->mcrp * ship->size );
   
//    cputs("\r\n");
//    cprintf("bri cpu hol fue ss  ww  scp col stl dmp scr glo\r\n");
//    cprintf("--- --- --- --- --- --- --- --- --- --- --- ---\r\n");
//    cprintf("%3u %3u %3u %3u %3u %3u %3u %3u %3u %3u %3u %3u\r\n",
//       ship->component[O_QSP_BRIDGE],
//       ship->component[O_QSP_CPU],
//       ship->component[O_QSP_CARGOP] * ship->size,
//       ship->component[O_QSP_FUELP]  * ship->size,
//       ship->component[O_QSP_SPACE],
//       ship->component[O_QSP_WORLD],
//       ship->component[O_QSP_SCOOPS],
//       ship->component[O_QSP_COLLECT],
//       ship->component[O_QSP_STEALTH],
//       ship->component[O_QSP_DAMPER],
//       ship->component[O_QSP_SCREEN],
//       ship->component[O_QSP_GLOBE]
//    );

//    cputs("\r\n");
//    for(i=0; i<8; ++i)
//    {
//       hardpoint = ship->component[O_QDP_HP(i)];
//       if (hardpoint & 31)
//          cprintf("hardpoint %d: %s %s\r\n", 
//             i, 
//             emplacement[ hardpoint >> 5],
//             weapon[ hardpoint & 31 ]
//          );
//       else
//          cprintf("hardpoint %d: none\r\n", i);
//    }
// }
