#include"spi.h"


/*
atmega8 datasheet, page 123
SPCR = SPIE, SPE, DORD, MSTR, CPOL, CPHA, SPR1, SPR0 (D7...D0)
SPSR = SPIF, WCOL, -, -, -, -, -, SPI2x (D7...D0)

SPI2X SPR1 SPR0 SCK_freq
0 0 0 fosc/4
0 0 1 fosc/16
0 1 0 fosc/64
0 1 1 fosc/128
1 0 0 fosc/2
1 0 1 fosc/8
1 1 0 fosc/32
1 1 1 fosc/64
*/

void SPI_MasterInit(void)
{
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS); // MOSI, SCK, SS su output
	DDR_SPI &=~(1<<DD_MISO);
	SPCR = (1<<SPE)|(1<<MSTR);			// enable SPI, SPI master
	SPCR |=(1<<SPR0);			// fosc/16



	PORTB &=~(1<<DD_SS);  //active low as a default
}

char SPI_Transmit(char cData)
{

	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

	return SPDR;	// Za GLCD
}

void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDR_SPI = (1<<DD_MISO);
	/* Enable SPI */
	SPCR = (1<<SPE);
}

//char SPI_SlaveReceive(void)
//{
//	/* Wait for reception complete */
//	while(!(SPSR & (1<<SPIF)))
//	;
//	/* Return data register */
//	return SPDR;
//}
