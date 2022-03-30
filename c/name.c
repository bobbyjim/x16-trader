
#include <stdlib.h>

#include "name.h"

char* fragment[] = {
"co da as si me ea ar ra iz at ta da ra le ig en ",
"ix ag ti id ro ga ex ox lo ca ni me li to la wo ",
"de ru de ag ro po ta se in mo lu de ri pe st er ",
"st ro ta so ar ru wa wo ad ak wa en eg de ac oc ",
"op or pa re ri nu ti ve wi vi ju mo me ne ut a  ",
"i  u  e  o  bectlefleshoonaoraphoshoforguelanlas",
"dentinartortertangingengongantontintindundendtri",
"itomeralubeldecremmenducscothedreownepicencansar",
"genterageatebithenhosgragnocidcanardcorcreousath",
"ainwheluclinmatmedmismelmarnecnornesnouostpelpro",
"porpriplafriregsansalinesolsemsecsulscascisheran",
"mermirspistatecaphteltoltomtentorterlocvalvererg",
};

char* name_generate(char* buf)
{
    char* row;
    int col;
    int i,j;
    int bufpos = 0;
    int syl = 2 + rand() % 2;

    for(i=0; i<syl; ++i)
    {
        row = fragment[ rand() % 12 ];
        col = (rand() % 16) * 3;
        for(j=col; j<col+3; ++j)
        {
           if (row[j] != ' ')
              buf[bufpos++] = row[j];
        }
    }
    buf[bufpos] = 0;
    return buf;
}
