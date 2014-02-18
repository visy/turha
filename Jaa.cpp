
//#include <ctype.h>
//#include <sys/types.h>
//#include <sys/stat.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <fcntl.h>
//#include <dos.h>
//#include "omat.cpp"
//#include "musa.cpp"
#include <time.h>
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

//#include "fmod.hpp"
//#include "fmod_errors.h"

#include "xmp.h"

using namespace std;
size_t
strlcpy(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}

size_t
strlcat(char *dst, const char *src, size_t siz)
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));	/* count does not include NUL */
}

        char * paikka;
        int * paikka_;

SDL_Surface *screen;


		int framenum=0;
        int a;
        int tyypit[8];
        int uudestaan = 0;
		unsigned int position = 0;
		unsigned int row = 0;
		unsigned char counter = 0;

        int kola = 0;
        
		int viimeiseen; // efektiin. uudestaan=2
        char * grafiikka_naytto;
        int  * grafiikka_naytto_temp;
        char * paikka2;
        int * paikka2_;
        char * paikka3;
        int * paikka3_;
        char *kuva[21];
		char *maisema1;
		char *paletti;
		char *paletti2;
/*
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}
*/
void render()
{   
  // Lock surface if needed
  if (SDL_MUSTLOCK(screen)) 
    if (SDL_LockSurface(screen) < 0) 
      return;

  // Ask SDL for the time in milliseconds
  int tick = SDL_GetTicks();

  // Declare a couple of variables
  int i, j, yofs, ofs;

  // Draw to screen
  yofs = 0;
  int pix = 0;
  for (i = 0; i < 200*4; i++)
  {
    for (j = 0;j < 320; j++)
    {
      ((unsigned char*)screen->pixels)[320*i+j] = paikka[pix];
	  pix++;
    }
	i++;
	pix-=320;
    for (j = 0;j < 320; j++)
    {
      ((unsigned char*)screen->pixels)[320*i+j] = paikka[pix];
	  pix++;
    }
	i++;
	pix-=320;
    for (j = 0;j < 320; j++)
    {
      ((unsigned char*)screen->pixels)[320*i+j] = paikka[pix];
	  pix++;
    }
	i++;
	pix-=320;
    for (j = 0;j < 320; j++)
    {
      ((unsigned char*)screen->pixels)[320*i+j] = paikka[pix];
	  pix++;
    }
  }

  // Unlock if needed
 if (SDL_MUSTLOCK(screen)) 
    SDL_UnlockSurface(screen);

  // Tell SDL to update the whole screen
  //SDL_UpdateRect(screen, 0, 0, 320, 200);    
  SDL_Flip(screen);
}

void flip( char *source, char *desti )
{
        for (int q=0; q<64000; q++) {
                desti[q]=source[q];
        }
}


void cls( char *paik )
{
        int *p = (int*) paik;
        for (int a=0;a<16000; a++) p[a]=0;
}


void zoom( int sx1, int sy1, int sx2, int sy2,
           int dx1, int dy1, int dx2, int dy2,
           char *sourc, char *desti )
{
        int deltax, deltay, deltax2, deltay2;
        int q,w;
        int e, aa;
        int annax,valmix, annay,valmiy;
        deltax=dx2-dx1;
        deltay=dy2-dy1;
        deltax2=sx2-sx1;
        deltay2=sy2-sy1;
        desti+=dy1*320+dx1;
        aa = ( (float) deltay2 / deltay * 256 );
        annay = aa;
        aa = ( (float) deltax2 / deltax * 256 );
        annax = aa;
        valmiy=0;
        for (int a=dy1; a<(dy2+1); a++) {
                e=(valmiy >> 8) + sy1;
                e=(e << 8) + (e << 6) + sx1;
                valmix = 0;
                for (int b=dx1; b<(dx2+1); b++) {
                        w=(valmix >> 8) + e;
                        *desti++ = sourc[w];
                        valmix = valmix + annax;
                }
                desti+=(319-deltax);
                valmiy = valmiy + annay;
        }
}

void transpic( char *sour, char *desti )
{
        for (int a=0; a<64000; a++) {
                char o = sour[a];
                if (o>0) desti[a]=o;
        }
}

void copy( int sx1, int sy1, int sx2, int sy2, int dx, int dy, char *s, char *d )
{
        int w = dy*320+dx-sx1;
        for (int a=sy1; a<sy2; a++) {
                int q = (a << 8) + (a << 6);
                for (int b=sx1; b<sx2; b++) {
                        d[w+b]=s[q+b];
                }
                w+=320;
        }
}


