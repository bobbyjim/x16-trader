
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
#include "damage.h"

extern World current;
extern World destination;
extern byte  distance;
extern Starship ship;
extern byte playerAchievementLevel;
extern byte shipDamage[]; //SHIP_COMPONENT_COUNT];

#define INSYSTEM_NORMAL_SHIP_ENCOUNTER_COUNT    10
#define INSYSTEM_SHIP_ENCOUNTER_PIRATE_INDEX    10
#define INSYSTEM_PIRATE_SHIP_ENCOUNTER_COUNT    7

#define INDENT                                  5
#define I_S_LINE_1                              8
#define I_S_LINE_2                              10
#define I_S_LINE_3                              13
#define I_S_LINE_4                              15
#define I_S_LINE_5                              18
#define I_S_LINE_6                              20
#define I_S_LINE_7                              23
#define I_S_LINE_8                              26
#define I_S_LINE_9                              28
#define I_S_LINE_10                             30

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
    cputsxy(INDENT,I_S_LINE_1,"sir, we have a signal detection at long range.");
    cputsxy(INDENT,I_S_LINE_2,"do we (c)lose distance, or (r)un?");

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
        cputsxy(INDENT,I_S_LINE_3,"we have left the target behind.");
        cputsxy(INDENT,I_S_LINE_4,"press <return> to continue.");
        cgetc();
        return;
    }

    cputsxy(INDENT,I_S_LINE_3,"the ship has overtaken us!");
    cputsxy(INDENT,I_S_LINE_4,"press <return> to continue.");
    cgetc();
    if (guestIsPirate)
        insystem_shipCombat();
    else
        insystem_shipEncounter();
}

void insystem_shipEncounter()
{
    cputsxy(INDENT,I_S_LINE_5,"it's an ordinary ship:");
    gotoxy(INDENT,I_S_LINE_6);
    showShipSummary( &guestStarship );

    // do nothing, for now.
    cputsxy(INDENT,I_S_LINE_7, "pleasantries are exchanged.");
}

void insystem_shipCombat()
{
    unsigned char s1, s2;

    gotoxy(INDENT,15);
    s1 = ship_combatStrength( &ship );
    s2 = ship_combatStrength( &guestStarship );

    textcolor(COLOR_LIGHTRED);
    cputsxy(INDENT,I_S_LINE_5,"it's a pirate!");
    gotoxy(INDENT,I_S_LINE_6);
    showShipSummary( &guestStarship );

    textcolor(COLOR_LIGHTBLUE);
    if (s1 >= s2 ) // if you're well-enough armed, it will run
    {
       cputsxy(INDENT,I_S_LINE_7,"they're not interested in us.");
       return;
    }
    else if ( SHIP_MANEUVER_RATING(&ship) > SHIP_MANEUVER_RATING(&guestStarship))
    {
        // we're faster.
        cputsxy(INDENT,I_S_LINE_7,"we're faster and have outrun them.");
        return;
    }
    cputsxy(INDENT,I_S_LINE_7,"they overtake us and board.");
    cputsxy(INDENT,I_S_LINE_8,"they take our cargo as booty and our passengers as slaves.");
    cputsxy(INDENT,I_S_LINE_9,"they damage a random part of our ship, as well.");

    gotoxy(INDENT,I_S_LINE_10);
    damage_ship(&ship, shipDamage);
    if (playerAchievementLevel > 0)
        --playerAchievementLevel;
}