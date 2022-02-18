
#include <conio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "hiring_hall.h"

#define     TOTAL_PERSON_NAMES      34

Crew crew[TOTAL_CREW_SLOTS];

char* skillName[TOTAL_SKILLS] = {
    "none",
    "admn",
    "astr",
    "engr",
    "gunr",
    "medc",
    "pilt",
    "stwi",
    "stwd"
};

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
    "tyner"
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

void printCrewmember(Crew* crew)
{
   int j;
   cprintf("%-15s  %x%x%x%x%x%x   ", 
       crew->name, 
       crew->upp[0],
       crew->upp[1],
       crew->upp[2],
       crew->upp[3],
       crew->upp[4],
       crew->upp[5]
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

    gotoxy(0,5);
    cprintf("   crewmember       profile  skills\r\n");
    cputs("   ");
    chline(15);
    cputs("  ");
    chline(7);
    cputs("  ");
    chline(45);
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

    strncpy(candidate->name, personName[ rand() % TOTAL_PERSON_NAMES ], 15);

    for(i=0; i<6; ++i)
       candidate->upp[i] = (rand() % 6) + (rand() % 6) + 2;

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

