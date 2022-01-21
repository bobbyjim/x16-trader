#include <conio.h>
#include "common.h"
#include "builder.h"
#include "update.h"
#include "hull_editor.h"

ShipDesign ship;

char* components[] = {
    "-",                    // 0
    "hull",
    "armor",
    "computer",
    "bridge",

    "maneuver",             // 5
    "jump",
    "power",
    "collector",
    "hop",
    "antimatter",           // 10
    "fission",
    "nafal",

    "sound sens",           // 13
    "mass sens",
    "deep radar",           // 15
    "analyzer",
    "densitometer",
    "life det",
    "proximeter",
    "activity sens",        // 20
    "field sens",
    "communicator",
    "jammer",
    "radar",
    "scope",                // 25
    "neutrino det",
    "ems",
    "stealth mask",
    "grav sens",
    "visor",                // 30
    "commplus",
    "holovisor",
    "scanner",

    "slug thrower",
    "datacaster",           // 35
    "stasis",
    "fusion gun",
    "inducer",
    "mining laser",
    "pulse laser",          // 40
    "beam laser",
    "plasma gun",
    "ortillery",
    "sandcaster",
    "jump damper",          // 45
    "tract/press",
    "disruptor",
    "L-M-S",

    "meson scr",
    "damper",               // 50
    "mag scrb",
    "black globe",
    "grav scrb",
    "proton scr",
    "white globe",          // 55

    "ls std",
    "ls lt",
    "ls lux",
    "ls adaptive",
    "clinic",               // 60
    "counsellor",
    "officer ste",
    "crew sr",
    "crew commons",
    "crew c-fresher",       // 65
    "crew s-fresher",
    "emerg lb",
    "low berth",
    "squad barracks",
    "pax suite",            // 70
    "pax sr",
    "pax common",
    "pax c-fresher",
    "pax s-fresher",

    "cargo hold",           // 75
    "magazine",
    "mail vault",
    "cargo lock",
    "lg cargo lock",
    "bracket",              // 80
    "bracket sl",
    "hull niche",
    "hangar lock",
    "grapple"               // 84

};

char* sections[] = {
    "hull",
    "engineering",
    "ops",
    "crew",
    "payload",
    "sensor",
    "weapon",
    "defense"
//    "vehicle"
};

extern char* missionLabel[] = {
   "trader",
   "beagle",
   "cruiser",
   "defender",
   "escort",
   "freighter",
   "frigate",
   "medical", // ?
   "?", // ?
   "prospector",
   "safari",
   "lab",
   "liner",
   "science",
   "?", // ?
   "corsair",
   "boat", // ? 
   "merchant",
   "scout",
   "transport",
   "packet",
   "corvette",
   "barge",
   "express",
   "yacht",
   "exotic"
};

char* allegiance[] = {
   "aslan",
   "b",
   "c",
   "droyne",
   "e",
   "f",
   "g",
   "humbolt",
   "imperial",
   "j",
   "kkree",
   "llellew",
   "m",
   "n",
   "o",
   "p",
   "q",
   "r",
   "s",
   "t",
   "u",
   "vargr",
   "w",
   "x",
   "y",
   "zhodani"
};

char* ratings[]      = { "", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
char* ranges[]       = { "", "l", "d", "vd", "or", "fo", "g", "fr", "sr", "ar", "lr", "ds" };
char* emplacements[] = { "", "surf", "antenna", "extender", "bigant", "t1", "t2", "t3", "t4", "b1", "b2", "bay", "lbay", "main" };
char  cfg[]          = { 'c', 'b', 'p', 'u', 's', 'a', 'l' };

int backgroundColor[] = {
    COLOR_BROWN,
    COLOR_BLACK
};


void builder_init()
{
   ship.mission = 'a';
   ship.hull    = 'b';
   ship.cfg     = 4;
   ship.m       = 1;
   ship.j       = 1;

   ship.allegiance = 'i'; 

   ship.cpu = 1;
   ship.cargo = 80;
   ship.passengers = 8;
   ship.low = 8;

   ship.mcr = 50;

   // ship.component[0].device = 40; // pulse laser
   // ship.component[0].emplacement = 7; // T3
   // ship.component[0].range = 3; // Vd
   // ship.component[0].decitons  = 10; // = 1 ton
   // ship.component[0].hkcr  = 16; // = MCr 1.6
}

void toprow()
{
   cclearxy( 8, 1, 24);
   gotoxy(1,1);
   cprintf("%c-%c%c%d%d %s %s", ship.mission, ship.hull, cfg[ship.cfg], ship.m, ship.j, allegiance[ship.allegiance - 65], missionLabel[ship.mission - 65]);

   gotoxy(30,1);
   cprintf("m/%d c:%-4d px:%-2d lo:%-2d", ship.cpu, ship.cargo, ship.passengers, ship.low);

   gotoxy(70,1);
   cprintf("mcr %d", ship.mcr);
}

void drawSummary()
{
   int i;
   int y;

   unsigned total_decitons = 0;
   unsigned total_hkcr     = 0;
   int x1 = 41;
   int x2 = 48;
   int x3 = 72;

   for(i=0; i<53; ++i)
   {
      y = i+3;
      bgcolor( backgroundColor[ i % 2 ] );
      cputsxy(x1,y, "                                       ");

      if (ship.component[i].decitons > 0)
      {
         gotoxy(x1,y);
         cprintf("%4d", ship.component[i].decitons/10, ship.component[i].decitons % 10);
      }
      gotoxy(x2,y);
      cprintf("%2s %2s %s", 
            ranges[ship.component[i].range],
            emplacements[ship.component[i].emplacement],
            components[ship.component[i].device]
      );
      if (ship.component[i].hkcr > 0)
      {
         gotoxy(x3,y);
         cprintf("%4d.%d", ship.component[i].hkcr/10, ship.component[i].hkcr % 10);
      }

      total_hkcr     += ship.component[i].hkcr;
      total_decitons += ship.component[i].decitons;
   }   
}

void drawScreen()
{
   chlinexy(0,  2, 80 );
   cvlinexy(40, 4, 51 );
   chlinexy(0, 56, 80 );

   toprow();
}

void clearEditor()
{
   int i;
   cclearxy(1,4,20);
   for(i=0; i<8; ++i)
      cclearxy( 1, 6+i, 20);
}

void mainEditor()
{
   int i;
   char c;

   bgcolor(COLOR_BLUE);
   textcolor(COLOR_YELLOW);

   cputsxy(1, 4, "add/edit a component:");
   for(i=0; i<8; ++i)
   {
      gotoxy( 1, 6+i );
      cprintf("%d - %s", i, sections[i]);
   }

   c = cgetc();
   switch(c)
   {
      case EDIT_HULL:       clearEditor(); editHull(&ship);    break;
      case EDIT_DRIVES:     //clearEditor(); editDrive();   break;
      case EDIT_OPS:        //clearEditor(); editOps();     break;
      case EDIT_CREW:       //clearEditor(); editCrew();    break;
      case EDIT_PAYLOAD:    //clearEditor(); editPayload(); break;
      case EDIT_SENSORS:    
      case EDIT_WEAPONS:
      case EDIT_DEFENSES:
      default: break;
   }
}

void builder()
{
   bgcolor(COLOR_BLUE);
   clrscr();
   textcolor(COLOR_YELLOW);

   drawScreen();

   for(;;)
   {
      drawSummary();
      mainEditor();
      update(&ship);
   }
}
