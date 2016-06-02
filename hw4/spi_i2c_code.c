#include <xc.h>           // processor SFR definitions
#include <stdio.h>
#include <sys/attribs.h>  // __ISR macro
#include "spi_lib.h"
#include "spi_lib.c"

//#define CS1 LATBbits.LATB7

/*#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // free    up secondary osc pins  NOT SURE ON NTHIS ONE
#pragma config FPBDIV = DIV_1 // divide CPU freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // slowest wdt
#pragma config WINDIS = OFF // no wdt window
#pragma config FWDTEN = OFF // wdt off by default
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the CPU clock to 48MHz
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz - 8MHz/2 = 4MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV - 4MHz * 24 = 96 MHz
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz = 96MHz / 2 = 48 MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiply by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 13 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module */

int main()
{
	/* code */
    char volt = -1;
    char channel = 0; // 0 is for using VoutA; 1 is for using VoutB
    int i,j;
	spi_init();
    //TRISBbits.TRISB7 = 0;
    //setVoltage(channel,volt);
    while (1)
    {
        /*CS1 = 0;
        for (i=0;i<100000;++i)
        {;}
        CS1 = 1;
        for (j=0;j<100000;++j)
        {;}*/
	    setVoltage(channel,volt);
        //_CP0_SET_COUNT(0);                // set core timer counter to 0
        //while(_CP0_GET_COUNT()<10000)
        //    {;}
    }
	return 0;
}