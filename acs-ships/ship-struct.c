#include <string.h>
#include "ship-struct.h"

//////////////////////////////////////////////////
//
//  ShipDAO
//
//  I moved this out of the header and into the C file
//  in order to hide it from the rest of the system.
//
//////////////////////////////////////////////////
typedef struct {
   unsigned      index: 8;
   char name[16];
   unsigned    mission: 5;
   unsigned        cfg: 3;
   unsigned       hull: 5;
   unsigned       mdrv: 3;
   unsigned         tl: 5;
   unsigned       jdrv: 3;
   unsigned     cargop: 7;
   unsigned collectors: 1;
   unsigned       mcrp: 7;
   unsigned    landers: 1;
   unsigned      fuelp: 7;
   unsigned     scoops: 1;
   unsigned         av: 7;
   unsigned    intakes: 1;
   unsigned staterooms: 6;
   unsigned     bridge: 2;
   unsigned        low: 6;
   unsigned       bins: 1;
   unsigned     jammer: 1;
   unsigned    comfort: 3;
   unsigned     demand: 3;
   unsigned      globe: 2;
   unsigned        cpu: 3;
   unsigned     damper: 1;
   unsigned     screen: 1;
   unsigned  scrambler: 1;
   unsigned     spare1: 2;
   unsigned combatSensors: 4;
   unsigned surveySensors: 4;
   unsigned      owner: 5;
   unsigned     spare2: 3;
} ShipDAO;

unsigned   shipDAO_getIndex(ShipDAO* obj) { return obj->index; }
char*      shipDAO_getName(ShipDAO* obj) { return obj->name; }
unsigned   shipDAO_getMission(ShipDAO* obj) { return obj->mission; }
unsigned   shipDAO_getCfg(ShipDAO* obj) { return obj->cfg; }
unsigned   shipDAO_getHull(ShipDAO* obj) { return obj->hull; }
unsigned   shipDAO_getMdrv(ShipDAO* obj) { return obj->mdrv; }
unsigned   shipDAO_getTl(ShipDAO* obj) { return obj->tl; }
unsigned   shipDAO_getJdrv(ShipDAO* obj) { return obj->jdrv; }
unsigned   shipDAO_getCargop(ShipDAO* obj) { return obj->cargop; }
unsigned   shipDAO_getCollectors(ShipDAO* obj) { return obj->collectors; }
unsigned   shipDAO_getMcrp(ShipDAO* obj) { return obj->mcrp; }
unsigned   shipDAO_getLanders(ShipDAO* obj) { return obj->landers; }
unsigned   shipDAO_getFuelp(ShipDAO* obj) { return obj->fuelp; }
unsigned   shipDAO_getScoops(ShipDAO* obj) { return obj->scoops; }
unsigned   shipDAO_getAv(ShipDAO* obj) { return obj->av; }
unsigned   shipDAO_getIntakes(ShipDAO* obj) { return obj->intakes; }
unsigned   shipDAO_getStaterooms(ShipDAO* obj) { return obj->staterooms; }
unsigned   shipDAO_getBridge(ShipDAO* obj) { return obj->bridge; }
unsigned   shipDAO_getLow(ShipDAO* obj) { return obj->low; }
unsigned   shipDAO_getBins(ShipDAO* obj) { return obj->bins; }
unsigned   shipDAO_getJammer(ShipDAO* obj) { return obj->jammer; }
unsigned   shipDAO_getComfort(ShipDAO* obj) { return obj->comfort; }
unsigned   shipDAO_getDemand(ShipDAO* obj) { return obj->demand; }
unsigned   shipDAO_getGlobe(ShipDAO* obj) { return obj->globe; }
unsigned   shipDAO_getCpu(ShipDAO* obj) { return obj->cpu; }
unsigned   shipDAO_getDamper(ShipDAO* obj) { return obj->damper; }
unsigned   shipDAO_getScreen(ShipDAO* obj) { return obj->screen; }
unsigned   shipDAO_getScrambler(ShipDAO* obj) { return obj->scrambler; }
unsigned   shipDAO_getSpare1(ShipDAO* obj) { return obj->spare1; }
unsigned   shipDAO_getCombatSensors(ShipDAO* obj) { return obj->combatSensors; }
unsigned   shipDAO_getSurveySensors(ShipDAO* obj) { return obj->surveySensors; }
unsigned   shipDAO_getOwner(ShipDAO* obj) { return obj->owner; }
unsigned   shipDAO_getSpare2(ShipDAO* obj) { return obj->spare2; }
//////////////////////////////////////////////////
//
//   Ship Accessors
//
//////////////////////////////////////////////////
unsigned   ship_getIndex(Ship* obj) { return obj->index; }
void       ship_setIndex(Ship* obj, unsigned value) 
{
   obj->index = value;
}
char*      ship_getName(Ship* obj) { return obj->name; }
void       ship_setName(Ship* obj, char* value) 
{
   strcpy(obj->name, value);
}
unsigned   ship_getMission(Ship* obj) { return obj->mission; }
void       ship_setMission(Ship* obj, unsigned value) 
{
   obj->mission = value;
}
Component* ship_getCfg(Ship* obj) { return &(obj->component[1]); }

