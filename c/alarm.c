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
#include <string.h>

#include "alarm.h"
#include "common.h"
#include "ship.h"

//
//  Player data
//
extern Starship ship;
extern byte     shipState[];
extern byte     shipDamage[];
extern byte     playerAchievementLevel;

unsigned char cost[20];
unsigned char xposition[] = {
      3,  7,  11, 15, 19, 23,             // main components
      47, 50, 53, 56, 59, 62, 65, 68,     // aux components
      32,33,34,35,36,37,38,39             // hardpoints
};

char* alarmLabel[] = {

   "mnv", "jmp", "bri", "cpu", "cgo", "fue", 
   "ss", "ww", "fs", "co", "st", "da", "sc", "gl",
   "1", "2", "3", "4", "5", "6", "7", "8"

};

char alarmColor[7] = {
   CH_GREEN,            // OK
   CH_LIGHTGREEN,       // Minor
   CH_LIGHTBLUE,        // Serious
   CH_YELLOW,           // Major
   CH_LIGHTRED,         // Critical
   CH_GRAY3,            // Unknown
   CH_PURPLE            // Low
};

char* dashes[3] = { "-", "--", "---" };

char* alarmValue[7][] = {
   { ".", ". ", " . " },
   { "*", "* ", " * " },
   { "s", "se", "ser" },
   { "m", "mj", "maj" },
   { "c", "cr", "cri" },
   { "?", "??", "???" },
   { "l", "lo", "low" }
};

char* componentLabel[] = {
      "maneuver drive",
      "jump drive",
      "bridge",
      "computer",
      "cargo",
      "fuel",

      "space sensors",
      "world sensors",
      "scoops",
      "collectors",

      "stealth",
      "damper",
      "screen",
      "globe",

      "emplacement 1",
      "emplacement 2",
      "emplacement 3",
      "emplacement 4",
      "emplacement 5",
      "emplacement 6",
      "emplacement 7",
      "emplacement 8"
};

byte getComponentState(byte component)
{
   return shipState[component];
}

void clearComponentState(byte component)
{
   shipState[component] = STATUS_OK;
   cost[component] = 0;
}

void showStatus(byte component)
{
   int x = xposition[component];
   int y = 2; 
   char *name   = alarmLabel[ component ];
   byte  len    = strlen( name ) - 1;
   byte  state  = shipState[ component ] > shipDamage[component] ?
                  shipState[ component ] : shipDamage[ component ];
   byte  color  = alarmColor[state];
   char* label  = alarmValue[ state ][ len ];

   //
   //  Maybe later we can try blacking out components that are OK.
   // 
   cbm_k_bsout(CH_GREEN);
   cputsxy( x, y-1, name );
   cbm_k_bsout(color); 
   cputsxy( x, y, label );
}

char* getStatusLabel(byte severity)
{
   switch(severity)
   {
 	case STATUS_OK:		 return "ok";
	case STATUS_MINOR:	 return "minor";
	case STATUS_SERIOUS:	 return "serious";
	case STATUS_MAJOR:	 return "major"; 
	case STATUS_CRITICAL: return "critical";
	case STATUS_UNKNOWN:  return "unknown";
	case STATUS_LOW:	    return "low";
   }
   return "unknown";
}

void printAlarmBar()
{
   byte x;

   titleLine();

   if (playerAchievementLevel < 3)
      return;
      
   //cputsxy(2,1,"mnv jmp bri cpu hol fue   12345678   ss ws fs co st dm sc gl");

   for(x=0; x<COMPONENT_COUNT; ++x)
   {
      showStatus(x);
   }

   cputsxy(76,1,"ar");
   gotoxy(75,2);
   cprintf("%3u", ship.armor * ship.tl);

   cprintf("\r\n");
}
