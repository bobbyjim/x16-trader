

#include <peekpoke.h>

#include "common.h"
#include "trig.h"
#include "bank.h"

// azimuth and elevation are in 1.4 degree steps
byte trig_azimuth;
byte trig_elevation;
 
TrigEntry trig_x; 
TrigEntry trig_y;
TrigEntry trig_z;

TrigEntry* trigTable = (TrigEntry*) TRIG_ADDRESS;

TrigEntry  trig_sinAzimuth, 
	   trig_cosAzimuth,
	   trig_sinElevation,
	   trig_cosElevation;

void unitVector()
{
   // first, get the sin and cos of azimuth and elevation.
   // because we only have integers, the resulting values
   // are in hundredths; for example, a value of 0.21 is 
   // represented as a byte value 21.
   setBank(TRIG_BANK);

   trig_sinAzimuth   =  trigTable[trig_azimuth];
   trig_cosAzimuth   =  trigTable[trig_azimuth+64];
   trig_sinElevation =  trigTable[trig_elevation];
   trig_cosElevation =  trigTable[trig_elevation+64];

   // second, compute unit vector while remembering that
   // the sin and cos values are in hundredths.

/* the old clunky way...
   trig_x.mantissa = (byte) (trig_sinAzimuth.mantissa * trig_cosElevation.mantissa / 100);
   trig_y.mantissa = (byte) (trig_cosAzimuth.mantissa * trig_cosElevation.mantissa / 100);
   trig_z.mantissa = trig_sinElevation.mantissa;

   // XOR the signs for the unit vector sign
   trig_x.sign = trig_sinAzimuth.sign ^ trig_cosElevation.sign; 
   trig_y.sign = trig_cosAzimuth.sign ^ trig_cosElevation.sign; 

   trig_z.sign = trig_sinElevation.sign;
*/
   trig_x = (char) (trig_sinAzimuth * trig_cosElevation / 100);
   trig_y = (char) (trig_cosAzimuth * trig_cosElevation / 100);
   trig_z = (char) trig_sinElevation;

   // OK, we're done.  Did it work?
}

char sin(byte theta)
{
   //TrigEntry entry;
   setBank(TRIG_BANK);
   //entry = trigTable[theta];
   //return entry.sign? -entry.mantissa : entry.mantissa;
   return trigTable[theta];
}
   

char cos(byte theta)
{
   return sin(theta+64);
}

