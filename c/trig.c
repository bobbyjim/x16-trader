
#include <stdlib.h>

#include "trig.h"


/*
    The trig table is stored in RAM, and is 400 bytes long.
    The first 321 bytes is for sine and cosine:

    Its values run from 
        0 at position 0, to
        +100 at position 64, to
        0 at position 128, to
        -100 at position 192, to
        0 at position 256, to
        +100 at position 320.

    The bytes from 336 to 400 are for arctangent.
    Its values run from 0 to 63.

    The unit circle is thus divided into 256 interpolated degrees.
    You have been warned.
*/

#define        TRIG_TABLE       ((unsigned char*)(0x0400))

int sin(int pseudoDegrees)
{
    pseudoDegrees %= 256;
    if (pseudoDegrees < 128)
        return TRIG_TABLE[ pseudoDegrees ];
    else
        return -(TRIG_TABLE[ pseudoDegrees ] & 127);
}

int cos(int pseudoDegrees)
{
    return sin(pseudoDegrees + 64);
}

/*
    atan: pass in an int, and I'll return the pseudoDegrees.
    And it's going to be super coarse.
*/
int atan(int x)
{
    if (x<-100) return -64; // close enuf
    if (x>100)  return  64; // close enuf

    // now we use the table for the rest.
    if (x >= 0)
        return TRIG_TABLE[0x150 + x];
    else
        return -TRIG_TABLE[0x150 + x];
}
