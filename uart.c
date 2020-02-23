
#include "uart.h"
#include "uartcfg.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include<stdlib.h>

#define TX_BIT	(1 << UART_TX_BIT)

#define BIT_US	(1000000/UART_BAUD)	/*!< Bit period in microseconds */

/*!
 * Configure the UART pins.
 */
void uart_init() {
	UART_TX_DDR	|= TX_BIT;
	UART_TX_PORT	|= TX_BIT;
}

/*!
 * Send a raw byte to the UART.
 */
void uart_tx(uint8_t byte) {
	uint8_t mask = 1;
	/* Start bit */
	UART_TX_PORT &= ~TX_BIT;	_delay_us(BIT_US);
	while(mask) {
		if (mask & byte)
			UART_TX_PORT |= TX_BIT;
		else
			UART_TX_PORT &= ~TX_BIT;
		_delay_us(BIT_US);
		mask <<= 1;
	}
	/* Stop bit */
	UART_TX_PORT |= TX_BIT;	_delay_us(BIT_US);
}


void uart_TX_string( char* str) {
	int i = 0;
	while (str[i]!='\0')
	{
		uart_tx(str[i]);
        i++;
	}
}




void uart_TX_number(int number)
{
   char buffer[20];

   itoa(number,buffer,10);
   uart_TX_string(buffer);


}




/*!
 * Send a text string (in pgmspace) to the UART.
 * String shall be null-terminated!
 */
void uart_tx_str(const char* str) {
	char c = pgm_read_byte(str);
	while (c) {
		str++;
		uart_tx(c);
		c = pgm_read_byte(str);
	}
}

/*!
 * Send a 4-bit nybble to the UART.
 */
void uart_tx_hex(uint8_t nybble) {
	if (nybble >= 10)
		uart_tx(nybble - 10 + 'a');
	else
		uart_tx(nybble + '0');
}

/*!
 * Send a 8-bit byte to the UART.
 */
void uart_tx_hex_byte(uint8_t byte) {
	uart_tx_hex(byte >> 4);
	uart_tx_hex(byte & 0x0f);
}

/*!
 * Send a 16-bit word to the UART.
 */
void uart_tx_hex_word(uint16_t word) {
	uart_tx_hex_byte(word >> 8);
	uart_tx_hex_byte(word & 0xff);
}

/*!
 * Send a 32-bit long word to the UART.
 */
void uart_tx_hex_lword(uint32_t lword) {
	uart_tx_hex_word(lword >> 16);
	uart_tx_hex_word(lword & 0xffff);
}
