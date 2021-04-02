/*
 * USART_RxTx.c
 *
 * Created: 06-01-2021 20:49:40
 * Author : acer
 */ 

#include <avr/io.h>
#define FOSC 8000000
#define BAUD 9600
#define MYUBRR 51

void USART_Init( unsigned int ubrr)
{
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}


void USART_Transmit( uint8_t data )
{
	while(!(UCSR0A & (1<<UDRE0)));
	
	UDR0 = data;
}

uint8_t USART_Receive(void)
{
	while(!(UCSR0A & (1<<RXC0)));
	
	return UDR0;
}

int main(void)
{
	USART_Init(MYUBRR);
	
	while(1) {
		USART_Transmit(USART_Receive());
	}
	
}

