

#include "emplacement.h"

char* weaponName[32] = {
    "-",
    "-",
    "mining las",       
    "pulse las"  ,       
    "beam las"    ,      
    "plasma gun"     ,     
    "fusion gun"      ,    
    "salvo rack"       ,   
    "missile"           ,  
    "k-missile"         , 
    "a-missile"          ,
    "damper"         ,
    "tractor"     ,
    "inducer"             ,
    "disruptor"           ,
    "stasis"              ,
    "sandcaster"          ,
    "hybrid lsm"        ,
    "pa",
    "meson gun"               
};

char* getEmplacementName(byte encoded)
{
    if (encoded == 0) return "-";

    switch(encoded>>5)
    {
        case 0: return "t1";
        case 1: return "t2";
        case 2: return "t3";
        case 3: return "b1";
        case 4: return "b2";
        case 5: return "bay";
        case 6: return "lbay";
        case 7: return "main";
    }
    return "-";
}

char* getWeaponName(byte encoded)
{
    return weaponName[encoded & 31];
}
