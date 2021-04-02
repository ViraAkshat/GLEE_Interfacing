/*
 * I2C_SR.c
 *
 * Created: 13-03-2021 16:19:07
 * Author : acer
 */ 

#include <avr/io.h>
#define F_CPU 8000000
#define BAUD 9600
#define MYUBRR 51

//____________USART____________
void USART_Init( unsigned int ubrr)
{
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}


void USART_Transmit(unsigned char data )
{
	while(!(UCSR0A & (1<<UDRE0)));
	
	UDR0 = data;
}

unsigned char USART_Receive(void)
{
	while(!(UCSR0A & (1<<RXC0)));
	
	return UDR0;
}

//_____________I2C__________________

void TWI_init_slave(void)
{
	TWAR = 0b00000010;
	TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWINT);
	TWBR = 0x47;
}

void TWI_listen(void)
{
	//while ((TWSR & 0xf8) != 0x60);
	//TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWINT);
	
	while ((TWCR & (1<<TWINT)) == 0);

}

unsigned char TWI_read_data(unsigned char isLast)
{
	//unsigned char rcvd_data;
	//TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWINT);
	//while (!(TWCR & (1<<TWINT)));
	//rcvd_data = TWDR;
	//while ((TWSR & 0xF8) != 0x80);
	
	//return rcvd_data;
	//code from book

	if (isLast == 0)
	    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	else
	    TWCR = (1<<TWINT)|(1<<TWEN);
	
	while ((TWCR & (1<<TWINT)) == 0);
	
	return TWDR;
}

int main()
{
    USART_Init(MYUBRR);
	unsigned char data;
	//DDRA = 0xff;
	TWI_init_slave();
	TWI_listen();
	
	data = TWI_read_data(1);
	//PORTA = TWI_read_data(1);
	USART_Transmit(data);
	
	while(1);
	
	return 0;
}

