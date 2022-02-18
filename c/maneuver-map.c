#include <conio.h>
#include <stdlib.h>

#include "maneuver-map.h"
//#include "sprite.h"
#include "world.h"
#include "ship.h"
//#include "burtle_jsf32.h"
#include "alarm.h"
#include "menu.h"

// #define     OBJ_NONE            0
// #define     OBJ_INSTALLATION    1
// #define     OBJ_ASTEROID        2
// #define     OBJ_ASTEROID_BELT   3
// #define     OBJ_PLANET          4
// #define     OBJ_GAS_GIANT       5
// #define     OBJ_STAR            6
// #define     OBJ_MAINWORLD       7

// typedef struct {

//     int type    :3;
//     int size    :4; 

// } SystemSquare;

// #define SYSTEM_ORBIT_COUNT  16

/*
    Moving around the system.
 */

// SystemSquare map[SYSTEM_ORBIT_COUNT]; // whatever
// long rho;
// unsigned theta;
// unsigned char worldVisible;
// unsigned char worldMenuVisible;

// SpriteDefinition tmpManeuverWorldSprite;
extern World     current, destination;
extern byte      playerAchievementLevel;
extern Starship  ship;
extern long      hcr;

// extern Starship ship;

// void maneuvermapInit()
// {
//    int i  = burtle32_srand( current.col + current.row * 11 + current.data.siz * 13 + current.data.atm * 17 + current.data.hyd * 19);
//    int mw =      1 + burtle32_random() % 4; // 1 to 4
//    int ab = mw + 1 + burtle32_random() % 6; // 2 to 10
//    int gg = ab + 1 + burtle32_random() % 6; // 3 to 14
   
//    map[0].type = OBJ_STAR;
//    map[3].type = OBJ_MAINWORLD;
//    if (WORLD_HAS_BELTS(&current))
//       map[ab].type = OBJ_ASTEROID_BELT;   

//    if (WORLD_HAS_GGS(&current))
//       map[gg].type = OBJ_GAS_GIANT;

//    for(i=1; i<gg+2; ++i)
//       if ((map[i].type == 0) && (burtle32_random() % 2 == 1))
//          map[i].type  = OBJ_PLANET;
// }

// void maneuvermapShowSystem()
// {
//     int i;
//     int x = 0;
//     int y = 30;
//     for(i=0; i<SYSTEM_ORBIT_COUNT; ++i)
//     {
//         switch(map[i].type)
//         {
//             case OBJ_ASTEROID:
//                 x += 4;
//                 textcolor(COLOR_GRAY1);
//                 cputcxy(x,y,',');
//                 break;
//             case OBJ_ASTEROID_BELT:
//                 x += 4;
//                 textcolor(COLOR_GRAY1);
//                 cputcxy(x,y,';');
//                 break;
//             case OBJ_GAS_GIANT:
//                 x += 4;
//                 textcolor(COLOR_LIGHTBLUE);
//                 cputcxy(x,y,209);
//                 break;
//             case OBJ_INSTALLATION:
//                 x += 4;
//                 textcolor(COLOR_WHITE);
//                 cputcxy(x,y,'i');
//                 break;
//             case OBJ_MAINWORLD:
//                 x += 4;
//                 textcolor(COLOR_GREEN);
//                 cputcxy(x,y,209);
//                 break;
//             case OBJ_PLANET:
//                 x += 4;
//                 textcolor(COLOR_GRAY3);
//                 cputcxy(x,y,172);
//                 break;
//             case OBJ_STAR:
//                 x += 4;
//                 textcolor(COLOR_YELLOW);
//                 cputcxy(x,y,'*');
//                 break;
//             case OBJ_NONE:
//             default: // nothing
//                 break;
//         }
//     }
// }

// void showWorldSprite(
//    unsigned char spriteNum, 
//    unsigned char general_world_type,
//    unsigned x,
//    unsigned y)
// {
//    if (general_world_type > 10) 
//       return;

//    if (general_world_type == 0) ++general_world_type;

//    tmpManeuverWorldSprite.block = 0x6000 + (general_world_type - 1) * 0x400;
//    tmpManeuverWorldSprite.mode  = SPRITE_MODE_8BPP;
//    tmpManeuverWorldSprite.layer = SPRITE_LAYER_BACKGROUND;
//    tmpManeuverWorldSprite.dimensions = SPRITE_32_BY_32;
//    tmpManeuverWorldSprite.x     = SPRITE_X_SCALE(x);
//    tmpManeuverWorldSprite.y     = SPRITE_Y_SCALE(y+120);
   
//    sprite_define(spriteNum, &tmpManeuverWorldSprite);
// }

// void hideWorldSprite(unsigned char spriteNum)
// {
//     tmpManeuverWorldSprite.layer = SPRITE_DISABLED;
//     sprite_define(spriteNum, &tmpManeuverWorldSprite);
// }

// void showWorldMenu()
// {
//     gotoxy(4,1);
//     cputs("(d)ock  (l)and  (o)rbit  (r)efuel  (s)urvey");
// }

// void hideWorldMenu()
// {
//     cclearxy(4,1,40);
// }

void showCurrentLocation()
{
    clrscr();

    printAlarmBar();

    gotoxy(0,7);
    textcolor(COLOR_LIGHTRED);
    cputs("     current world\r\n\r\n");
    world_describe(&current);
    cputs("\r\n");
    //cprintf("         credits         : %ld00\r\n\r\n", hcr);

    if (DIFFERENT_WORLDS(destination, current))
    {
        gotoxy(0,17);
        textcolor(COLOR_LIGHTRED);
        cputs("     destination world\r\n\r\n");
        world_describe(&destination);
    }
}

unsigned char maneuver()
{
    int opts = 6;
    char* mainworldMenuOptions[] = {
        "astrogation",
        "jump",
        "starport",
        "trade",
        "hiring hall",
        "shipyard",
    };  

    textcolor(COLOR_YELLOW);
    if (playerAchievementLevel > 1)
        cputsxy(9, 30, "move to location:");
    else   
        cputsxy(9, 30, "press <return>");
   
    if (playerAchievementLevel < opts)
        opts = playerAchievementLevel;

    return menu_run(9, 30, opts, mainworldMenuOptions);
}

// void maneuvermapShow()
// {
//     int cpos = 1;

//     // maneuvermapShowSystem();

//     // cputcxy(4 + cpos * 4, 32, '^');
//     // for(;;)
//     // {
//     //     if (kbhit())
//     //     {
//     //         switch(cgetc())
//     //         {
//     //             case 0x9d: // left
//     //                 cpos = (cpos + 15) % 16;
//     //                 break;
//     //             case 0x1d: // right
//     //                 cpos = (cpos + 1) % 16;
//     //                 break;
//     //         }
//     //         cclearxy(0, 32, 80);
//     //         cputcxy(4 + cpos * 4, 32, '^');
//     //     }
//     // }
// }
