
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
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "hiring_hall.h"
#include "burtle_jsf32.h"

#define     TOTAL_PERSON_NAMES      35

Crew crew[TOTAL_CREW_SLOTS];

char* skillName[TOTAL_SKILLS] = {
    "none",
    "admn",
    "astr",
    "engr",
    "gunr",
    "lias",
    "medc",
    "pilt",
    "strw",
    "stwd"
};

char* noname = "";

char* personName[TOTAL_PERSON_NAMES] = {
    "santanocheev",
    "aledon",
    "da santos",
    "vren",
    "jamison",
    "illethin",
    "bulolo",
    "tukera",
    "bannerji",
    "arrlanroughl",
    "fireau",
    "thingvellir",
    "oberlindes",
    "laaglir",
    "telemon",
    "krenstein",
    "owen",
    "andor",
    "sirkin",
    "de seripando",
    "moy",
    "hotzel",
    "kurlush",
    "rivera",
    "cobb",
    "frey",
    "serra",
    "torres",
    "washburne",
    "tam",
    "reynolds",
    "mccoy",
    "skinner",
    "tyner",
    "gibson"
};

byte findHighestSkillFor(byte skillIndex)
{
   int i;
   byte high = 0;
   for(i=0; i<TOTAL_CREW_SLOTS; ++i)
   {
       if (crew[i].skill[skillIndex] > high)
          high = crew[i].skill[skillIndex];
   }
   return high;
}

//
// 3D distribution
//                       3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18
char validPeoples[] = { 'm', 'r', 'b', 'a', 'a', 'd', 'i', 'i', 't', 't', 'l', 'v', 'v', 'z', 'r', 's' };

char* decodeRace(char r)
{
    switch(r)
    {
        case 'a': return "aslan";
        case 'b': return "bwap";
        case 'd': return "droyne";
        case 'i': return "vilani";
        case 'l': return "llellewy";
        case 'm': return "amindii";
        case 'r': return "threep";
        case 's': return "shrieker";
        case 't': return "solomani";
        case 'v': return "vargr";
        case 'z': return "zhodani";

        default:
            return "";
    }
}

void printCrewmember(Crew* crew)
{
   int j;

   if (crew->name == 0) crew->name = noname;

   cprintf("%-12s %x%x%x%x%x%x %8s ", 
       crew->name, 
       crew->upp[0],
       crew->upp[1],
       crew->upp[2],
       crew->upp[3],
       crew->upp[4],
       crew->upp[5],
       decodeRace(crew->race)
   );

   for(j=1; j<TOTAL_SKILLS; ++j)
   {
       if (crew->skill[j] > 0)
           cprintf("%s-%d ", skillName[j], crew->skill[j]);
   }
   cputs("\r\n");
}

void showCrew()
{
    int i;

    textcolor(COLOR_LIGHTBLUE);

    cputsxy(2, 5, "crewmember      profile                 skills");
    chlinexy( 2, 6, 12);
    chlinexy(16, 6, 15);
    chlinexy(32, 6, 47);
    cputs("\r\n");

    for(i=0; i<TOTAL_CREW_SLOTS; ++i)
    {
        cprintf("\r\n%d. ", i);
        printCrewmember(&crew[i]);
    }
}

void createCandidate(Crew* candidate)
{
    byte i;

    candidate->name = personName[ burtle32() % TOTAL_PERSON_NAMES ];
//    strncpy(candidate->name, personName[ rand() % TOTAL_PERSON_NAMES ], 15);

    for(i=0; i<6; ++i)
       candidate->upp[i] = diceRoll2d();

    candidate->race = validPeoples[ diceRoll3d() - 3 ];

    for(i=0; i<TOTAL_SKILLS; ++i)
    {
        candidate->skill[i] = 0;
        switch(diceRoll2d())
        {
            case 2: candidate->skill[ i ]++;
            case 3: candidate->skill[ i ]++;
            case 4: candidate->skill[ i ]++;
            case 5: candidate->skill[ i ]++;
            case 6: candidate->skill[ i ]++;
        }
    }
}

void hireSomeone()
{
    Crew candidate;

    createCandidate( &candidate );
    redline();

    textcolor(COLOR_GRAY2);

    cputs("\r\ncandidate:\r\n\r\n");
    printCrewmember( &candidate );
    cputs("\r\n");
    cputs("     [0-9]   to hire.\r\n");
    cputs("    <return> to reject.\r\n");

    for(;;)
    {
       switch(cgetc())
       {
           case 13:  return;
           case '0': crew[0] = candidate; return;
           case '1': crew[1] = candidate; return;
           case '2': crew[2] = candidate; return;
           case '3': crew[3] = candidate; return;
           case '4': crew[4] = candidate; return;
           case '5': crew[5] = candidate; return;
           case '6': crew[6] = candidate; return;
           case '7': crew[7] = candidate; return;
           case '8': crew[8] = candidate; return;
           case '9': crew[9] = candidate; return;
       }
    }
}

void hire()
{
    _randomize();

    clrscr();
    titleLine();
    showCrew();
    hireSomeone();
}