void       ship_setCfg(Ship* obj, unsigned value) 
{
   obj->component[1].value = value;
}
unsigned   ship_getHull(Ship* obj) { return obj->hull; }
void       ship_setHull(Ship* obj, unsigned value) 
{
   obj->hull = value;
}
Component* ship_getMdrv(Ship* obj) { return &(obj->component[3]); }

void       ship_setMdrv(Ship* obj, unsigned value) 
{
   obj->component[3].value = value;
}
unsigned   ship_getTl(Ship* obj) { return obj->tl; }
void       ship_setTl(Ship* obj, unsigned value) 
{
   obj->tl = value;
}
Component* ship_getJdrv(Ship* obj) { return &(obj->component[4]); }

void       ship_setJdrv(Ship* obj, unsigned value) 
{
   obj->component[4].value = value;
}
unsigned   ship_getCargop(Ship* obj) { return obj->cargop; }
void       ship_setCargop(Ship* obj, unsigned value) 
{
   obj->cargop = value;
}
Component* ship_getCollectors(Ship* obj) { return &(obj->component[16]); }

void       ship_setCollectors(Ship* obj, unsigned value) 
{
   obj->component[16].value = value;
}
unsigned   ship_getMcrp(Ship* obj) { return obj->mcrp; }
void       ship_setMcrp(Ship* obj, unsigned value) 
{
   obj->mcrp = value;
}
unsigned   ship_getLanders(Ship* obj) { return obj->landers; }
void       ship_setLanders(Ship* obj, unsigned value) 
{
   obj->landers = value;
}
Component* ship_getFuelp(Ship* obj) { return &(obj->component[5]); }

void       ship_setFuelp(Ship* obj, unsigned value) 
{
   obj->component[5].value = value;
}
Component* ship_getScoops(Ship* obj) { return &(obj->component[6]); }

void       ship_setScoops(Ship* obj, unsigned value) 
{
   obj->component[6].value = value;
}
unsigned   ship_getAv(Ship* obj) { return obj->av; }
void       ship_setAv(Ship* obj, unsigned value) 
{
   obj->av = value;
}
unsigned   ship_getIntakes(Ship* obj) { return obj->intakes; }
void       ship_setIntakes(Ship* obj, unsigned value) 
{
   obj->intakes = value;
}
unsigned   ship_getStaterooms(Ship* obj) { return obj->staterooms; }
void       ship_setStaterooms(Ship* obj, unsigned value) 
{
   obj->staterooms = value;
}
Component* ship_getBridge(Ship* obj) { return &(obj->component[0]); }

void       ship_setBridge(Ship* obj, unsigned value) 
{
   obj->component[0].value = value;
}
unsigned   ship_getLow(Ship* obj) { return obj->low; }
void       ship_setLow(Ship* obj, unsigned value) 
{
   obj->low = value;
}
unsigned   ship_getBins(Ship* obj) { return obj->bins; }
void       ship_setBins(Ship* obj, unsigned value) 
{
   obj->bins = value;
}
Component* ship_getJammer(Ship* obj) { return &(obj->component[17]); }

void       ship_setJammer(Ship* obj, unsigned value) 
{
   obj->component[17].value = value;
}
unsigned   ship_getComfort(Ship* obj) { return obj->comfort; }
void       ship_setComfort(Ship* obj, unsigned value) 
{
   obj->comfort = value;
}
unsigned   ship_getDemand(Ship* obj) { return obj->demand; }
void       ship_setDemand(Ship* obj, unsigned value) 
{
   obj->demand = value;
}
Component* ship_getGlobe(Ship* obj) { return &(obj->component[20]); }

void       ship_setGlobe(Ship* obj, unsigned value) 
{
   obj->component[20].value = value;
}
Component* ship_getCpu(Ship* obj) { return &(obj->component[2]); }

void       ship_setCpu(Ship* obj, unsigned value) 
{
   obj->component[2].value = value;
}
Component* ship_getDamper(Ship* obj) { return &(obj->component[18]); }

void       ship_setDamper(Ship* obj, unsigned value) 
{
   obj->component[18].value = value;
}
Component* ship_getScreen(Ship* obj) { return &(obj->component[19]); }

