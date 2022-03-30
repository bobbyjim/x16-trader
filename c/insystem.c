
#include    <stdlib.h>
#include <conio.h>

#include "common.h"
#include "insystem.h"
#include "ship.h"
#include "world.h"
#include "name.h"

extern World current;
extern World destination;
extern byte  distance;

#define INSYSTEM_NORMAL_SHIP_ENCOUNTER_COUNT    17
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
char     guestName[15];

byte checkShipEncounter()
{
    int s = 72;
    int p = 0;
    int value = rand() % 100;

    name_generate(&guestName);

    if (current.data.starport == 'c')                       { s = 52;  p = 8;  }
    if (current.data.starport == 'd')                       { s = 40;  p = 11; }
    if (current.data.starport == 'e')                       { s = 20;  p = 8;  }
    if (current.data.starport == 'x')                       { s = 7;   p = 13; }
    if (current.data.zone     == 'a')   /* amber zone */    { s -= 10; p += 10; }  
    if (current.data.zone     == 'r')   /* red zone!  */    { s -= 20; p += 20; }  

    if (value >= 100-p) // pirate
    {
        value = INSYSTEM_SHIP_ENCOUNTER_PIRATE_INDEX + rand() % INSYSTEM_PIRATE_SHIP_ENCOUNTER_COUNT;
        readShip(value, &guestStarship);
        guestIsPirate = 1;
        return 1;
    }
    else if (value <= s) // non-pirate ship
    {
        value = rand() % INSYSTEM_NORMAL_SHIP_ENCOUNTER_COUNT;
        readShip(value, &guestStarship);
        guestIsPirate = 0;
        return 1;
    }
    else return 0;
}

void insystem_headForJumpPoint()
{
    if (checkShipEncounter())
        insystem_shipSighting();
}

void insystem_prospect()
{

}

void insystem_shipSighting()
{
    clrscr();
    // sleep(2);
    titleLine();

    cputsxy(20,20,"sir, we have a signal detection at long range.");
    cputsxy(20,22,"do we (c)lose distance, or (r)un?");

    switch(cgetc())
    {
        case 'c':
            insystem_shipEncounter();
            break;

        case 'r':
        default:
            insystem_run();
    }
}

void insystem_run()
{
    if (rand() % 100 > 50) // (2D + maneuver - pirate's maneuver >= 10) for this
    {
        cputsxy(20,24,"we have left the target behind.");
        cputsxy(20,26,"press <return> to continue.");
        if (cgetc()) {}
        return;
    }

    cputsxy(20,24,"the ship has overtaken us!");
    cputsxy(20,26,"press <return> to continue.");
    if (cgetc()) {}
    if (guestIsPirate)
        insystem_shipCombat();
    else
        insystem_shipEncounter();
}

void insystem_shipEncounter()
{
    cputs(" == ordinary ship encounter here ==");
    if (cgetc()) {}
    return;
}

void insystem_shipCombat()
{
    cputs(" == ship combat here ==");
    if (cgetc()) {}
    return;
}