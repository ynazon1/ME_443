#include <xc.h>           // processor SFR definitions
#include <stdio.h>
#include <sys/attribs.h>  // __ISR macro
#include "spi_lib.h"
#include "spi_lib.c"

int main()
{
	/* code */
	spi_init();
	setVoltage(1,255);
	return 0;
}