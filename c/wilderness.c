
#include <conio.h>

#include "wilderness.h"
#include "ship.h"
#include "world.h"
#include "alarm.h"

extern Starship ship;
extern byte  shipState[22];
extern World current;
extern byte pay_period;

void wilderness_refuel()
{
    clrscr();

    shipState[ O_STATE_JUMP_FUEL_USED ] = 0;

    printAlarmBar();

    textcolor(COLOR_GREEN);
    gotoxy(0,40);

    if (current.data.hyd > 0 && current.data.atm > 2 && current.data.atm < 11)
    {
        cputsxy(7,35,"you have refueled from surface water on the mainworld (one day)");
        world_setRockballPalette();
        drawWorld(0,1);
        ++pay_period;
    }
    else if (current.data.gg && ship.component[ O_QSP_SCOOPS ])
    {
        cputsxy(12,35,"you have refueled by skimming a gas giant (five days)");
        world_setGasGiantPalette();
        drawWorld(1,1);
        pay_period += 5;
    }
    else if (ship.component[ O_QSP_COLLECT ])
    {
        cputsxy(6,35,"you have refueled by charging your collectors (seven days)");
        pay_period += 7; // might be more based on condition of collectors!
    }
    else if (current.data.belt)
    {
        cputsxy(13,32,"you have refueled from ice in an asteroid belt (ten days)");
        world_drawPlanetoid(37,20);
        pay_period += 10;
    }
    else 
    {
        cputsxy(19,35,"you have refueled at the oort cloud (two weeks)");        
        pay_period += 14;
    }
    textcolor(COLOR_YELLOW);
    cputsxy(28,50,"(press return to continue)");

    cgetc();

}