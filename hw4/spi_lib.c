#pragma config DEBUG = OFF // no debugging
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
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module

#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include "spi_lib.h"
#define CS LATBbits.LATB7

void spi_init()
{
	/*SPI1CON = 0; //turn off SPI and reset it
	SPI1CONbits.MODE32 = 0; //Make it so 8 bits of data
	SPI1CONbits.MODE16 = 0;	//are sent per transfer 
    SPI1BRG=0xFFF; // Set desired clock freq. SPI1BRG = (Fpb/2*Fsck)-1
	SPI1CONbits.CKP = 1; //clk is idle when high, active when low
	SPI1CONbits.CKE = 1; //output data changes when clk transitions from active to idle
	//SPI1CONbits.SSEN = 1; // SS1 pin must be low for slave to be selected
	SPI1CONbits.MSTEN = 1; // PIC is the master SPI
    SPI1CONbits.ON = 1; //Enable the SPI peripheral */
    
    SPI1CON = 0;              // turn off the spi module and reset it
    SPI1BUF;                  // clear the rx buffer by reading from it
    SPI1BRG = 0xFFF;            // baud rate to ~5 kHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0;  // clear the overflow bit
    SPI1CONbits.CKE = 1;      // data changes when clock goes from hi to lo (since CKP is 0)
    //SPI1CONbits.MODE16 = 1;	// 16 bits of data are sent per transfer
    SPI1CONbits.MSTEN = 1;    // master operation
    SPI1CONbits.ON = 1;  
    
    ANSELB = 0x0000; // Make all pins digital not analog
    TRISBbits.TRISB7 = 0; // Make RB7 pin an output (the chip select pin) (RB7 is pin 16)
	//RPB7Rbits.RPB7R = 0b0011; //RB7 is SS1 for now....
    //TRISBbits.TRISB14 = 0;
	SDI1Rbits.SDI1R = 0b0100; //RB8 is SDI1 for now.... (RB8 is pin 17)
	RPB13Rbits.RPB13R = 0b0011; //RB13 (SDO1 pin) for now.... (RB13 is pin 24)   
}

// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) 
{
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) 
  { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}

void spi_write(unsigned char sent_value)
{
	unsigned char received_value;
	CS = 0; //tell slave to get ready for info
	SPI1BUF = sent_value; //send info to slave
	received_value = SPI1BUF; // clear SPIxBUF to allow it to send again. This value is read from the slave and doesnt mean anything
	CS = 1; //stop transferring information
}

unsigned char spi_read()
{
	unsigned char received_value, trash_value;
	CS = 0; //tell slave to get ready for info
	SPI1BUF = trash_value; // write a trash value to the slave
	received_value = SPI1BUF; //read the correct value from the slave
	CS = 1; //stop transferring information
	return received_value;
}

void setVoltage(char channel, char voltage)
{
	unsigned char first_8,second_8,unsign_channel,unsign_voltage,volt;
    //char volt;
    short int full_16_bars;
    
    unsign_channel = (unsigned) channel;
    unsign_voltage = (unsigned) voltage;
    volt = unsign_voltage;
    //TRISBbits.TRISB7 = 0; // Make RB7 pin an output (the chip select pin) (RB7 is pin 16)
	//RPB7Rbits.RPB7R = 0b0011; //RB7 is SS1 for now....
    //TRISBbits.TRISB14 = 0;
	//SDI1Rbits.SDI1R = 0b0100; //RB8 is SDI1 for now.... (RB8 is pin 17)
	//RPB13Rbits.RPB13R = 0b0011; //RB13 (SDO1 pin) for now.... (RB13 is pin 24)
    CS = 0;
    //full_16_bars=0x7700;
	//first_8=0x70;
    //second_8=0x00;
	first_8= 0x70 | (unsign_channel<<7); 
    first_8= first_8 | (unsign_voltage>>4);
	second_8=(volt<<4);
	spi_io(first_8);
	spi_io(second_8);
    //spi_io(full_16_bars);
    _CP0_SET_COUNT(0);                // set core timer counter to 0
    while(_CP0_GET_COUNT()<10000)
    {;}
    CS = 1;

}











