#ifndef _UART_H
#define _UART_H



#include <stdint.h>

/*!
 * Configure the UART pins.
 */
void uart_init();

/*!
 * Send a raw byte to the UART.
 */
void uart_tx(uint8_t byte);


void uart_TX_string( char* str);

void uart_TX_number(int number);
/*!
 * Send a text string (in pgmspace) to the UART.
 * String shall be null-terminated!
 */
void uart_tx_str(const char* str);

/*!
 * Send a 8-bit byte to the UART.
 */
void uart_tx_hex_byte(uint8_t byte);

/*!
 * Send a 16-bit word to the UART.
 */
void uart_tx_hex_word(uint16_t word);

/*!
 * Send a 32-bit long word to the UART.
 */
void uart_tx_hex_lword(uint32_t lword);
#endif
