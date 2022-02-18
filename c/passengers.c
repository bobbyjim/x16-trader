
#include <conio.h>
#include <stdlib.h>

#include "common.h"
#include "passengers.h"
#include "ship.h"
#include "world.h"

extern Starship ship;
extern World current;
extern World destination;
extern byte steward;
extern byte admin;
extern byte streetwise;
extern long hcr;

extern byte admin;
extern byte astrogator;
extern byte engineer;
extern byte gunner;
extern byte medic;
extern byte pilot;
extern byte steward;
extern byte streetwise;

//
//  We used to call burtle's library function here.
//
byte goodFlux( int num )
{
   int d1 = rand() % 6;
   int d2 = rand() % 6;
   d2 += num;
   d2 -= d1;
   return abs(d2);
}

void bookPassengersAndPayCrew()
{
   byte demand  = 0;
   byte high    = 0;
   byte mid     = 0;
   byte low     = 0;

   byte high_00;
   byte mid_00;
   byte low_00;
   word subtotal;

   byte crewPay = 0;

   clrscr();
   gotoxy(5,2);
   textcolor(COLOR_LIGHTRED);
   cprintf("booking passengers bound for %s", WORLD_LONG_LABEL(&destination));
   titleLine();

   high = goodFlux(current.data.pop + steward);
   if (high > ship.sr) high = ship.sr;

   mid  = goodFlux(current.data.pop + admin);
   if (high+mid > ship.sr) mid = ship.sr - high;

   low  = goodFlux(current.data.pop + streetwise);
   if (low > ship.lb) low = ship.lb;

   high_00 = ship.demand + 100;
   mid_00  = ship.demand + 80;
   low_00  = ship.demand + 10;

   subtotal = high * high_00 + mid * mid_00 + low * low_00;
   hcr += subtotal;

   textcolor(COLOR_LIGHTBLUE);
   cprintf("     %2u high passengers booked at cr %u00 each.\r\n\r\n", high, high_00);
   cprintf("     %2u mid passengers booked at cr %u00 each.\r\n\r\n", mid, mid_00);
   cprintf("     %2u low passengers booked at cr %u00 each.\r\n\r\n", low, low_00);
   cprintf("\r\n\r\n");
   cprintf("     total revenue from passengers: cr %u.\r\n\r\n", subtotal * 100 );

   textcolor(COLOR_LIGHTRED);
   cprintf("     balance subtotal:              cr %ld00\r\n\r\n", hcr );

   crewPay = admin + astrogator + engineer + gunner + medic + pilot + steward + streetwise;
   textcolor(COLOR_LIGHTBLUE);
   cprintf("     crew salaries based on skill:  cr %u.\r\n\r\n", crewPay * 100 );
   hcr -= crewPay;

   textcolor(COLOR_LIGHTRED);
   cprintf("     balance:                       cr %ld00\r\n\r\n", hcr );

   pressReturnAndClear();
}
