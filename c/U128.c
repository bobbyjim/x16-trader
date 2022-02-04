
#include "U128.h"

//
//  Visible characters for conveying data:
//
//   33-127    ->    0 to 95
//  160-192    ->   96 to 127
//
//  This allows 7 bits per byte transmission.
//  That's very compact for a text representation.
//

//
//  Convert a byte to a printable char
//
char U8ToU128(unsigned char c)
{
    if (c < 96)  return c + 32;
    if (c < 128) return c + 64;
    return 0; // ??
}

//
//  Convert a printable char to a byte value
//
unsigned char U128ToU8(char c)
{
    if (c > 32   && c < 128) return c - 33;
    if (c > 159  && c < 193) return c - 65;
    return 255; // ??
}

