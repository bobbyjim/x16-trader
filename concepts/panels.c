#include <cx16.h>
#include <conio.h>
#include <stdlib.h>

#define     CORNER_NW       213
#define     CORNER_SW       202
#define     CORNER_NE       201
#define     CORNER_SE       203
#define     HORIZ           192
#define     VERT            221

#define     DEFAULT_COLOR       COLOR_GRAY3

//
//  height and width include border
//
void drawPanel( int x, int y, int width, int height, char* label )
{
    int tmp;

    --width;
    --height;

    cputcxy(x,       y,        CORNER_NW);
    cputcxy(x+width, y,        CORNER_NE);
    cputcxy(x,       y+height, CORNER_SW);
    cputcxy(x+width, y+height, CORNER_SE);

    for(tmp=x+1; tmp<x+width; ++tmp)
    {
        cputcxy(tmp,y,       HORIZ);
        cputcxy(tmp,y+height,HORIZ);
    }

    cputsxy(x+2,y,label);

    while(--height)
    {
        cputcxy(x,      y+height,VERT);
        cputcxy(x+width,y+height,VERT);
    }
}

void drawAlarmBar()
{
   drawPanel(0, 0,  80, 4,  " alarms ");
   cputsxy(2,1, "ops hul cfg mnv jmp     1l  2l  3m  4m  5s  6s  7a  8a  9k      nuc scr glb");
   cputsxy(2,2, " .   .   .   .   .      .   .   .   .   .   .   .   .   .        .   .   . ");
}

void drawFooter()
{
    drawPanel(0, 57, 80, 3,  "");
    cputsxy(25,58,"t r a v e l l e r   t r a d e r");
    cputsxy(2, 58, "1105-200");
    cputsxy(68, 58, "broadsword" );
}

void drawCurrentWorldPanel()
{
    drawPanel(0, 5,  40, 13, " current system ");
    textcolor(COLOR_LIGHTBLUE);
    cputsxy(  2, 7,  "world name     : ");
    cputsxy(  2, 9,  "starport       : ");
    cputsxy(  2, 11, "gas giant      : ");
    cputsxy(  2, 13, "zone           : ");
    cputsxy(  2, 15, "trade comments : ");
    textcolor(DEFAULT_COLOR);
}

void drawDestinationWorldPanel()
{
    drawPanel(0, 19, 40, 13, " destination system ");
    textcolor(COLOR_LIGHTBLUE);
    cputsxy(  2, 21, "world name     : ");
    cputsxy(  2, 23, "starport       : ");
    cputsxy(  2, 25, "gas giant      : ");
    cputsxy(  2, 27, "zone           : ");
    cputsxy(  2, 29, "trade comments : ");
    textcolor(DEFAULT_COLOR);
}

void bracketLetter(int x, int y, char letter, char*label)
{
    gotoxy(x,y);
    textcolor(COLOR_GRAY2);
    cputc('[');
    textcolor(COLOR_YELLOW);
    cputc(letter);
    textcolor(COLOR_GRAY2);
    cputs("] ");
    textcolor(COLOR_GREEN);
    cputs(label);
    textcolor(DEFAULT_COLOR);
}
void drawWorldSelectionList()
{
    drawPanel(40, 5, 40, 51, " nearby systems ");
    bracketLetter(42, 7,  'a', "prince edw  1  a567908-f   pg");
    bracketLetter(42, 9,  'b', "ochecate    1  a567908-f   pg");
    bracketLetter(42, 11, 'c', "mewey       1  a567908-f   pg");
    bracketLetter(42, 13, 'd', "retief      2  a567908-f   pg");
    bracketLetter(42, 15, 'e', "asteltine   2  a567908-f   pg");
    bracketLetter(42, 17, 'f', "inchin      2  a567908-f   pg");
    bracketLetter(42, 19, 'g', "singer      3  a567908-f   pg");
    bracketLetter(42, 21, 'h', "denuli      3  a567908-f   pg");
    bracketLetter(42, 23, 'i', "avastan     3  a567908-f   pg");
    bracketLetter(42, 25, 'j', "faldor      3  a567908-f   pg");
    bracketLetter(42, 27, 'k', "bowman      3  a567908-f   pg");
    bracketLetter(42, 29, 'l', "squallia    3  a567908-f   pg");
    bracketLetter(42, 31, 'm', "tarsus      3  a567908-f   pg");
    bracketLetter(42, 33, 'n', "walston     4  a567908-f   pg");
    bracketLetter(42, 35, 'o', "flexos      4  a567908-f   pg");
    bracketLetter(42, 37, 'p', "collace     4  a567908-f   pg");
    bracketLetter(42, 39, 'q', "pavabid     4  a567908-f   pg");
    bracketLetter(42, 41, 'r', "datrillian  4  a567908-f   pg");
    bracketLetter(42, 43, 's', "nirton      4  a567908-f   pg");
    bracketLetter(42, 45, 't', "judice      5  a567908-f   pg");
    bracketLetter(42, 47, 'u', "trexalon    5  a567908-f   pg");
    bracketLetter(42, 49, 'v', "motmos      5  a567908-f   pg");
    bracketLetter(42, 51, 'w', "noctocol    5  a567908-f   pg");
    bracketLetter(42, 53, 'x', "tarkine     6  a567908-f   pg");
}

void main()
{
    cbm_k_bsout(0x8E); // revert to primary case
    cbm_k_setnam("petfont.bin");
    cbm_k_setlfs(0,8,0);
    cbm_k_load(3, 0x0f000); // r41_rc3

    bgcolor(COLOR_BLACK);    
    textcolor(DEFAULT_COLOR);
    clrscr();
    drawAlarmBar();
    drawFooter();
    drawWorldSelectionList();
    drawCurrentWorldPanel();
    drawDestinationWorldPanel();

    cputsxy( 1, 51, "select a destination world");
}