#ifndef _HIRING_HALL_H_
#define _HIRING_HALL_H_

#include "common.h"

#define     SKILL_NONE          0
#define     SKILL_ADMIN         1
#define     SKILL_ASTROGATOR    2
#define     SKILL_ENGINEER      3
#define     SKILL_GUNNER        4
#define     SKILL_MEDIC         5
#define     SKILL_PILOT         6
#define     SKILL_STEWARD       7
#define     SKILL_STREETWISE    8

#define     TOTAL_SKILLS        9
#define     TOTAL_CREW_SLOTS    10

typedef struct
{
    char name[16];
    byte upp[6];
    byte skill[10];
    byte room;
} Crew;

void hire();
byte findHighestSkillFor(byte skill);

#endif
