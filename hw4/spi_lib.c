#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include "spi_lib.h"
#define CS RPB9Rbits.RPB9R

void spi_init()
{
	SPI1CON = 0; //turn off SPI and reset it
	SPI1CONbits.ON = 1; //Enable the SPI peripheral
	SPI1CONbits.MODE32 = 0; //Make it so 8 bits of data
	SPI1CONbits.MODE16 = 0;	//are sent per transfer 
	SPI1CONbits.CKP = 1; //clk is idle when high, active when low
	SPI1CONbits.CKE = 1; //output data changes when clk transitions from active to idle
	SPI1CONbits.SSEN = 1; // SS1 pin must be low for slave to be selected
	SPI1CONbits.MSTEN = 1; // PIC is the master SPI
	RPB7Rbits.RPB7R = 0b0011; //RB7 is SS1 for now....
	SDI1Rbits.SDI1R = 0b0100; //RB8 is SDI1 for now....
	RPB13Rbits.RPB13R = 0b0011; //RB13 (SDO1 pin) for now....
}

// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
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
	CS = 1; //stop transfering information
}

unsigned char spi_read()
{
	unsigned char received_value, trash_value;
	CS = 0; //tell slave to get ready for info
	SPI1BUF = trash_value; // write a trash value to the slave
	received_value = SPI1BUF; //read the correct value from the slave
	CS = 1; //stop transfering information
	return received_value;
}

void setVoltage(char channel, char voltage)
{
	unsigned char first_8,second_8;
	first_8=0X70;
	first_8= (channel<<8) | (voltage>>4);
	second_8=(voltage<<4) | (0X0);
	spi_io(first_8);
	spi_io(second_8);
}











