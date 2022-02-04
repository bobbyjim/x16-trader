
#include <stdlib.h>
#include <conio.h>
#include <peekpoke.h>

#include "shipyard.h"
#include "ship.h"
#include "common.h"
#include "alarm.h"

extern Starship ship; // your ship
extern long hcr;      // your cash
extern char* hullCode;
extern char* cfgCode;

byte indexMap[] = {
	255,255,255,255,255,
	255,255,255,255,255,
	255,255,255,255,255,
	255,255,255,255,255
};

void shipSummaryHeader()
{
   revers(1);
   cputs(" ##  type       ar classname          cmj  tons cargo px lo scoops  mcr finance\r\n\r\n");
   revers(0);
}

void tradeStarships()
{
   char shipSelected;
   byte i;

   cputsxy(3,55,"enter ship letter to finance ship purchase.");
   shipSelected = pressReturnAndClear();
  
   if (shipSelected == 13) // return
      return;

   if (shipSelected > 64 			// at least A
    && shipSelected < 85 			// at most T
    && indexMap[shipSelected-65] < 255)		// and there's a ship here
   {
      gotoxy(0,4);
      redline();
      cputsxy(3,6,"congratulations, you have traded in your ship");

      // replace the old one
      i = indexMap[shipSelected]-65;
      readShip(i, &ship);

      // and update our player's mortgage?
   }

   for(i=0; i<20; ++i) indexMap[i] = 255; // reset map
   //exit(0);
}

void landAtShipyard(
        char starport,
        char bases,
        char zone,
        char alleg
)
{
   byte ok = 1;
   byte i  = 0;
   byte i2 = 0;
   char shipSize;
   long refinance = 0;
   Starship floorModel;

   clrscr();

   // 
   // Filter ships by TL and Allegiance.
   // 
   shipSummaryHeader();

   for(i=0;ok > 0;++i)
   {
      readShip(i,&floorModel);
      ok = floorModel.name[0];

      if ( //shipOwnerMatchesAllegiance(&floorModel,alleg)
	      //&& shipMatchesBaseRequirements(&floorModel,bases)
	      //&& shipMatchesZoneRequirements(&floorModel,zone)
	      //&& 
         shipMatchesStarportRequirements(&floorModel,starport)
	   )
      {
         shipSize  = hullCode[ floorModel.size ];      
         refinance = (floorModel.mcrp * floorModel.size) 
                   - (ship.mcrp       * ship.size);

         if (refinance<0) refinance = 0;

         indexMap[i2] = i;
         cprintf(" (%c) %-9s %3u %-18s %c%u%u %5u %5u %2u %2u  %2s     %-4ld\r\n\r\n",
		      65+i2,
		      shipBasicMission(i),
		      floorModel.armor,
		      shipName(i),
		      cfgCode[floorModel.cfg],
		      floorModel.component[ O_QSP_M ],
		      floorModel.component[ O_QSP_J ],
		      floorModel.size * 100,
		      floorModel.component[ O_QSP_CARGOP ] * floorModel.size,
		      floorModel.sr,
		      floorModel.lb,
		      floorModel.component[ O_QSP_SCOOPS ]? "  ":"no",
		      refinance
	         );
         ++i2;
         if (i2 % 20 == 0)
         {
	         tradeStarships();
   	      i2=0;
            shipSummaryHeader();
         }
      }
      ++i;
   }

   if (i2>0) tradeStarships();
   return;
}