void copytrans( int sx1, int sy1, int sx2, int sy2, int dx, int dy, char *s, char *d )
{
        int w = dy*320+dx-sx1;
        for (int a=sy1; a<sy2; a++) {
                int q = (a << 8) + (a << 6);
                for (int b=sx1; b<sx2; b++) {
                        char o = s[q+b];
                        if (o>0) d[w+b]=o;
                }
                w+=320;
        }
}

void setpal( char *paletti ) 
{
		SDL_Color colors[256];

		int co = 0;
        for(int i=0;i<256;i++){
          colors[i].r=paletti[co];
          colors[i].g=paletti[co+1];
          colors[i].b=paletti[co+2];
		  co+=3;
        }

		SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);
}

#define GLubyte unsigned char
#define GLushort unsigned short

#pragma pack(1)
/* PCX header */
struct pcx_header_t
{
  GLubyte manufacturer;
  GLubyte version;
  GLubyte encoding;
  GLubyte bitsPerPixel;

  GLushort xmin, ymin;
  GLushort xmax, ymax;
  GLushort horzRes, vertRes;

  GLubyte palette[48];
  GLubyte reserved;
  GLubyte numColorPlanes;

  GLushort bytesPerScanLine;
  GLushort paletteType;
  GLushort horzSize, vertSize;

  GLubyte padding[54];
};


void katopcx(char fln[80], char *paikka, int sx, int sy, char *paletti)
{
  struct pcx_header_t header;
  FILE *fp = NULL;
  int biter;

  /* Open image file */
  fp = fopen (fln, "rb");
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", fln);
      return;
    }

  /* Read header file */
  fread (&header, sizeof (struct pcx_header_t), 1, fp);
  if (header.manufacturer != 0x0a)
    {
      fprintf (stderr, "error: bad version number! (%i)\n",
	       header.manufacturer);
      return;
    }

  int rle_counter = 0, rle_value = 0;
  GLubyte palette[768];
  GLubyte magic;
  GLubyte *ptr;
  fpos_t curpos;
  int y, bytes;

  /* The palette is contained in the last 769 bytes of the file */
  fgetpos (fp, &curpos);
  fseek (fp, -769, SEEK_END);
  magic = fgetc (fp);

  /* First byte must be equal to 0x0c (12) */
  if (magic != 0x0c)
    {
      fprintf (stderr, "error: colormap's first byte must be 0x0c! "
	       "(%#x)\n", magic);

      return;
    }

  /* Read palette */
  fread (paletti, sizeof (char), 768, fp);
  fsetpos (fp, &curpos);

  int ind = 0;
  /* Read pixel data */
  for (y = 0; y < sy; ++y)
    {
      bytes = header.bytesPerScanLine;

      /* Decode line number y */
      while (bytes--)
	{
	  if (rle_counter == 0)
	    {
	      if( (rle_value = fgetc (fp)) < 0xc0)
		{
		  rle_counter = 1;
		}
	      else
		{
		  rle_counter = rle_value - 0xc0;
		  rle_value = fgetc (fp);
		}
	    }

	  rle_counter--;

	  paikka[ind] = rle_value;
      ind++;
	}
    }

}

/*
  FMOD::System     *systemah;
  FMOD::Sound      *sound;
      FMOD::Channel    *channel = 0;
*/
int MIDASvuMeter(int i) {
  return (int)12+((cos(i+SDL_GetTicks()*0.01)*8.0)*(sin(i+SDL_GetTicks()*0.02)));
}


static void fill_audio(void *udata, unsigned char *stream, int len)
{
    xmp_play_buffer((xmp_context)udata, stream, len, 0);
}

int sound_init(xmp_context ctx, int sampling_rate, int channels)
{
    SDL_AudioSpec a;

    a.freq = sampling_rate;
    a.format = (AUDIO_S16);
    a.channels = channels;
    a.samples = 2048;
    a.callback = fill_audio;
    a.userdata = ctx;

    if (SDL_OpenAudio(&a, NULL) < 0) {
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
    }
	return 0;
}

xmp_context ctx;

