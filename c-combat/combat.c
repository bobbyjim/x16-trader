#include <conio.h>
#include <unistd.h>
#include <cx16.h>

typedef struct {
    char key;
    char name[16];
    int  capacity   : 3; // initial overhead
    int  max_rating : 3; // +1 per rating
    int  active     : 2; // 0=off, 1=on, 2=broken
} Program;

unsigned char activeColors[] = {
    COLOR_GRAY1,
    COLOR_GREEN,
    COLOR_RED
};
#define     PROGRAM_COUNT        15
#define     PROGRAM_ACTIVE       209
#define     PROGRAM_INACTIVE     215

Program program[] = {

    { 'd', "2fi",               4, 0, 0 },
    { 'e', "ecm",               3, 0, 0 },
    { 'g', "gen",               1, 0, 0 },
    { 'h', "ahj",               1, 0, 0 },
    { 'i', "gnr",               1, 0, 0 },
    { 'j', "jmp",               1, 6, 0 }, 
    { 'l', "las",               0, 0, 0 },
    { 'm', "msl",               0, 0, 0 },
    { 'p', "pre",               0, 3, 0 },
    { 'r', "rtf",               1, 0, 0 },
    { 's', "snd",               0, 0, 0 },
    { 't', "rdy",               1, 0, 0 },
    { 'v', "mnv",               1, 6, 0 },
    { 'w', "evd",               1, 0, 0 },
    { 'x', "ami",               2, 0, 0 }
};

void showCommands()
{
    int x;
    //chlinexy(0,50,80);
    cputsxy(0,50,"programs:");
    for (x=0; x<PROGRAM_COUNT; ++x)
    {
        gotoxy( (x % 4) * 19, 52 + (x / 4) * 2);
        cprintf("   %c: %s",
            program[x].key, 
            program[x].name );
    }
}

void showCommandStatus()
{
    int x;
    for (x=0; x<PROGRAM_COUNT; ++x)
    {
        gotoxy( (x % 4) * 19, 52 + (x / 4) * 2);
        textcolor( activeColors[program[x].active] );
        cprintf(" %c", PROGRAM_ACTIVE );
    }
}

void combat()
{
    int x;
    char k;

    bgcolor(COLOR_BLACK);
    clrscr();
    showCommands();

    for(;;)
       if (kbhit())
       {
           k = cgetc();
           if (k=='q') 
              break;
           switch(k)
           {
               case CH_CURS_UP:
                    break;
               case CH_CURS_DOWN:
                    break;
               case CH_CURS_LEFT:
                    break;
               case CH_CURS_RIGHT:
                    break;
           }
           for(x=0; x<PROGRAM_COUNT; ++x)
              if (program[x].key == k)
                 program[x].active = 1 - program[x].active;
           showCommandStatus();
       }
}
