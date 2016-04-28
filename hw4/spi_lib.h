#ifndef SPI_LIB
#define SPI_LIB 

void SPI_init();
void SPI_write();
void SPI_read();
void setVoltage(char channel, char voltage);
unsigned char spi_io(unsigned char o);

#endif