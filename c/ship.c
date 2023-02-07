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
#include "panel.h"
#include "emplacement.h"

char* cfgCode  = "cbpusal";
char* hullCode = "0abcdefghjklmnpqrstuvwxyz?????????";
char* tlCode   = "0123456789abcdefghjklmnpqrstuvwxyz";

extern byte playerAchievementLevel;

void showShipSummary(Starship* ship)
{
	setBank(SHIP_BANK);

   // cprintf("         %s-class type %c %s (%d00 tons)   mcr %u\r\n\r\n\r\n", 
   //    ship->name, 
   //    ship->mission, 
   //    SHIP_MISSION_LABEL(ship->mission), 
   //    ship->size, 
   //    ship->mcrp * ship->size);

   cprintf("(%c) %c-%c%c%u%u %s-class %s  (%d00 tons) av %u  mcr %u\r\n\r\n", 
         ship->allegiance,
         ship->mission,
         hullCode[ship->size],
         cfgCode[ship->cfg],
         ship->component[O_QSP_M],
         ship->component[O_QSP_J],
         ship->name,
         SHIP_MISSION_LABEL(ship->mission),
         ship->size,
         // ship->sr,
         // ship->demand,
         // ship->lb,
         ship->armor,
         ship->mcrp * ship->size );
}

void showShipStatus(Starship* ship)
{
   unsigned char i, comp;

   setBank(SHIP_BANK);

   cprintf("%-15s mnv: %ug  vol: %d00  av: %u\r\n\r\n", 
         SHIP_MISSION_LABEL(ship->mission),
         ship->component[O_QSP_M],
         ship->size,
         ship->armor);

   //textcolor(COLOR_LIGHTGREEN);
   cputs("           # empl   weapon     damage\r\n");
   textcolor(COLOR_LIGHTGREEN);

   for(i=0; i<8; ++i)
   {
       comp = SHIP_HARDPOINT(ship,i);
       if (comp > 0)
       {
          cprintf("          %2u %-4s %1s %-10s %2u-%-3u\r\n",
              i,
              getEmplacementName(comp),
              hitsInNextTurn(comp)? "*" : "",
              getWeaponName(comp),
              getMinHits(comp),
              getMaxHits(comp)
          );
       }
   }         
}

void ship_init(byte ship_index, Starship* ship)
{
   readShip(ship_index, ship);
}

byte readShip(byte index, Starship* ship)
{
   Starship* tmp = STARSHIP_DATA; // ((Starship*)(0xa040));
	setBank(SHIP_BANK);
   *ship = tmp[index];
	return ship->index; // 0 = done
}

void ship_describe(Starship* ship)
{
   setBank(SHIP_BANK);

   cprintf("        cargo hold : %-14d     armor : %u\r\n\r\n", SHIP_CARGO(ship), ship->armor);
   cprintf("        staterooms : %-14dlow berths : %d\r\n\r\n", ship->sr, ship->lb);
   cprintf("         s-sensors : %-14dw-sensors  : %d\r\n\r\n", SHIP_SPACE_SENSORS(ship), SHIP_SURVEY_SENSORS(ship));
   // cprintf("        config     : %-15s   armor   : %u\r\n\r\n", SHIP_CFG_LABEL( ship->cfg ), ship->armor);
   // cprintf("        collectors : %-15s   scoops  : %s\r\n\r\n", SHIP_HAS_COLLECTORS(ship)? "yes" : "no", SHIP_HAS_SCOOPS(ship)? "yes" : "no" );
   cprintf("           stealth : %-14d   damper  : %d\r\n\r\n", SHIP_STEALTH_RATING(ship), SHIP_DAMPER_RATING(ship));
   cprintf("            screen : %-14d   globe   : %d\r\n\r\n", SHIP_SCREEN_RATING(ship), SHIP_GLOBE_RATING(ship));   
   cputs("\r\n");
}

void ship_loadImage(char* filename)
{
   sprite_loadToVERA(filename,  0x4000);
}

char* ship_filename(byte ship_index)
{
   switch(ship_index)
   {
//      case SHIP_INDEX_BEOWULF:    return "aia-beo.bin";
      case SHIP_INDEX_MAADA  :    return "aia-maa.bin";
      case SHIP_INDEX_MARAVA :    return "aia-far.bin";
      case SHIP_INDEX_SUSA:       return "aig-enkidu.bin";
      case SHIP_INDEX_SAFARI :    return "aik-saf.bin";
      case SHIP_INDEX_LINER  :    return "aim-lin.bin";
      case SHIP_INDEX_MARCH  :    return "air-sub.bin";
      case SHIP_INDEX_PACKET :    return "aiu-pak.bin";
      case SHIP_INDEX_GUILDER:    return "air-jg.bin";
      case SHIP_INDEX_BROADSWORD: return "aic-cru.bin";
      case SHIP_INDEX_KILAALUM:   return "aie-kil.bin";
      case SHIP_INDEX_GAZELLE :   return "aie-gaz.bin";
      case SHIP_INDEX_ANNIC_NOVA: return "aik-saf.bin"; // safari
      case SHIP_INDEX_DONOSEV :   return "ain-don.bin";
      case SHIP_INDEX_LAB    :    return "ail-lab.bin";
      case SHIP_INDEX_SERPENT:    // scout
      case SHIP_INDEX_MURPHY :    return "ais-sc.bin";
      case SHIP_INDEX_AZ_ALRRAK:  return "aip-var.bin";

      // case cluster liner       return "aig-enkidu.bin";
      // case patrol picket       return "aip-hoshi.bin";
      // case corsair             return "aip-nis.bin";
      
      default:
         ship_loadImage("aia-beo.bin"); break;
   }
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

/*
byte ship_combatStrength(Starship* ship)
{
   int hp = 8;
   byte strength = 0;
   while(--hp)
   {
      byte comp        = ship->component[ O_QDP_BATTERY(hp) ];
      byte emplacement = comp >> 5;
      byte unit        = comp & 31;
      //cprintf("e:%u u:%u, ", emplacement, unit);
      if (comp > 1) // 0 or 1 = empty 
      {
         strength += emplacement;
      }
   }
   //cprintf(" str=%u\r\n", strength);
   return strength;
}
*/