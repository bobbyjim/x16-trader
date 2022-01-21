#include <stdio.h>
#include <cbm.h>
#include <conio.h>
#include <string.h>

void createFile()
{
    FILE *fp  = fopen( "uwp1", "w" );
    fprintf(fp, "spin1910 a788899-c ns ri an cp sa       amin varg asla 4bcf 703 8 imdd f7 5 bd   m3 5 regina\r\n");
    fclose(fp);
}

//
// Poor man's fseek() for the X16
//
unsigned char x16_fseek( unsigned char channel, unsigned long position )
{
    char command[6];
    unsigned char result;

    command[0] = 'P';
    command[1] = channel & 15;
    command[2] = position >> 24;
    command[3] = (position >> 16) & 255;
    command[4] = (position >> 8) & 255;
    command[5] = position & 255;

    cbm_k_setlfs (15,8,15);
    cbm_k_setnam( command );
    result = cbm_k_open();
    cbm_k_close(15);

    return result;
}

void x16_fclose( unsigned char channel )
{
    cbm_k_close(channel);
}

void testSeek()
{
    unsigned char lfn = 1;
    unsigned char sec_addr = 1;
    unsigned char buffer[255];
    unsigned char c = cbm_open(lfn, 8, sec_addr, "mapfile" );
    
    cprintf("response: %u\r\n", c );

    cbm_read(lfn, buffer, 200); 
 
    cbm_close(lfn);
}

void readBinaryFile(const char *filename)
{
   char c1;
   int  x, y;
   FILE *fp = fopen( filename, "r" );

   if (fp)
   {
       for(x=0; x<50; ++x)
       {
           for( y=0; y<20; ++y)
           {
               c1 = fgetc(fp);
               if (c1 == 1) cprintf("\r\n");
               else         cprintf(" %c ", c1 );
           }
           cprintf("\r\n");
       }
       fclose(fp);
   }
}

void readFile2(const char *filename)
{
    char buf[200];
    FILE *fp = fopen( filename, "r" );

    if (fp)
    {
        fgets(buf, 100, fp);
        fclose(fp);
        cprintf("data: %s\r\n", buf);
    }
}

unsigned char readFile(const char *filename)
{
    char s[200];
    unsigned char lfn = 1;
    unsigned char dev = 8;
    unsigned char sec_addr = 0;
    unsigned char res = cbm_open(lfn, dev, sec_addr, filename);

    if (res == 0)
    {
        cbm_read(lfn, s, 200);
        cbm_close(lfn);
        cprintf("data: %s\r\n", s);
    }
    
    return res;
}

unsigned char readDir(const char* path)
{
    struct cbm_dirent dir;
    unsigned char res = cbm_opendir(1, 8, path);

    //cprintf("dir: [%s]\n", path);
    if (res == 0)
    {
        cprintf("\r\nlisting for %s:\r\n", path);
        cbm_readdir(1, &dir); // disk header
        //cprintf("acc size  timestamp   name\r\n"
        //       "--- ---- ------------ ----------------\r\n");
        while (cbm_readdir(1, &dir) == 0) 
        {
            switch(dir.type)
            {
                case 2:  textcolor(15); break;
                case 17: textcolor(13); break; 
                default: textcolor(1); break; 
            }
            cprintf("%03d %4d              %s\r\n",
                dir.access,
                dir.size, 
                dir.name
                );
        }
        cbm_closedir(1);
    }
    return res;
}

void main(void)
{
    bgcolor(0);
    textcolor(5);
    clrscr();
    cprintf( "v15\r\n\r\n" );

    testSeek();
    //readFile("uwp1");
    //readFile2("uwp1");
    //readBinaryFile("$");
    //readDir("$");
    //readDir("$/test1/:uwp*");

    cbm_k_bsout(0x8E); // revert to primary case
}


//1910 Regina               A788899-C Ri Pa Ph An Cp (Amindii)2 Varg0 Asla0 Sa { 4 }  (D7E+5) [9C6D] BcCeF NS - 703 8  ImDd F7 V BD M3 V
