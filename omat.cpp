
/*****************************************************************************
*                                                                            *
*   Keyboard handler - 09h (hookup)    /stdio.h + conio.h + dos.h            *
*                                                                            *
*   Initializing string:                                                     *
*       mov_hookup_09h (); //no parameters                                   *
*                                                                            *
*   Release string:                                                          *
*       mov_release_09h (); //no parameters                                  *
*                                                                            *
*   mov_get_presses() and mov_put_presses (int how_many ) are used to put    *
*   and get the count of presses that user have made.                        *
*                                                                            *
*****************************************************************************/

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;


float PI=3.14159265359;
float PI_=256/PI;

void (__interrupt __far *mov_prev_int_9) ();

char inter_luku1, inter_luku2;
char mov_keydown[128];
int  presses = 0;
int  keypressed;

void __interrupt __far mov_keyb_09h()
{
    inter_luku1 = inp(0x60);
    mov_keydown[inter_luku1 & 0x7f] = (inter_luku1 <= 0x7f); //mika nappula alhaalla
    inter_luku2 = inp(0x61);
    outp(0x61, (inter_luku2 | 0x80));
    outp(0x61, inter_luku2);
    keypressed = 1;
    outp(0x20, 0x20);
    presses++;
}

void mov_hookup_09h ()
{
    mov_prev_int_9 = _dos_getvect (0x09);  //ota intin osote yl”s.
    _dos_setvect (0x09, mov_keyb_09h);      //laita oman routinen osote sinne.
}

void mov_release_09h ()
{
    _dos_setvect (0x09, mov_prev_int_9);   //palauta vanha kunnon osote takaisin
}

int mov_get_presses ()
{
    return ((int)presses/2);
}

void mov_put_presses ( int how_many )
{
    presses = how_many;
}



void mode (int numero );
#pragma aux mode = \
    " int 10h      " \
    parm [eax];


unsigned char kbd();
#pragma aux kbd = \
        "       in al, 60h     "       \
        value [al] \
        modify exact [];


void retrace ();
#pragma aux retrace = \
        "               mov dx,3dah     " \
        " @vert1:       in al,dx        " \
        "               test al,8       " \
        "               jz @vert1       " \
        " @vert2:       in al,dx        " \
        "               test al,8       " \
        "               jnz @vert2      " \
    modify [eax edx];


void pal(uint col, uint r, uint g, uint b);
#pragma aux pal = \
        "       mov   dx, 3c8h  " \
        "       out   dx, al    " \
        "       inc   dx        " \
        "       mov   al, bl    " \
        "       out   dx, al    " \
        "       mov   al, cl    " \
        "       out   dx, al    " \
        "       mov   al, dl    " \
        "       out   dx, al    " \
    parm [eax ebx ecx edx] \
    modify [eax ebx ecx edx];

void setpal( char *paletti );
#pragma aux setpal = \
        "     mov   dx, 3c8h    " \
        "     mov   al, 0       " \
        "     out   dx, al      " \
        "     inc   dx          " \
        "    xor    cx, cx      " \
        "@je:                   " \
        "     mov   al, [edi]   " \
        "     out   dx, al      " \
        "     mov   al, [edi+1] " \
        "     out   dx, al      " \
        "     mov   al, [edi+2] " \
        "     out   dx, al      " \
        "    add    di, 3       " \
        "    inc    cx          " \
        "    cmp    cx, 256     " \
        "    jne    @je         " \
    parm [edi] \
    modify [eax ecx edx edi];



void katopcx(char fln[80], char *paikka, int x, int y, char *paletti)
{
        int cox,co;
        unsigned char data,pcount,pcloop;
        int readoff, ptrot;
        unsigned char rr,gg,bb;

        ifstream pcxfile(fln, ios::binary);

        char *posso = (char *) malloc (200);
        char *kuvaan = (char *) paikka;

        pcxfile.read(posso, 128);

        ptrot=0;
        readoff=0;

        do {
                pcxfile.get(rr);
                data=rr;
                if ( (data & 0xC0) == 0xC0 ) {
                        ptrot++;
                        pcount=data & 0x3F;
                        pcxfile.get(rr);
                        data=rr;
                        for (pcloop=0;pcloop<pcount;pcloop++) {
                                kuvaan[readoff]=data;
                                readoff++;
                        }
                        cox+=pcount;
                } else {
                        kuvaan[readoff]=data;
                        readoff++;
                        cox++;
                }
                ptrot++;
        } while (readoff < x*y);

        pcxfile.get(rr);

        readoff = 0;
        for ( co=0; co<256; co++ ) {
                pcxfile.get(rr);
                pcxfile.get(gg);
                pcxfile.get(bb);
                rr = rr >> 2;
                gg = gg >> 2;
                bb = bb >> 2;
                paletti[readoff]=rr;
                paletti[readoff+1]=gg;
                paletti[readoff+2]=bb;
                readoff+=3;
        }
        pcxfile.close();
        setpal(paletti);
}


void load24bittga(char fln[80], char *paikka)
{
        int cox,co;
        unsigned char data,pcount,pcloop;
        int readoff, ptrot;
        unsigned char rr,gg,bb;

        ifstream tgafile(fln, ios::binary);


        char *posso = (char *) malloc (50);
        char *kuvaan = (char *) paikka;

        tgafile.read(posso, 44);
        tgafile.read(kuvaan, 1024*768);

        tgafile.close();
}




