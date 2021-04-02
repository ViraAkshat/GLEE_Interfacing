/*
 * I2C_MT.c
 *
 * Created: 13-03-2021 16:02:51
 * Author : acer
 */ 

#include <avr/io.h>
#define F_CPU 8000000
//#include <util/delay.h>
#define BAUD 9600
#define MYUBRR 51

//__________________USART_______________
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

//______________________I2C_________________
void TWI_init_master(void)
{
	//XTAL=8MHz, SCL freq = 50kHz
	TWBR = 0x47; //Bit rate
	TWSR = (0<<TWPS1)|(0<<TWPS0); // Prescalar
	TWCR = (1<<TWEN);
}

void TWI_start(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //Set TWCR for START
	while ((TWCR & (1<<TWINT)) == 0); //Wait for Start condition to transmit
	//while ((TWSR & 0xf8) != 0x08); //Check for ack
}

void TWI_write_sla(unsigned char sla_w)
{
	TWDR = sla_w; //send SLA_W 
    TWCR = (1<<TWINT)|(1<<TWEN); //Clear TWINT and set TWEN
	
	while ((TWCR & (1<<TWINT)) == 0); //Wait for TWINT
	//while ((TWSR & 0xf8) != 0x18); //Check ack	
}

void TWI_write_data(unsigned char data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	while((TWCR & (1<<TWINT)) == 0);
	//while ((TWSR & 0xf8) != 0x28);
}

void TWI_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	//while (!(TWCR & (1<<TWSTO)));
}

int main(void)
{
    USART_Init(MYUBRR);
	unsigned char data = USART_Receive();
	
//	_delay_ms(200);
	TWI_init_master();
	
	TWI_start();
	TWI_write_sla(0b00000010);
	//TWI_write_data(0b11110000);
	TWI_write_data(data);
	
	TWI_stop();
	
	while(1);
	
	return 0;
}

