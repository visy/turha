#include <midasdll.h>
#include <midas.h>
#include <vu.h>


volatile unsigned   count, count2;
volatile unsigned   count3, count4;


void MIDAS_CALL prevr(void)
{
        count++;
        count2++;
        count3++;
        count4++;
}

unsigned        position;               /* Current position */
unsigned        pattern;                /* Current pattern number */
unsigned        row;                    /* Current row number */
int             syncInfo;               /* Music synchronization info */

gmpModule       *module;                /* current playing module */
gmpInformation  *info;                  /* current module playing info */
MIDASmodulePlayHandle playHandle;       /* Das Playing Handle */
static unsigned refreshRate;            /* Display refresh rate */
int error;

uint MIDASvuMeter(int i)
{
        static unsigned  meter;
        static unsigned  position, volume, sample;
        static ulong     rate;
        gmpChannel  *chan;

        chan = &info->channels[i];

            /* Get playing rate for channel: */
        if ( (error = midasSD->GetRate(i, &rate)) != OK )
                midasError(error);

            /* Get playing position for channel: */
        if ( (error = midasSD->GetPosition(i, &position)) != OK )
                midasError(error);

            /* Get current sample handle for channel: */
        if ( (error = midasSD->GetSample(i, &sample)) != OK )
                midasError(error);

            /* Get current volume for channel: */
        if ( (error = midasSD->GetVolume(i, &volume)) != OK )
                midasError(error);

        if ( (sample > 0) && (rate > 0) ) {
                /* Get real VU meter value: */
                if ( (error = vuMeter(sample-1, rate, position, volume,
                        &meter)) != OK )
                        midasError(error);
        } else meter = 0;
        return meter;
}

void MIDASerror(void)
{
        mode(3);
        cout << "MIDAS error: %s\n" << MIDASgetErrorMessage(MIDASgetLastError());
        MIDASclose(); /*!!!!!*/
        exit(EXIT_FAILURE);
}

void MIDAS_CALL UpdateInfo(void)
{
    /* MIDAS_CALL is cdecl for Watcom, empty for DJGPP. Helps calling this
       from assembler, otherwise unnecessary */
    
    static MIDASplayStatus status;

    /* Get playback status: */
    if ( !MIDASgetPlayStatus(playHandle, &status) )
        MIDASerror();

    /* Store interesting information in easy-to-access variables: */
    position = status.position;
    pattern = status.pattern;
    row = status.row;
    syncInfo = status.syncInfo;
}

int CALLING MakeMeter(sdSample *sdsmp, gmpSample *gmpsmp)
{
    return vuPrepare(sdsmp, gmpsmp->sdHandle - 1);
}



int musainit()
{
        midasSetDefaults();
        MIDASstartup();
        if (!MIDASdetectSoundCard()) MIDASerror();
        refreshRate = MIDASgetDisplayRefreshRate();
        if ( refreshRate == 0 ) {
/*                cout << "Warning! Unable to synchronize to display refresh!\n";
                if ( getch() == 27 ) return 0;*/
                refreshRate = 70000; /* default 70Hz */
        }
        if ( !MIDASinit() ) MIDASerror();
        if ( (error = vuInit()) != OK )
                midasError(error);
        if ( !MIDASsetTimerCallbacks(refreshRate, TRUE, &prevr, NULL, NULL) )
                MIDASerror();
        return 0;
}
void musa(char moduleName[])
{
        musainit();
//        if ( (module = MIDASloadModule(moduleName)) == NULL ) MIDASerror();
        if ( (error = gmpLoadIT(moduleName, 1, &MakeMeter, &module)) != OK )
                midasError(error);
        if ( (playHandle = MIDASplayModule(module, TRUE)) == 0 ) MIDASerror();
}
void musapois()
{
        if ( !MIDASsetMusicSyncCallback(playHandle, NULL) )
                MIDASerror();
        for ( int i = 0; i < MAXSAMPLES; i++ )
                if ( (error = vuRemove(i)) != OK ) midasError(error);
        if ( (error = vuClose()) != OK )
                midasError(error);
        if ( !MIDASstopModule(playHandle) ) MIDASerror();
        if ( !MIDASfreeModule(module) ) MIDASerror();
        if ( !MIDASremoveTimerCallbacks() )
                MIDASerror();
        if ( !MIDASclose() ) MIDASerror();
}