void       ship_setScreen(Ship* obj, unsigned value) 
{
   obj->component[19].value = value;
}
unsigned   ship_getScrambler(Ship* obj) { return obj->scrambler; }
void       ship_setScrambler(Ship* obj, unsigned value) 
{
   obj->scrambler = value;
}
unsigned   ship_getSpare1(Ship* obj) { return obj->spare1; }
void       ship_setSpare1(Ship* obj, unsigned value) 
{
   obj->spare1 = value;
}
Component* ship_getCombatSensors(Ship* obj) { return &(obj->component[21]); }

void       ship_setCombatSensors(Ship* obj, unsigned value) 
{
   obj->component[21].value = value;
}
Component* ship_getSurveySensors(Ship* obj) { return &(obj->component[22]); }

void       ship_setSurveySensors(Ship* obj, unsigned value) 
{
   obj->component[22].value = value;
}
unsigned   ship_getOwner(Ship* obj) { return obj->owner; }
void       ship_setOwner(Ship* obj, unsigned value) 
{
   obj->owner = value;
}
unsigned   ship_getSpare2(Ship* obj) { return obj->spare2; }
void       ship_setSpare2(Ship* obj, unsigned value) 
{
   obj->spare2 = value;
}
//////////////////////////////////////////////////
//
//   Ship Builder
//
//////////////////////////////////////////////////
Ship* buildShip(int num, Ship* ship)
{
   ShipDAO* data = (ShipDAO*)(START_ADDRESS + (num<<5));

   ship_setIndex( ship, shipDAO_getIndex(data) );
   ship_setName( ship, shipDAO_getName(data) );
   ship_setMission( ship, shipDAO_getMission(data) );
   ship_setCfg( ship, shipDAO_getCfg(data) );
   ship_setHull( ship, shipDAO_getHull(data) );
   ship_setMdrv( ship, shipDAO_getMdrv(data) );
   ship_setTl( ship, shipDAO_getTl(data) );
   ship_setJdrv( ship, shipDAO_getJdrv(data) );
   ship_setCargop( ship, shipDAO_getCargop(data) );
   ship_setCollectors( ship, shipDAO_getCollectors(data) );
   ship_setMcrp( ship, shipDAO_getMcrp(data) );
   ship_setLanders( ship, shipDAO_getLanders(data) );
   ship_setFuelp( ship, shipDAO_getFuelp(data) );
   ship_setScoops( ship, shipDAO_getScoops(data) );
   ship_setAv( ship, shipDAO_getAv(data) );
   ship_setIntakes( ship, shipDAO_getIntakes(data) );
   ship_setStaterooms( ship, shipDAO_getStaterooms(data) );
   ship_setBridge( ship, shipDAO_getBridge(data) );
   ship_setLow( ship, shipDAO_getLow(data) );
   ship_setBins( ship, shipDAO_getBins(data) );
   ship_setJammer( ship, shipDAO_getJammer(data) );
   ship_setComfort( ship, shipDAO_getComfort(data) );
   ship_setDemand( ship, shipDAO_getDemand(data) );
   ship_setGlobe( ship, shipDAO_getGlobe(data) );
   ship_setCpu( ship, shipDAO_getCpu(data) );
   ship_setDamper( ship, shipDAO_getDamper(data) );
   ship_setScreen( ship, shipDAO_getScreen(data) );
   ship_setScrambler( ship, shipDAO_getScrambler(data) );
   ship_setSpare1( ship, shipDAO_getSpare1(data) );
   ship_setCombatSensors( ship, shipDAO_getCombatSensors(data) );
   ship_setSurveySensors( ship, shipDAO_getSurveySensors(data) );
   ship_setOwner( ship, shipDAO_getOwner(data) );
   ship_setSpare2( ship, shipDAO_getSpare2(data) );
   return ship;
}

