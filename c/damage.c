
#include    <stdlib.h>
#include    <conio.h>

#include    "common.h"
#include    "damage.h"

byte damage_location;

//      O_QSP_M			0
//      O_QSP_J			1
//      O_QSP_BRIDGE	2
//      O_QSP_CPU    	3
//      O_QSP_CARGOP	4
//      O_QSP_FUELP		5
//      O_STATE_JUMP_FUEL_USED 5
//      O_QSP_SPACE		6
//      O_QSP_WORLD		7
//      O_QSP_SCOOPS	8
//      O_QSP_COLLECT	9
//      O_QSP_STEALTH	10
//      O_QSP_DAMPER	11
//      O_QSP_SCREEN	12
//      O_QSP_GLOBE		13
//      O_QDP_BATTERY(x)		(14+x)
char* damage_message[SHIP_COMPONENT_COUNT] = {
    "the helm's not responding",
    "the jump drives are sparking",
    "a control console is on fire",
    "the computer is spouting gibberish",
    "we're leaking hydrogen fuel",
    "an antenna dish just melted",
    "a surface sensor just cracked",
    "fuel scoops are down",
    "the collector has a hole in it",
    "the stealth coating melted away",
    "the damper is offline",
    "the mescon screen is down",
    "the globe generator went dark",
    "battery one is not responding",
    "battery two is not responding",
    "battery three is not responding",
    "battery four is not responding",
    "battery five is not responding",
    "battery six is not responding",
    "battery seven is not responding",
    "battery eight is not responding"    
};

void damage_ship(Starship* thisShip, byte shipDamage[])
{
    for(;;)
    {
       damage_location = rand() % SHIP_COMPONENT_COUNT;
       if(thisShip->component[damage_location] > 0)
       {
          shipDamage[ damage_location ]++;
          cprintf("captain, %s.", damage_message[ damage_location] );
          return;
       }
    }
}
