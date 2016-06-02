#include <xc.h>           // processor SFR definitions
#include <stdio.h>
#include <sys/attribs.h>  // __ISR macro
#include "spi_lib.h"
#include "spi_lib.c"

unsigned char numshift(char channel, char voltage);

int main()
{
	/* code */
	char voltage,channel;
    unsigned char value;
	while(1)
	{
		printf("What is the voltage you want bruh?\n\r");
		scanf("%s\n\r",voltage);

		printf("What is the channel you want bruh? 0 is VoutA 1 is VoutB\n\r");
		scanf("%s",channel);

        printf("Voltage is= %s, Channel is= %s\n\r",voltage,channel);
        value=numshift(channel,voltage);
        
        printf("Shifted char is = %s\n\r",value);
	}
	return 0;
}

unsigned char numshift(char channel, char voltage)
{
    unsigned char first_8,second_8;
	first_8=0X70;
	first_8= (channel<<8) | (voltage>>4);
	second_8=(voltage<<4) | (0X0);
	return first_8;
}