//////////////////////////////////////////////////
//
//   debuggers
//
//////////////////////////////////////////////////
void ShipDAO_debug( ShipDAO* data )
{
   printf("   %10s %u\n", "index", shipDAO_getIndex(data) );
   printf("   %10s %u\n", "name", shipDAO_getName(data) );
   printf("   %10s %u\n", "mission", shipDAO_getMission(data) );
   printf("   %10s %u\n", "cfg", shipDAO_getCfg(data) );
   printf("   %10s %u\n", "hull", shipDAO_getHull(data) );
   printf("   %10s %u\n", "mdrv", shipDAO_getMdrv(data) );
   printf("   %10s %u\n", "tl", shipDAO_getTl(data) );
   printf("   %10s %u\n", "jdrv", shipDAO_getJdrv(data) );
   printf("   %10s %u\n", "cargop", shipDAO_getCargop(data) );
   printf("   %10s %u\n", "collectors", shipDAO_getCollectors(data) );
   printf("   %10s %u\n", "mcrp", shipDAO_getMcrp(data) );
   printf("   %10s %u\n", "landers", shipDAO_getLanders(data) );
   printf("   %10s %u\n", "fuelp", shipDAO_getFuelp(data) );
   printf("   %10s %u\n", "scoops", shipDAO_getScoops(data) );
   printf("   %10s %u\n", "av", shipDAO_getAv(data) );
   printf("   %10s %u\n", "intakes", shipDAO_getIntakes(data) );
   printf("   %10s %u\n", "staterooms", shipDAO_getStaterooms(data) );
   printf("   %10s %u\n", "bridge", shipDAO_getBridge(data) );
   printf("   %10s %u\n", "low", shipDAO_getLow(data) );
   printf("   %10s %u\n", "bins", shipDAO_getBins(data) );
   printf("   %10s %u\n", "jammer", shipDAO_getJammer(data) );
   printf("   %10s %u\n", "comfort", shipDAO_getComfort(data) );
   printf("   %10s %u\n", "demand", shipDAO_getDemand(data) );
   printf("   %10s %u\n", "globe", shipDAO_getGlobe(data) );
   printf("   %10s %u\n", "cpu", shipDAO_getCpu(data) );
   printf("   %10s %u\n", "damper", shipDAO_getDamper(data) );
   printf("   %10s %u\n", "screen", shipDAO_getScreen(data) );
   printf("   %10s %u\n", "scrambler", shipDAO_getScrambler(data) );
   printf("   %10s %u\n", "spare1", shipDAO_getSpare1(data) );
   printf("   %10s %u\n", "combatSensors", shipDAO_getCombatSensors(data) );
   printf("   %10s %u\n", "surveySensors", shipDAO_getSurveySensors(data) );
   printf("   %10s %u\n", "owner", shipDAO_getOwner(data) );
   printf("   %10s %u\n", "spare2", shipDAO_getSpare2(data) );
}

void Ship_debug( Ship* ship )
{
   printf("   %10s %d\n", "index", ship_getIndex(ship) );
   printf("   %10s %d\n", "name", ship_getName(ship) );
   printf("   %10s %d\n", "mission", ship_getMission(ship) );
   printf("   %10s %d\n", "cfg", ship_getCfg(ship) );
   printf("   %10s %d\n", "hull", ship_getHull(ship) );
   printf("   %10s %d\n", "mdrv", ship_getMdrv(ship) );
   printf("   %10s %d\n", "tl", ship_getTl(ship) );
   printf("   %10s %d\n", "jdrv", ship_getJdrv(ship) );
   printf("   %10s %d\n", "cargop", ship_getCargop(ship) );
   printf("   %10s %d\n", "collectors", ship_getCollectors(ship) );
   printf("   %10s %d\n", "mcrp", ship_getMcrp(ship) );
   printf("   %10s %d\n", "landers", ship_getLanders(ship) );
   printf("   %10s %d\n", "fuelp", ship_getFuelp(ship) );
   printf("   %10s %d\n", "scoops", ship_getScoops(ship) );
   printf("   %10s %d\n", "av", ship_getAv(ship) );
   printf("   %10s %d\n", "intakes", ship_getIntakes(ship) );
   printf("   %10s %d\n", "staterooms", ship_getStaterooms(ship) );
   printf("   %10s %d\n", "bridge", ship_getBridge(ship) );
   printf("   %10s %d\n", "low", ship_getLow(ship) );
   printf("   %10s %d\n", "bins", ship_getBins(ship) );
   printf("   %10s %d\n", "jammer", ship_getJammer(ship) );
   printf("   %10s %d\n", "comfort", ship_getComfort(ship) );
   printf("   %10s %d\n", "demand", ship_getDemand(ship) );
   printf("   %10s %d\n", "globe", ship_getGlobe(ship) );
   printf("   %10s %d\n", "cpu", ship_getCpu(ship) );
   printf("   %10s %d\n", "damper", ship_getDamper(ship) );
   printf("   %10s %d\n", "screen", ship_getScreen(ship) );
   printf("   %10s %d\n", "scrambler", ship_getScrambler(ship) );
   printf("   %10s %d\n", "spare1", ship_getSpare1(ship) );
   printf("   %10s %d\n", "combatSensors", ship_getCombatSensors(ship) );
   printf("   %10s %d\n", "surveySensors", ship_getSurveySensors(ship) );
   printf("   %10s %d\n", "owner", ship_getOwner(ship) );
   printf("   %10s %d\n", "spare2", ship_getSpare2(ship) );
}

