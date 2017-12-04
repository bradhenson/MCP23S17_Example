/*
 * MCP23S17_Example.c
 *
 * Created: 12/4/2017 11:50:45 AM
 * 
 * The below code is based on a tutorial from the ermicroblog:
 * http://www.ermicro.com/blog/?p=1050 
 * 
 */ 
#define F_CPU 1600000UL
#include <avr/io.h>
#include <util/delay.h>

#define SPI_MOSI PB3
#define SPI_SCK PB5
#define SPI_SS PB2
#define SPI_PORT DDRB

// MCP23S17 SPI Slave Device
#define MCP_SLAVE_ID    0x40
#define MCP_SLAVE_ADDR  0x00      // A2=0,A1=0,A0=0
#define MCP_SLAVE_WRITE 0x00
#define MCP_SLAVE_READ  0x01

// MCP23S17 Registers Definition for BANK=0 (default)
#define IODIRA 0x00
#define IODIRB 0x01
#define IOCONA 0x0A
#define GPPUA  0x0C
#define GPPUB  0x0D
#define GPIOA  0x12
#define GPIOB  0x13

void MCP_Write(unsigned char addr,unsigned char data)
{
	// Activate the CS pin
	SPI_PORT &= ~(1 << SPI_SS);
	// Start MCP23S17 OpCode transmission
	SPDR = MCP_SLAVE_ID | ((MCP_SLAVE_ADDR << 1) & 0x0E)| MCP_SLAVE_WRITE;
	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));
	// Start MCP23S17 Register Address transmission
	SPDR = addr;
	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));

	// Start Data transmission
	SPDR = data;
	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));
	// CS pin is not active
	SPI_PORT |= (1 << SPI_SS);
}

unsigned char MCP_Read(unsigned char addr)
{
	// Activate the CS pin
	SPI_PORT &= ~(1 << SPI_SS);
	// Start MCP23S17 OpCode transmission
	SPDR = MCP_SLAVE_ID | ((MCP_SLAVE_ADDR << 1) & 0x0E)| MCP_SLAVE_READ;
	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));

	// Start MCP23S17 Address transmission
	SPDR = addr;
	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));

	// Send Dummy transmission for reading the data
	SPDR = 0x00;
	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));

	// CS pin is not active
	SPI_PORT |= (1 << SPI_SS);
	return(SPDR);
}
int main(void)
{
	// Initial the AVR ATMega328P SPI Peripheral
	// Set MOSI (PB3),SCK (PB5) and PB2 (SS) as output, others as input
	SPI_PORT = (1 << SPI_MOSI)|(1 << SPI_SCK)|(1 << SPI_SS);
	// CS pin is not active
	SPI_PORT |= (1<<SPI_SS);
	
	// Enable SPI, Master, set clock rate fck/2
	SPCR = (1 << SPE)|(1 << MSTR);
	SPSR |= (1 << SPI2X);

	// Initial the MCP23S17 SPI I/O Expander
	MCP_Write(IOCONA,0x28);   // I/O Control Register: BANK=0, SEQOP=1, HAEN=1 (Enable Addressing)
	MCP_Write(IODIRA,0x00);   // GPIOA As Output
	MCP_Write(IODIRB,0xFF);   // GPIOB As Input
	MCP_Write(GPPUB,0xFF);    // Enable Pull-up Resistor on GPIOB
	MCP_Write(GPIOA,0x00);    // Reset Output on GPIOA

	while(1) {

	
	
	//Example of a Write Command
	MCP_Write(GPIOA, (1 << 0));
	
	//Example of a Read Command
	MCP_Read(GPIOB & (1 << 0));



	}
	return 0;
}
