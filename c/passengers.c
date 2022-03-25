
#include <conio.h>
#include <stdlib.h>

#include "common.h"
#include "passengers.h"
#include "ship.h"
#include "world.h"
#include "trade.h"

extern Starship ship;
extern World current;
extern World destination;
extern byte pay_period;

extern long hcr;

extern byte admin;
extern byte astrogator;
extern byte engineer;
extern byte gunner;
extern byte liaison;
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

int posFlux( int num )
{
   num += rand() % 6;
   num -= rand() % 6;
   return (num > 0)? num : 0;
}

void bookPassengersAndPayCrew()
{
   byte demand  = 0;
   byte high    = 0;
   byte mid     = 0;
   byte low     = 0;
   unsigned freight = 0;
   unsigned totalFreight = 0;
   unsigned totalTCs = 0;
   unsigned holdFree = getHoldFree();

   int i;
   byte high_00;
   byte mid_00;
   byte low_00;
   unsigned subtotal;
   unsigned crewPay;

   clrscr();
   gotoxy(5,2);
   textcolor(COLOR_GREEN);
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

   textcolor(COLOR_LIGHTRED);
   cprintf("     initial balance:                 cr %7ld00\r\n\r\n", hcr );

   subtotal = high * high_00 + mid * mid_00 + low * low_00;
   hcr += subtotal;

   textcolor(COLOR_LIGHTBLUE);
   if (high) cprintf("     %2u high passengers booked at cr %u00 each.\r\n\r\n", high, high_00);
   if (mid ) cprintf("     %2u mid passengers booked at cr %u00 each.\r\n\r\n", mid, mid_00);
   if (low ) cprintf("     %2u low passengers booked at cr %u00 each.\r\n\r\n", low, low_00);

   cprintf("\r\n\r\n");

   if (high + mid + low)
   {
      cprintf("     revenue from passengers:       + cr %9lu\r\n\r\n", (long)subtotal * 100L );
      textcolor(COLOR_LIGHTRED);
      cprintf("     balance subtotal:                cr %7ld00\r\n\r\n", hcr );
   }

   //
   // freight (T5 Book 2 p220)
   //
   totalTCs = 1
            + current.data.agricultural
            + current.data.asteroid
            + current.data.barren
            + current.data.desert
            + current.data.fluid_seas
            + current.data.hi_pop
            + current.data.icecapped
            + current.data.industrial
            + current.data.lo_pop
            + current.data.non_agri
            + current.data.non_ind
            + current.data.poor
            + current.data.rich
            + current.data.vacuum;

   for(i=0; i<7; ++i)
   {
      freight = posFlux( current.data.pop ) * totalTCs + liaison;
      if (totalFreight + freight <= holdFree)
         totalFreight += freight;
   }

   textcolor(COLOR_LIGHTBLUE);
   cprintf("     freight:                       + cr %6u000\r\n\r\n", totalFreight);
   hcr += totalFreight * 10;

   textcolor(COLOR_LIGHTRED);
   cprintf("     balance subtotal:                cr %7ld00\r\n\r\n", hcr );
   //
   //  hcr per week
   //
   crewPay = admin + astrogator + engineer + gunner + liaison + medic + pilot + steward + streetwise;

   //
   //  add in ship maintenance 
   //
   crewPay += (unsigned)ship.size + SHIP_JUMP_RATING(&ship) + SHIP_MANEUVER_RATING(&ship); // 1 share per 100 tons
   //cprintf("ship size: %u\r\n", ship.size);

   //
   //  scale to credits and pay period
   //
   crewPay *= (2 + 14 * pay_period);

   //
   //  scale back to hcr
   //
   crewPay /= 100;

   //
   //  reset pay period
   //
   pay_period = 0;

   textcolor(COLOR_LIGHTBLUE);
   cprintf("     crew and maintenance:          - cr %9u\r\n\r\n", crewPay * 100);
   hcr -= crewPay;

   textcolor(COLOR_LIGHTRED);
   cprintf("     balance:                         cr %7ld00\r\n\r\n", hcr );

   pressReturnAndClear();
}
