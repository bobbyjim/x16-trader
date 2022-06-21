
#include <conio.h>

#include "bankedText.h"

//
//   Print "externalized" string.
//
void printBankedText(
   unsigned int address,   // location in (banked) RAM to begin printing
   unsigned char len)      // number of bytes to process
{
   char* addr = ((char*)(address));
   while(--len)
   {
      switch(addr[0])
      {
         case 0x0a:
            cputs("\r\n\r\n");
            break;
         case 0x7d: // 125 = }
            cbm_k_bsout(0x1d); // cursor right
            cbm_k_bsout(0x1d); // cursor right
            cbm_k_bsout(0x1d); // cursor right
            cbm_k_bsout(0x1d); // cursor right
            //cputs("     "); // 0x1d); // cursor right
            break;
         default:
            cputc(addr[0]);
      }
      ++addr;
   }
}
