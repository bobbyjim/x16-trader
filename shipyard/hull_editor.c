#include <conio.h>
#include <cx16.h>

#include "hull_editor.h"
#include "builder.h"

char* menu[] = {
    "mission",
    "hull volume",
    "hull configuration",
    "landers",
    "fuel scoops",
    "fuel purifiers",
    "armor layers",
    "computer model"
};

void updateSelection(ShipDesign* ship, int sel, int val)
{
   switch(sel)
   {
       case 0: 
          ship->mission += val;  
          if (ship->mission < 'a') ship->mission = 'z';
          if (ship->mission > 'z') ship->mission = 'a';
          toprow();
          break;

       case 1: 
          ship->hull += val;
          if (ship->hull < 'a') ship->hull = 'z';
          if (ship->hull > 'z') ship->hull = 'a';
          toprow();
          break;
          
       case 2: 
          ship->cfg += val;
          if (ship->cfg < 0) ship->cfg = 6;
          if (ship->cfg > 6) ship->cfg = 0;
          toprow();
          break;
          
       case 3: 
       case 4: 
       case 5: 
       case 6: 
       case 7: 
       default: break;
   }
}

void editHull(ShipDesign* ship)
{
    int i=0;
    int sel=0;

    cputsxy(1, 4, "hull editor");

    for(;;)
    {
       for(i=0; i<8; ++i)
       {
           if (sel == i) revers(1);
           cputsxy( 1, 5+i, menu[i]);
           revers(0);
       }
       switch( cgetc() )
       {
           case CH_CURS_UP:     --sel; break;
           case CH_CURS_DOWN:   ++sel; break;
           case CH_CURS_LEFT:   updateSelection(ship, sel,-1); break;
           case CH_CURS_RIGHT:  updateSelection(ship, sel,1); break;
           default: break;
       }
       if (sel < 0) sel = 7;
       sel %= 8;
    }
}