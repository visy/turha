/*****************************************************************************\
*                                                                             *
* <MOV_TIMER_UNIT>              /no libs.                                     *
* timerlimit = 16                                                             *
*                                                                             *
* MOV_TIMER_INIT (int system);                                                *
*       initializes all counters if 0                                         *
*       but if 1-16 then it will initialize only that counter which system is.*
*                                                                             *
* MOV_TIMER_ELAPSED (int system1);                                            *
*       How much time has elapsed when initialized? in counter number system1 *
*                                                                             *
* MOV_TIMER ();                                                               *
*       Gives all time that has passed trough 00:00:00                        *
*                                                                             *
* (under development) (much more timers / counters, mins, hours etc stuff)    *
\*****************************************************************************/
extern "C"
{
    int  mov_timer_init (int system=0);
    int  mov_timer_elapsed (int system1=0);
    int  mov_timer ();
}
