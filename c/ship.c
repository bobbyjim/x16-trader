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
#include "sprite.h"

char* cfgCode  = "cbpusal";
char* cfg[] = {
   "close structure",
   "braced",
   "planetoid",
   "unstreamlined",
   "streamlined",
   "airframe",
   "lift body"
};
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
   "hybrid l-s-m",   "particle accelerator",    "meson gun" 
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
         shipMission(ship->mission),
         ship->sr,
         ship->demand,
         ship->lb,
         ship->mcrp * ship->size );
}

void ship_init(byte ship_index, Starship* ship)
{
   readShip(ship_index, ship); // Marava
}

byte readShip(byte index, Starship* ship)
{
   Starship* tmp = ((Starship*)(0xa040));
	setBank(SHIP_BANK);
   *ship = tmp[index];
	return ship->index; // 0 = done
}

char* shipMission(char code)
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
	   case 'l': return "research";
	   case 'm': return "liner";
      case 'n': return "survey";
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

char* shipOwner(char owner)
{  
   switch(owner)
   {
      case 'a': return "aslan";
      case 'b': return "baraccai technum";
      case 'd': return "droyne";
      case 'e': return "scouts";
      case 'h': return "humbolt";
      case 'i': return "imperial";
      case 'j': return "mc&s";
      case 'l': return "delta";
      case 'm': return "al morai";
      case 'n': return "navy";
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

void ship_describe(Starship* ship)
{
   cprintf("         %s-class type %c %s (%d00 tons)   mcr %u\r\n\r\n\r\n", ship->name, ship->mission, shipMission(ship->mission), ship->size, ship->mcrp * ship->size);
   cprintf("         owner      : %-15s   tl      : %u\r\n\r\n", shipOwner(ship->allegiance), ship->tl );
   cprintf("         config     : %-15s   armor   : %u\r\n\r\n", cfg[ ship->cfg ], ship->armor);
   cprintf("         m-drive    : %-15d   j-drive : %d\r\n\r\n",       SHIP_MANEUVER_RATING(ship), SHIP_JUMP_RATING(ship) );
   cprintf("         bridge     : %-15d   cpu     : model/%d\r\n\r\n", SHIP_BRIDGE_RATING(ship), SHIP_CPU_RATING(ship) );
   cprintf("         staterooms : %-14dlow berths  : %d\r\n\r\n", ship->sr, ship->lb);
   cprintf("         cargo hold : %-15d   \r\n\r\n\r\n", SHIP_CARGO(ship));
   cprintf("         sensor ratings: s: %d, r: %d\r\n\r\n\r\n", SHIP_SPACE_SENSORS(ship), SHIP_WORLD_SENSORS(ship));
   cprintf("         collectors : %-15s   scoops  : %s\r\n\r\n", SHIP_HAS_COLLECTORS(ship)? "yes" : "no", SHIP_HAS_SCOOPS(ship)? "yes" : "no" );
   cprintf("         stealth    : %-15d   damper  : %d\r\n\r\n", SHIP_STEALTH_RATING(ship), SHIP_DAMPER_RATING(ship));
   cprintf("         screen     : %-15d   globe   : %d\r\n\r\n", SHIP_SCREEN_RATING(ship), SHIP_GLOBE_RATING(ship));
   
}


//    cputs("\r\n");
//    for(i=0; i<8; ++i)
//    {
//       hardpoint = ship->component[O_QDP_BATTERY(i)];
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

void ship_loadImage(char* filename)
{
   sprite_loadToVERA(filename,  0x4000);
}

void ship_loadImageFromIndex(byte ship_index)
{
   switch(ship_index)
   {
      case SHIP_INDEX_BEOWULF: ship_loadImage("aia-beo.bin"); break;
      case SHIP_INDEX_MAADA  : ship_loadImage("aia-maa.bin"); break;
      case SHIP_INDEX_MARAVA : ship_loadImage("aia-far.bin"); break;
      case SHIP_INDEX_SAFARI : ship_loadImage("aik-saf.bin"); break;
      case SHIP_INDEX_LINER  : ship_loadImage("aim-lin.bin"); break;
      case SHIP_INDEX_MARCH  : ship_loadImage("air-sub.bin"); break;
      case SHIP_INDEX_PACKET : ship_loadImage("aiu-pak.bin"); break;
      case SHIP_INDEX_SUNFLOWER: ship_loadImage("aik-saf.bin"); break;
      case SHIP_INDEX_BROADSWORD: ship_loadImage("aic-cru.bin"); break;
      case SHIP_INDEX_KILAALUM: ship_loadImage("aie-kil.bin"); break;
      case SHIP_INDEX_GAZELLE : ship_loadImage("aie-gaz.bin"); break;
      case SHIP_INDEX_DONOSEV : ship_loadImage("ain-don.bin"); break;
      case SHIP_INDEX_LAB    : ship_loadImage("ail-lab.bin"); break;
      case SHIP_INDEX_MURPHY : ship_loadImage("ais-sc.bin"); break;

      default:
         ship_loadImage("aia-beo.bin"); break;
   }
}