void mainloop() {

  //           systemah->update();
                // UpdateInfo();
                framenum++;

			xmp_frame_info info;
			xmp_get_frame_info(ctx,&info);
			row = info.row;
			position = info.pos;

//        channel->getPosition(&row, FMOD_TIMEUNIT_MODROW);
//        channel->getPosition(&position, FMOD_TIMEUNIT_MODORDER);

				counter++;

                if ( (uudestaan==1) & (position<1) ) {
                        int i;
                        i = (row/4) && 3;
                        if (i==0) {
                                flip( kuva[18], paikka );
                        } else if (i==1) {
                                flip( kuva[19], paikka );
                        } else {
                                flip( kuva[20], paikka );
                        }
                } else if ( (uudestaan==1) & (position<3) ) {
                        int x=64;
                        if (position == 1) x=0;
                        x+=row;

                        int q=0;
                        int w=x;
                        for (int b=0; b<200; b++) {
                                for (int c=0; c<320; c++) {
                                        paikka[q]=maisema1[w];
                                        q++;
                                        w++;
                                }
                                w+=320;
                        }
                                q=32000+x*2;
                                w=0;
                                for(int a=0;a<100;a++) {
                                        for(int b=0;b<160;b++) {
                                                int o = kuva[7][w];
                                                if (o!=0) paikka[q]=o;
                                                q++;
                                                w++;
                                        }
                                        q+=160;
                                        w+=160;
                                }

                } else if ( (uudestaan==2) & (position<1) ) {
                        int k, k2;
                        int a = counter + viimeiseen;
                        k = 10 + 10 * sin( (float) a/20 );
                        k2 = 13 + 13 * cos( (float) a/20 );
                        zoom( k2, k, 319, 199, 0, 0, 319-k2, 199-k, kuva[20], paikka );
                        viimeiseen += (MIDASvuMeter(0)+MIDASvuMeter(1)+MIDASvuMeter(2));

                } else if (position==0) {
                        if (row<32) {
                                int w;
                                int q=16080;
                                if (row<8) {
                                        w=0;
                                } else if (row<16) {
                                        w=160;
                                } else if (row<24) {
                                        w=32000;
                                } else if (row<32) {
                                        w=32160;
                                }
                                for (int a=0; a<99; a++) {
                                        for (int b=0; b<159; b++) {
                                                char o=kuva[7][w];
                                                paikka[q]=o;
                                                q++;
                                                w++;
                                        }
                                        q+=161;
                                        w+=161;
                                }
                        } else {
                                int k = MIDASvuMeter(0)+MIDASvuMeter(1)+MIDASvuMeter(2)+20;
								flip( kuva[1], paikka );
                                for (int a=0; a<769; a++) {
                                        int b = paletti[a];
                                        b = b * k >> 5;
                                        if (b>63) b=63;
                                        paletti2[a] = b;
                                }
                                setpal(paletti2);
                        }
                } else if (position<3) {
                        setpal(paletti);
                        int o = MIDASvuMeter(2);
                        int k = MIDASvuMeter(0)+MIDASvuMeter(1)+o;
						int k2 = k*1.4;
                        zoom( k2, k, 319-k2, 199-k, 0, 0, 319, 199, kuva[13], paikka );
                        if (position==2) {
                                if (o>5) {
                                        if (row<32) {
                                                copytrans( 80, 80, 280, 120, 0, 0, kuva[15], paikka );
                                        } else {
                                                copytrans( 80, 50, 280, 110, 110, 130, kuva[15], paikka );
                                        }
                                }
                        }
                } else if (position<5) {
                        if ((row>39)&(row<53)) {
                                zoom( 30, 30, 289, 169, 0, 0, 319, 199, kuva[10], paikka );
                        } else {
                                flip( kuva[10], paikka );
                        }
                } else if (position<7) {
                        flip( kuva[2], paikka );
                        int q=0;
                        char p = row >> 3;
                        for (int a=0; a<199; a++) {
                                int m = 128 + 127*sin((float)(a+counter));
                                for (int b=0; b<319; b++) {
                                        char o = kuva[4][q];
                                        if ((o>0)&(o<=p)) {
                                                paikka[q-1]=0;
                                                paikka[q]=o;
                                        } else {
                                                if (kuva[2][q]==0) {
                                                        int n = 128 + 127*sin((float)(b+counter));
                                                        int c = (n<<8)+m;
                                                        paikka[q]=paikka3[c];
                                                }
                                        }
                                        q++;
                                }
                        }
                } else if (position<9) {
                        int d = MIDASvuMeter(3)+MIDASvuMeter(4)+MIDASvuMeter(5);
						zoom( d, 0, 319-d, 199, 0,0,319,199, kuva[12], paikka );
                } else if (position<10) {
                        if (row<4) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[5], paikka);
                        } else if (row<8) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[6], paikka);
                        } else if (row<16) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[14], paikka);
                        } else if (row<24) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[5], paikka);
                        } else if (row<28) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[6], paikka);
                        } else if (row<32) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[14], paikka);
                        } else if ((row>51) & (row<58)) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+ja, 189+jo,
                                      0, 0, 319, 199, kuva[5], paikka);
                        } else {
                                int ka = rand()%40;
                                int ko = rand()%30;
                                zoom( ka, ko, 160+ka, 100+ko,
                                      0,0,319,199, kuva[10],paikka);
                                int q=0;
                                for(int a=0;a<100;a++) {
                                        for(int b=0;b<160;b++) {
                                                int o = kuva[7][q+32160];
                                                if (o!=0) paikka[q+24120]=o;
                                                q++;
                                        }
                                        q+=160;
                                }
                        }
                } else if (position<11) {
                        if (row<4) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[1], paikka);
                        } else if (row<8) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[2], paikka);
                        } else if (row<16) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[3], paikka);
                        } else if (row<24) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[4], paikka);
                        } else if (row<28) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[5], paikka);
                        } else if (row<32) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+jo, 189+ja,
                                      0, 0, 319, 199, kuva[6], paikka);
                        } else if ((row>51) & (row<58)) {
                                int jo = rand()%10;
                                int ja = rand()%10;
                                zoom( jo, ja, 309+ja, 189+jo,
                                      0, 0, 319, 199, kuva[7], paikka);
                        } else {
                                int ka = rand()%40;
                                int ko = rand()%30;
                                zoom( ka, ko, 160+ka, 100+ko,
                                      0,0,319,199, kuva[10],paikka);
                                int q=0;
                                for(int a=0;a<100;a++) {
                                        for(int b=0;b<160;b++) {
                                                int o = kuva[7][q+160];
                                                if (o!=0) paikka[q+24120]=o;
                                                q++;
                                        }
                                        q+=160;
                                }
                        }
                        if (kola==0) {
                                if (uudestaan<2) {
                                        uudestaan++;
                                } else {
                                        uudestaan = 0;
                                }
                                kola = 1;
                        }
                } else if (position<12) {
                        kola = 0;
                        flip(kuva[8],paikka);
                        int q=0;
                        int p = 70*320+row*2;
                        for (int a=0; a<100; a++) {
                                for (int b=0; b<160; b++) {
                                        int o = kuva[7][q+32000];
                                        if (o!=0) paikka[q+p]=o;
                                        q++;
                                }
                                q+=160;
                        }
                } else if (position<13) {
                        flip(kuva[8],paikka);
                        int q=0;
                        int p = 70*320+160-(row*2);
                        for (int a=0; a<100; a++) {
                                for (int b=0; b<160; b++) {
                                        if (kuva[8][q+p]==0)
                                             paikka[q+p]=kuva[7][q+32000];
//                                        int o = kuva[7][q+32000];
//                                        if (o!=0) paikka[q+p]=o;
                                        q++;
                                }
                                q+=160;
                        }
                } else if (position<15) {
                        if (rand()%2==1) {
                                int a=rand()%4;
                                tyypit[a]=rand()%2;
                        }
                        flip(kuva[16],paikka);
                        int q=0;
                        for (int a=0; a<200; a++) {
                                for (int b=0; b<4; b++) {
                                        if (tyypit[b]==0) for (int c=0; c<(320/4); c++) {
                                                paikka[q+c]=kuva[17][q+c];
                                        }
                                        q+=(320/4);
                                }
                        }
                }

                //retrace();
                //flip( paikka, naytto );
				render();
}

