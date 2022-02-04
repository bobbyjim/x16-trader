#include <conio.h>
#include <stdlib.h>

#include "maneuver-map.h"
#include "sprite.h"

#define     OBJ_NONE            0
#define     OBJ_INSTALLATION    1
#define     OBJ_ASTEROID        2
#define     OBJ_ASTEROID_BELT   3
#define     OBJ_PLANET          4
#define     OBJ_GAS_GIANT       5
#define     OBJ_STAR            6
#define     OBJ_MAINWORLD       7

typedef struct {

    int type    :3;
    int quality :4; // could be color or rating
    int size    :4; 
    int junk    :5; 

} SystemSquare;

#define SYSTEM_WIDTH    64
#define SYSTEM_HEIGHT   64

SystemSquare map[SYSTEM_WIDTH][SYSTEM_HEIGHT]; // whatever


typedef struct {

    char type;
    int x;
    int y;

} WorldLocation;

int shipx, shipy, shipdx, shipdy;
WorldLocation world[10];
SpriteDefinition tmpManeuverWorldSprite;

void showWorldSprite(
   unsigned char spriteNum, 
   unsigned char general_world_type,
   int x,
   int y
   )
{
   if (general_world_type > 10) 
      return;

   if (general_world_type == 0) ++general_world_type;

   tmpManeuverWorldSprite.block = 0x6000 + (general_world_type - 1) * 0x400;
   tmpManeuverWorldSprite.mode  = SPRITE_MODE_8BPP;
   tmpManeuverWorldSprite.layer = SPRITE_LAYER_BACKGROUND;
   tmpManeuverWorldSprite.dimensions = SPRITE_32_BY_32;
   tmpManeuverWorldSprite.x     = SPRITE_X_SCALE(x);
   tmpManeuverWorldSprite.y     = SPRITE_Y_SCALE(y);
   
   sprite_define(spriteNum, &tmpManeuverWorldSprite);
}

void maneuvermapInit()
{
    int i;
    for(i=0; i<2; ++i)
    {
        world[i].x = (rand() % 2500) - (rand() % 2500);
        world[i].y = (rand() % 500)  - (rand() % 500);
        world[i].type = rand() % 16;
    }

    for(i=4;i<60;++i)
       cclearxy(0,i,80);

    for(i=50; i<99; ++i)
       sprite_disable(i);
}

void maneuvermapShow()
{
    int i;
    int x1, y1;
    int wx, wy;

   maneuvermapInit();

    shipx = 5000;
    shipy = 5000;

    shipdx = 0;
    shipdy = 0;


    for(;;)
    {
        if (kbhit())
        {
            switch(cgetc())
            {
                case 'w': // north
                    //shipdy /= 2;
                    if (shipdy > -16) --shipdy;
                    break;
                case 's': // south
                    //shipdy /= 2;
                    if (shipdy < 16) ++shipdy;
                    break;
                case 'a': // east
                    //shipdx /= 2;
                    if (shipdx < 16) ++shipdx;
                    break;
                case 'd': // west
                    //shipdx /= 2;
                    if (shipdx > -16) --shipdx;
                    break;
                case 'x':
                    shipdx /= 2;
                    shipdy /= 2;
                    break;
            }
        }
        shipx += shipdx;
        shipy += shipdy;

        gotoxy(5,15);
        cprintf("%d,%d", shipx >> 4, shipy >> 4);
        
        for(i=0; i<10; ++i)
        {           
            x1 = world[i].x - (shipx >> 4);
            y1 = world[i].y - (shipy >> 4);

            wx = 310 + (shipx>>4) - world[i].x;
            wy = 230 + (shipy>>4) - world[i].y;

            if (abs(x1) < 255 && abs(y1) < 255) // close
            {
                if (x1 != 0) shipdx += x1/abs(x1*y1);
                if (y1 != 0) shipdy += y1/abs(x1*y1);
            }
            else
            {
                if (x1 < -320)
                    wx = 620-x1/256;
                else if (x1 > 320)
                    wx = 20-x1/256;

                if (y1 < -240)
                    wy = 450-y1/256;
                else if (y1 > 240)
                    wy = 20-y1/256;
            }            
            showWorldSprite(50+i, world[i].type, wx, wy);
        }
    }

}

