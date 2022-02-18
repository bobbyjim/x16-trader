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
#include <stdlib.h>

#include "common.h"
#include "pilot.h"
#include "world.h"
#include "ship.h"
#include "alarm.h"

extern World destination;
extern World current;
extern World localSystem[];
extern Starship ship;

byte  componentSelection = 0; // main ship components
byte 	turretSelection    = 0; // emplacements
byte	systemSelection	   = 0; // specialized systems

byte	starfieldx[100], starfieldy[100];
char  thingtype[100];
byte	acceleration = 0;

byte 	orbit = 30; // = orbit 3.  255 = orbit 25.

byte 	vx=0,vy=0;

#define	WORLD_COUNT	10
#define  ECLIPTIC	   27

word    energy = 1000;

#define PILOT_MENU_OPTIONS	7

byte    pilotMenuSelection = 0;
char*   pilotMenuOptions[PILOT_MENU_OPTIONS] = {
	"pilot\0",
	"astrog\0",
	"port\0",
	"jump\0",
	"market\0",
	"hiring\0",
	"shipyd\0",
//	"survey\0",
//	"gg ref\0",
//	"prosp\0",
//	"patrol\0",
//	"piracy\0"
};

char reticleLeft[6]  = {219,219,192,192,192,0};
char reticleRight[6] = {192,192,192,219,219,0};

#define	CROSS		219
#define NWCORNER	176
#define NECORNER	174
#define SWCORNER	173
#define SECORNER	189

#define  VERTICAL_BAR      221

#define  PETSCII_BALL      209
#define  PETSCII_DIAMOND   218
#define  PETSCII_SAIL1     223
#define  PETSCII_SAIL2     169
#define  PETSCII_SQUARE    172


void initStarfield()
{
   byte x;
   if (starfieldx[0] > 0) return;

   for(x=0;x<100;++x)
   {
      starfieldx[x] = 1 + rand() % 255;
      starfieldy[x] = 1 + rand() % 255;
      thingtype[x]  = '.';

      switch((rand() % 6) + (rand() % 6))
      {
         case 0:  thingtype[x] = ':';              break;
         case 1:  thingtype[x] = '*';              break;
         case 2:  thingtype[x] = '+';              break;
         default: thingtype[x] = '.';
      }
 
      //
      //  put planets on the ecliptic
      //
      if ((starfieldy[x] >= ECLIPTIC-1) && (starfieldy[x] <= ECLIPTIC+1))
         thingtype[x] = PETSCII_BALL;

   }
   thingtype[0]  = PETSCII_DIAMOND; // mainworld
   starfieldy[0] = ECLIPTIC; 
   starfieldx[0] = 40;
}

byte pilotMenu()
{
   int i;
   for(;;)
   {
      for(i=0;i<PILOT_MENU_OPTIONS;++i)
      {
         if(i==pilotMenuSelection) revers(1);
         cputsxy(73,i+5,pilotMenuOptions[i]);
         if(i==pilotMenuSelection) revers(0);
      }
      switch(cgetc())
      {
        case 0x11: // down
            pilotMenuSelection = ++pilotMenuSelection % PILOT_MENU_OPTIONS;
            break;
        case 0x91: // up
            if (pilotMenuSelection == 0) 
               pilotMenuSelection = PILOT_MENU_OPTIONS;
            --pilotMenuSelection;
            break;
        case 0x0d: // return key
         	cputsxy(73,pilotMenuSelection+5,pilotMenuOptions[pilotMenuSelection]);
		      if (pilotMenuSelection != JUMP_OPTION || destination.row != current.row || destination.col != current.col)
            {
               return pilotMenuSelection;
            }
 		      break;
      }
   }
}

void drawReticle()
{
   cputcxy(38, 25, NWCORNER);
   cputcxy(42, 25, NECORNER);
   cputcxy(38, 29, SWCORNER);
   cputcxy(42, 29, SECORNER);
   cputsxy(33, 27, reticleLeft);
   cputsxy(43, 27, reticleRight);
}

char* whatisthis(char thing)
{
   switch(thing)
   {
      case PETSCII_DIAMOND:   return "mainworld";
      case PETSCII_BALL:      return "planet   ";
      case PETSCII_SQUARE:    return "planet   ";
      case '*':               return "star     ";
      case '+':               return "star     ";
      case '.':               return "star     ";
      case ':':               return "comet    ";
      default :               return "         ";
   }
}

void drawScreen(char dx, char dy)
{
   byte i;
   byte x,y;

   textcolor(COLOR_GRAY3);

   //
   //  starfield
   //
   cputsxy( 70, 58, "         " );
   for(i=0;i<100;++i)
   {
      // translate to viewport coords(vx,vy is top left)
      x = (starfieldx[i] - vx);
      y = (starfieldy[i] - vy);
      if (x>0 && x<80 && y>4 && y<56) // erase
         cputcxy(x, y, ' ');

      // translate again
      x -= dx;
      y -= dy;
      
      if (x>0 && x<80 && y>4 && y<56) // draw
         cputcxy(x, y, thingtype[i]);

      if (x == 40 && y == 27) // in the center of the reticle
         cputsxy( 70, 58, whatisthis(thingtype[ i ]) );
   }

   // update viewport x,y
   vx += dx; 
   vy += dy; 

   drawReticle();
   toDefaultColor();
   gotoxy(60,58);
   // vx and vy are apparently azimuth and elevation
   cprintf("%03u:%03u", 
      vx, 
      vy, 
      ((starfieldx[0] - 40) + 256) % 256, 
      ((starfieldy[0] - 27) + 256) % 256
   );
}

void clearViewport()
{
   int y = 56 ;

   while(y-- > 4)
      cclearxy(0,y,80);
}

byte doPilot()
{
   byte i;

   initStarfield();

   clrscr();
   toDefaultColor();
   pilotMenuSelection = 0; // reset

   printAlarmBar();

   drawScreen(0,0);

   statusLine();
   
   cprintf("      energy: %05u      %c     <return> for menu     %c", energy, VERTICAL_BAR, VERTICAL_BAR );

   for(;;)
   {
      switch(cgetc())
      {
         // Find the mainworld, please
         case 'm': // m = mainworld
            vx = ((starfieldx[0] - 40) + 256) % 256;
            vy = ((starfieldy[0] - 27) + 256) % 256;
            clearViewport();
            drawScreen(0,0);
            break;

	      // Elevation
	      case 0x11:
		      drawScreen(0,-1);
		      break;
	      case 0x91:
		      drawScreen(0,1);
		      break;

	      // Acceleration
	      case 'a': // down
		      if (acceleration > 0) --acceleration;
		      break;
	      case 'z': // up
		      if (acceleration < 255) ++acceleration;
		      break;

	      // Steering
	      case 0x9d: // right
		      drawScreen(1,0);
		      break;
	      case 0x1d: // left
		      drawScreen(-1,0);
		      break;
	
	// Menu
	case 0x0d: // return
	 	i = pilotMenu();
		if (i>0) return i;
		break;
      }
   }
   return 0;
}
