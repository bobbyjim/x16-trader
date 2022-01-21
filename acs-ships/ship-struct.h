#ifndef _SHIP_STRUCT_H_
#define _SHIP_STRUCT_H_

// auto-generated with perl

#define     START_ADDRESS   0xa000

#define     BRIDGE      0
#define     CFG         1
#define     CPU         2
#define     MDRV        3
#define     JDRV        4
#define     FUELP       5
#define     SCOOPS      6
#define     HARDPOINT_1 7
#define     HARDPOINT_2 8
#define     HARDPOINT_3 9
#define     HARDPOINT_4 10
#define     HARDPOINT_5 11
#define     HARDPOINT_6 12
#define     HARDPOINT_7 13
#define     HARDPOINT_8 14
#define     HARDPOINT_9 15
#define     COLLECTOR   16
#define     JAMMER      17
#define     DAMPER      18
#define     SCREEN      19
#define     GLOBE       20
#define     TRACKING    21
#define     SURVEY      22

typedef struct {
    unsigned value    : 8;
    unsigned something: 5;
    unsigned damage   : 3;
} Component;

//////////////////////////////////////////////////
//
//   Ship
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
   Component component[28];
} Ship;


Ship* buildShip(int num, Ship* ship);
void debugShip(Ship* ship);

//////////////////////////////////////////////////
//
//   Ship accessors
//
//////////////////////////////////////////////////
unsigned   ship_getIndex(Ship* obj);
void       ship_setIndex(Ship* obj, unsigned value);
char*      ship_getName(Ship* obj);
void       ship_setName(Ship* obj, char* value);
unsigned   ship_getMission(Ship* obj);
void       ship_setMission(Ship* obj, unsigned value);
Component* ship_getCfg(Ship* obj);
void       ship_setCfg(Ship* obj, unsigned value);
unsigned   ship_getHull(Ship* obj);
void       ship_setHull(Ship* obj, unsigned value);
Component* ship_getMdrv(Ship* obj);
void       ship_setMdrv(Ship* obj, unsigned value);
unsigned   ship_getTl(Ship* obj);
void       ship_setTl(Ship* obj, unsigned value);
Component* ship_getJdrv(Ship* obj);
void       ship_setJdrv(Ship* obj, unsigned value);
unsigned   ship_getCargop(Ship* obj);
void       ship_setCargop(Ship* obj, unsigned value);
Component* ship_getCollectors(Ship* obj);
void       ship_setCollectors(Ship* obj, unsigned value);
unsigned   ship_getMcrp(Ship* obj);
void       ship_setMcrp(Ship* obj, unsigned value);
unsigned   ship_getLanders(Ship* obj);
void       ship_setLanders(Ship* obj, unsigned value);
Component* ship_getFuelp(Ship* obj);
void       ship_setFuelp(Ship* obj, unsigned value);
Component* ship_getScoops(Ship* obj);
void       ship_setScoops(Ship* obj, unsigned value);
unsigned   ship_getAv(Ship* obj);
void       ship_setAv(Ship* obj, unsigned value);
unsigned   ship_getIntakes(Ship* obj);
void       ship_setIntakes(Ship* obj, unsigned value);
unsigned   ship_getStaterooms(Ship* obj);
void       ship_setStaterooms(Ship* obj, unsigned value);
Component* ship_getBridge(Ship* obj);
void       ship_setBridge(Ship* obj, unsigned value);
unsigned   ship_getLow(Ship* obj);
void       ship_setLow(Ship* obj, unsigned value);
unsigned   ship_getBins(Ship* obj);
void       ship_setBins(Ship* obj, unsigned value);
Component* ship_getJammer(Ship* obj);
void       ship_setJammer(Ship* obj, unsigned value);
unsigned   ship_getComfort(Ship* obj);
void       ship_setComfort(Ship* obj, unsigned value);
unsigned   ship_getDemand(Ship* obj);
void       ship_setDemand(Ship* obj, unsigned value);
Component* ship_getGlobe(Ship* obj);
void       ship_setGlobe(Ship* obj, unsigned value);
Component* ship_getCpu(Ship* obj);
void       ship_setCpu(Ship* obj, unsigned value);
Component* ship_getDamper(Ship* obj);
void       ship_setDamper(Ship* obj, unsigned value);
Component* ship_getScreen(Ship* obj);
void       ship_setScreen(Ship* obj, unsigned value);
unsigned   ship_getScrambler(Ship* obj);
void       ship_setScrambler(Ship* obj, unsigned value);
unsigned   ship_getSpare1(Ship* obj);
void       ship_setSpare1(Ship* obj, unsigned value);
Component* ship_getCombatSensors(Ship* obj);
void       ship_setCombatSensors(Ship* obj, unsigned value);
Component* ship_getSurveySensors(Ship* obj);
void       ship_setSurveySensors(Ship* obj, unsigned value);
unsigned   ship_getOwner(Ship* obj);
void       ship_setOwner(Ship* obj, unsigned value);
unsigned   ship_getSpare2(Ship* obj);
void       ship_setSpare2(Ship* obj, unsigned value);

#endif
