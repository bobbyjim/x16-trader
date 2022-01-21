#ifndef _trig_h_
#define _trig_h_

#include "common.h"

#define	TRIG_ADDRESS	0xb800

//typedef struct
//{
   //byte mantissa: 9;
   //byte sign    : 1;
//
//} TrigEntry;
typedef char TrigEntry;

void unitVector();
char sin(byte theta);
char cos(byte theta);

#endif