int main(int argc, char *argv[])
{

  // Initialize SDL's subsystemahs - in this case, only video.
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0 ) 
  {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }

  // Register SDL_Quit to be called at exit; makes sure things are
  // cleaned up when we quit.
  atexit(SDL_Quit);

/*
    FMOD_RESULT       result;
    unsigned int      version;
    result = FMOD::System_Create(&systemah);
    ERRCHECK(result);

    result = systemah->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }

    result = systemah->init(32, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);

        result = systemah->createSound("isja.it", FMOD_SOFTWARE, 0, &sound);
    ERRCHECK(result);
  */  
  // Attempt to create a 320x200 window with 256 color pixels.
  screen = SDL_SetVideoMode(320, 200, 32, SDL_SWSURFACE);
  SDL_WM_SetCaption("Turha on porttia avata", NULL);

  // If we fail, return error.
  if ( screen == NULL ) 
  {
    fprintf(stderr, "Unable to set 320x200 video: %s\n", SDL_GetError());
    exit(1);
  }

        //char * naytto = (char *) 0xa0000;
        //int  * naytto_ = (int *) 0xa0000;
        paletti = (char *) malloc (768);
        paletti2 = (char *) malloc (768);

        grafiikka_naytto = (char *) malloc (64000*4);
        paikka = (char *) malloc (66000*4);
        paikka2 = (char *) malloc (66000*4);
        paikka3 = (char *) malloc (66000*4);

        paikka2_ = (int *) paikka2;
        paikka_ = (int *) paikka;
        paikka3_ = (int *) paikka3;


        maisema1 = (char *) malloc (128000*4);
        for (int cm=0; cm<21; cm++) {
                kuva[cm] = (char *) malloc (64000*4);
        }

        katopcx( "data/talopaa.pcx", maisema1, 640, 200, paletti );

