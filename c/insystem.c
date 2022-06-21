
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

#include <stdlib.h>
#include <conio.h>

#include "common.h"
#include "insystem.h"
#include "ship.h"
#include "world.h"
#include "name.h"

extern World current;
extern World destination;
extern byte  distance;

#define INSYSTEM_NORMAL_SHIP_ENCOUNTER_COUNT    10
#define INSYSTEM_SHIP_ENCOUNTER_PIRATE_INDEX    10
#define INSYSTEM_PIRATE_SHIP_ENCOUNTER_COUNT    7

byte shipType[] = { 
        SHIP_INDEX_BEOWULF,
        SHIP_INDEX_MAADA,
        SHIP_INDEX_MARAVA,
        SHIP_INDEX_SAFARI,
        SHIP_INDEX_LINER,
        SHIP_INDEX_MARCH,
        SHIP_INDEX_PACKET,
        SHIP_INDEX_GAZELLE,
        SHIP_INDEX_DONOSEV,
        SHIP_INDEX_LAB,
        // these can be pirates:
        SHIP_INDEX_MURPHY,          
        SHIP_INDEX_BROADSWORD,
        SHIP_INDEX_KILAALUM,
        // these are only pirates:
        SHIP_INDEX_KFORGZUE,
        SHIP_INDEX_AZ_ALRRAK,
        SHIP_INDEX_FANG,
        SHIP_INDEX_AEDOKHAZ
};

Starship guestStarship;
byte     guestIsPirate;
//char     guestName[15];

byte checkShipEncounter()
{
    int s = 72;
    int p = 0;
    int value = rand() % 100;

//    name_generate(guestName);

    if (current.data.starport == 'c')                       { s = 52;  p  = 8;  }
    if (current.data.starport == 'd')                       { s = 40;  p  = 11; }
    if (current.data.starport == 'e')                       { s = 20;  p  = 8;  }
    if (current.data.starport == 'x')                       { s = 7;   p  = 13; }
    if (current.data.zone     == 'a')   /* amber zone */    { s -= 10; p += 10; } 
    if (current.data.zone     == 'r')   /* red zone!  */    { s -= 20; p += 20; } 
    
    p = 100;

    if (value >= 100-p) // p = the percentage chance of running into a pirate (ranges from 0 to 33%)
    {
        value = INSYSTEM_SHIP_ENCOUNTER_PIRATE_INDEX + rand() % INSYSTEM_PIRATE_SHIP_ENCOUNTER_COUNT;
        readShip(shipType[value], &guestStarship);
        guestIsPirate = 1;
        return 1;
    }
    else if (value <= s) // non-pirate ship
    {
        value = rand() % INSYSTEM_NORMAL_SHIP_ENCOUNTER_COUNT;
        readShip(shipType[value], &guestStarship);
        guestIsPirate = 0;
        return 1;
    }
    else return 0;
}

void insystem_headForJumpPoint()
{
    if (checkShipEncounter() == 0) return; // ok no encounter.

    clrscr();
    // sleep(2);
    titleLine();

    textcolor(COLOR_ORANGE);
    cputsxy(10,20,"sir, we have a signal detection at long range.");
    cputsxy(10,22,"do we (c)lose distance, or (r)un?");

    switch(cgetc())
    {
        case 'c':
            if (guestIsPirate)
                insystem_shipCombat();
            else
                insystem_shipEncounter();
            break;

        case 'r':
        default:
            insystem_run();
    }

    textcolor(COLOR_YELLOW);
    cputsxy(30,40,"press <return>");
    cgetc();
}

void insystem_run()
{
    if (rand() % 100 > 50) // (2D + maneuver - pirate's maneuver >= 10) for this
    {
        cputsxy(10,24,"we have left the target behind.");
        cputsxy(10,26,"press <return> to continue.");
        if (cgetc()) {}
        return;
    }

    cputsxy(10,24,"the ship has overtaken us!");
    cputsxy(10,26,"press <return> to continue.");
    if (cgetc()) {}
    if (guestIsPirate)
        insystem_shipCombat();
    else
        insystem_shipEncounter();
}

void insystem_shipEncounter()
{
    cputsxy(10,25,"it's an ordinary ship:");
    gotoxy(10,27);
    showShipSummary( &guestStarship );

    // do nothing, for now.
    cputsxy(10,31, "pleasantries are exchanged.");
}

void insystem_shipCombat()
{
    cputsxy(10,25,"it's a pirate!");
    gotoxy(10,27);
    showShipSummary( &guestStarship );

    cputsxy(10,31,"it's afraid of us, and is running.");
    // if you're well-enough armed, it will run
    // else
    // if you're faster, you can run
    // else
    // 
}