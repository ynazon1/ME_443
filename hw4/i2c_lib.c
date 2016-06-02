// I2C Master utilities, 100 kHz, using polling rather than interrupts
// The functions must be called in the correct order as per the I2C protocol
// Change I2C2 to the I2C channel you are using
// I2C pins need pull-up resistors, 2k-10k

void i2c_master_setup(void) {
  I2C2BRG = some number for 100kHz;            // I2CxBRG = [1/(2*Fsck) - PGD]*Pblck - 2 
                                    // look up PGD for your PIC32
  I2C2CONbits.ON = 1;               // turn on the I2C1 module
}

// Start a transmission on the I2C bus
void i2c_master_start(void) {
    I2C2CONbits.SEN = 1;            // send the start bit
    while(I2C2CONbits.SEN) { ; }    // wait for the start bit to be sent
}

void i2c_master_restart(void) {     
    I2C2CONbits.RSEN = 1;           // send a restart 
    while(I2C2CONbits.RSEN) { ; }   // wait for the restart to clear
}

void i2c_master_send(unsigned char byte) { // send a byte to slave
  I2C2TRN = byte;                   // if an address, bit 0 = 0 for write, 1 for read
  while(I2C2STATbits.TRSTAT) { ; }  // wait for the transmission to finish
  if(I2C2STATbits.ACKSTAT) {        // if this is high, slave has not acknowledged
    // ("I2C2 Master: failed to receive ACK\r\n");
  }
}

unsigned char i2c_master_recv(void) { // receive a byte from the slave
    I2C2CONbits.RCEN = 1;             // start receiving data
    while(!I2C2STATbits.RBF) { ; }    // wait to receive the data
    return I2C2RCV;                   // read and return the data
}

void i2c_master_ack(int val) {        // sends ACK = 0 (slave should send another byte)
                                      // or NACK = 1 (no more bytes requested from slave)
    I2C2CONbits.ACKDT = val;          // store ACK/NACK in ACKDT
    I2C2CONbits.ACKEN = 1;            // send ACKDT
    while(I2C2CONbits.ACKEN) { ; }    // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) {          // send a STOP:
  I2C2CONbits.PEN = 1;                // comm is complete and master relinquishes bus
  while(I2C2CONbits.PEN) { ; }        // wait for STOP to complete
}

void initExpander()
{
    //i2c_master_setup();
    i2c_master_start();
    i2c_master_send();
    i2c_master_send();
    i2c_master_send();
}

void setExpander(char pin, char level, unsigned char chip_addr, unsigned char register_addr)
{
    i2c_write(chip_addr, register_addr, pin);
    i2c_write(chip_addr, register_addr, level);
}

char getExpander(unsigned char chip_addr, unsigned char register_addr)
{
    unsigned char expander_info;
    i2c_read(chip_addr,register_addr);
    return expander_info;
}

void i2c_init(void)
{
    i2c_master_setup();
}

void i2c_write(unsigned char chip_addr, unsigned char register_addr, unsigned char info_to_send)
{
    i2c_master_start();
    i2c_master_send(chip_addr << 1); 
    i2c_master_send(register_addr);
    i2c_master_send(info_to_send);
    //i2c_master_restart();
    i2c_master_stop();
}

unsigned char i2c_read(unsigned char chip_addr, unsigned char register_addr)
{
    unsigned char info_received;
    
    i2c_master_start();
    i2c_master_send(chip_addr << 1); 
    i2c_master_send(register_addr);
    i2c_master_restart();
    i2c_master_send((chip_addr << 1) | 1);
    //i2c_master_send(register_addr);    (SLAVE_ADDR << 1) | 1));
    info_received = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return info_received;
}