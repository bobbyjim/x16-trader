
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

void bookPassengers()
{
   byte demand  = 0;
   byte high    = 0;
   byte mid     = 0;
   byte low     = 0;

   byte high_00;
   byte mid_00;
   byte low_00;
   word subtotal;

   clrscr();
   gotoxy(3,2);
   cprintf("booking passengers bound for %s", getLabel(&destination));
   titleLine();

   high = goodFlux(current.pop + steward);
   if (high > ship.sr) high = ship.sr;

   mid  = goodFlux(current.pop + admin);
   if (high+mid > ship.sr) mid = ship.sr - high;

   low  = goodFlux(current.pop + streetwise);
   if (low > ship.lb) low = ship.lb;

   high_00 = ship.demand + 100;
   mid_00  = ship.demand + 80;
   low_00  = ship.demand + 10;

   subtotal = high * high_00 + mid * mid_00 + low * low_00;
   hcr += subtotal;

   cprintf(" high passengers booked: %u at cr %u00 each.\r\n\r\n", high, high_00);
   cprintf(" mid passengers booked:  %u at cr %u00 each.\r\n\r\n", mid, mid_00);
   cprintf(" low passengers booked:  %u at cr %u00 each.\r\n\r\n", low, low_00);
   cprintf("\r\n\r\n");
   cprintf(" total revenue from passengers: cr %u00\r\n\r\n", subtotal );

   pressReturnAndClear();
}
