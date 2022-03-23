

#include <stdlib.h>
#include <conio.h>

#include "common.h"
#include "combat.h"
#include "damage.h"

extern byte              playerAchievementLevel;
extern Starship          ship;
extern byte              shipState[SHIP_COMPONENT_COUNT];
extern byte              shipDamage[SHIP_COMPONENT_COUNT];
extern byte engineer;
extern byte gunner  ;
extern byte medic   ;
extern byte pilot   ;

Starship adversary;
byte     adversaryDamage[SHIP_COMPONENT_COUNT];
byte     adversaryEngineer;
byte     adversaryGunner;
byte     adversaryMedic;
byte     adversaryPilot;

void combat_run()
{
    // determine adversary and crew skill level
}