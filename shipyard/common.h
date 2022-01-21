#ifndef _COMMON_H_
#define _COMMON_H_
#include <cx16.h>


typedef struct {

    unsigned decitons;          // tenths of tons
    unsigned device      :8;    // unique ID, e.g. Particle Accelerator
    unsigned section     :4;    // e.g. Hull, Engineering, Operations, Vehicles, Crew, Payload, Sensors, Weapons, Defenses
    unsigned range       :4;    // e.g. "AR"
    unsigned emplacement :4;    // [Surf,Ant,BigAnt,T1,T2,T3,T4,B1,B2,Bay,LBay,Main]
    unsigned rating      :4;    // comes after the device name e.g. jump-"1"
    unsigned hkcr        :8;    // Cr100,000 increments.  So from KCr100 to MCr25.5.

} Component;

typedef struct {

    char mission;
    char hull;
    int cfg;
    int m;
    int j;

    char allegiance;

    unsigned char cpu;
    unsigned cargo;
    unsigned char passengers;
    unsigned char low;
    unsigned mcr;

    Component component[53];

} ShipDesign;

#endif