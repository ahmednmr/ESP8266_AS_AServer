/*
 * main.c
 *
 *  Created on: 5 Oct 2015
 *      Author: EmbeddedFab
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "MCAL/UART.h"
#include <avr/interrupt.h>
#include "uart.h"

#define TRUE 1
#define FALSE 0
#define DEFAULT_BUFFER_SIZE 1000
char Rec_Data[DEFAULT_BUFFER_SIZE];
char parsing_Data[100];
int Counter=0;
int REC_Length=0;




char Check_Respond(char * Expected_Respond);
char Check_Rec_data(void);
void Clear_REC_Buffer(void);

ISR (USART_RXC_vect)
{
	uint8_t oldsrg = SREG;
	cli();
	Rec_Data[Counter] = UDR;
	Counter++;
	if(Counter == DEFAULT_BUFFER_SIZE){
		Counter = 0; //pointer = 0;
	}
	SREG = oldsrg;
}


int main()
{
	char i=0,x=128,y=0,z=0,a=0,b=0,c=0,d=0;

	DDRD=(1<<PD4);
	PORTD |=(1<<PD4);
	init_UART();
	sei();


	Clear_REC_Buffer();
	UART_SEND_string("ATE0\r\n");
	while(!((Check_Respond("\r\nOK\r\n"))||(Check_Respond("ATE0\r\n\r\nOK\r\n"))))
	{
		_delay_ms(10);
	}

	UART_SEND_string("AT+CWMODE=3\r\n");
	while(!Check_Respond("\r\nOK\r\n"))
	{
		_delay_ms(10);
	}


	UART_SEND_string("AT+CIPMUX=1\r\n");
	while(!Check_Respond("\r\nOK\r\n"))
	{
		_delay_ms(10);
	}

	UART_SEND_string("AT+CIPSERVER=1,80\r\n");
	while(!((Check_Respond("\r\nOK\r\n"))||(Check_Respond("no change\r\n\r\nOK\r\n"))))
	{
		_delay_ms(10);
	}


	UART_SEND_string("AT+CIPSTO=4000\r\n");
	while(!Check_Respond("\r\nOK\r\n"))
	{
		_delay_ms(10);
	}


	while(1)
	{

		Check_Rec_data();

	}


	return 0;
}




char Check_Respond(char * Expected_Respond)
{
	char Respond_Length=0;

	Respond_Length=strlen(Expected_Respond);

	if(strncmp(Rec_Data,Expected_Respond, Respond_Length)==0)
	{
		Clear_REC_Buffer();
		return TRUE;
	}
	return FALSE;


}



char Check_Rec_data(void)
{

	REC_Length= strlen(Rec_Data);



	if(REC_Length>90)
	{
		memcpy (parsing_Data, Rec_Data, 90*sizeof(char));


		if(strstr(Rec_Data,"turnon") != 0)
		{
			PORTD &=~(1<<PD4);
		}

		else if(strstr(Rec_Data,"turnoff") != 0)
		{
			PORTD |=(1<<PD4);
		}

		memset(Rec_Data,0,REC_Length);
		memset(parsing_Data,0,100);
		Counter=0;
		REC_Length=0;

		return TRUE;
	}
	return FALSE;
}


void Clear_REC_Buffer(void)
{
	Counter=0;
	memset(Rec_Data,0,DEFAULT_BUFFER_SIZE);
}