//        mode(0x13);

        katopcx("data/mapp.pcx", paikka3, 256, 256, paletti );

        katopcx("data/kk.pcx",       kuva[ 0],320,200,paletti);
        katopcx("data/iu.pcx",       kuva[ 1],320,200,paletti);
        katopcx("data/efup.pcx",     kuva[ 2],320,200,paletti);
        katopcx("data/taust1.pcx",   kuva[ 3],320,200,paletti);
        katopcx("data/teksy.pcx",    kuva[ 4],320,200,paletti);
        katopcx("data/sotku2.pcx",   kuva[ 5],320,200,paletti);
        katopcx("data/sotku3.pcx",   kuva[ 6],320,200,paletti);
        katopcx("data/tyypit.pcx",   kuva[ 7],320,200,paletti);
        katopcx("data/oo.pcx",       kuva[ 8],320,200,paletti);
        katopcx("data/taus.pcx",     kuva[ 9],320,200,paletti);
        katopcx("data/ovi.pcx",      kuva[10],320,200,paletti);
        katopcx("data/tunele.pcx",   kuva[11],320,200,paletti);
        katopcx("data/nunn.pcx",     kuva[12],320,200,paletti);
        katopcx("data/tyyp.pcx",     kuva[13],320,200,paletti);  // 0, naama
        katopcx("data/tek2.pcx",     kuva[14],320,200,paletti);
        katopcx("data/iso.pcx",      kuva[15],320,200,paletti);  // 2, vilk
        katopcx("data/mand1.pcx",    kuva[16],320,200,paletti);
        katopcx("data/mand2.pcx",    kuva[17],320,200,paletti);
        katopcx("data/vass1.pcx",    kuva[18],320,200,paletti);
        katopcx("data/vass2.pcx",    kuva[19],320,200,paletti);
        katopcx("data/vass3.pcx",    kuva[20],320,200,paletti);
        setpal( paletti );

//        mov_hookup_09h();

        int sin1[256];

        for (int kgo=0; kgo<255; kgo++) {
                float pff;
                pff=sin((float)kgo*M_PI/256);
                sin1[kgo]=(int) (pff*32);
        }



//        musa( "isja.it" );
//        MIDASsetPosition( playHandle, 12 );
/*
  result = systemah->playSound(FMOD_CHANNEL_FREE, sound, 0, &channel);
  ERRCHECK(result);

  channel->setPosition(12, FMOD_TIMEUNIT_MODORDER);

*/

    if ((ctx = xmp_create_context()) == NULL)
		            return 1;

	sound_init(ctx, 44100, 2);
    xmp_load_module(ctx, "data/isja.it");
 


	clock_t time1, time2;
	time1 = clock();
    
        cls( paikka );

        for (a=0;a<8;a++) tyypit[a]=0;


	xmp_start_player(ctx, 44100, 0);
	xmp_set_position(ctx,12);
	SDL_PauseAudio(0);

	#ifdef EMSCRIPTEN
	  emscripten_set_main_loop(mainloop, 60, 1);
	#else
	while (1==1){mainloop();}
#endif

        time2 = clock();
        //musapois();

    SDL_PauseAudio(1);

    xmp_end_player(ctx);
    xmp_release_module(ctx);
    xmp_free_context(ctx);

    SDL_CloseAudio();

        //mov_release_09h();


        //mode(3);

}
