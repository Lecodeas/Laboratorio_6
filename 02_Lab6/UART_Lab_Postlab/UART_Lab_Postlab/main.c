/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 UART_Lab_Postlab.c
 Autor: Ian Anleu Rivera
 Proyecto: Lab 6
 Hardware: ATMEGA328P
 Creado: 22/04/2024
 Ultima modificacion: 24/04/2024
    -----------------------------------------------
*/

//HEADER FILES
#define F_CPU 16000000 //Frecuencia es 16Mhz
#include <avr/io.h>//IO registros
#include <util/delay.h> // Lib para delays
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones

//VARIABLES GLOBALES
volatile uint8_t rx_leido;

//PROTOTIPOS DE FUNCIÓN
void setup(void);
void setupUART(void);
void enviarChar(char Caracter);

//PRINCIPAL
int main(void)
{
	cli();
	setup();
    setupUART();
	sei();
	
	enviarChar('H');
	enviarChar('o');
	enviarChar('l');
	enviarChar('a');
	enviarChar(' ');
	enviarChar('m');
	enviarChar('u');
	enviarChar('n');
	enviarChar('d');
	enviarChar('o');
	enviarChar('\n');
    while (1) 
    {
		
    }
}

// OTRAS FUNCIONES
void setup(void){
	//Entradas y Salidas
	//Rx como entrada
	DDRD = 0;
	DDRD &= ~(1<<DDD0);
	
	//PORTB Como Salida
	DDRB = 0xFF;
	//Tx salida
	DDRD |= (1<<DDD1);
	//PC0 Y 1 como Salida
	DDRC = 0;
	DDRC |= (1<<DDC0)|(1<<DDC1);
}

void setupUART(void){		
	//No necesito nada de este registro
	UCSR0A = 0;
	
	//Rx int. | Tx En. | Rx en.  
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	//Async | No Pariety | 1 stop | 8 data
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	//UDR0 9600
	UBRR0 = 103;
	
}

void enviarChar(char Caracter){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
}


//INTERRUPCIONES

ISR(USART_RX_vect){
	rx_leido = UDR0;
	PORTB = rx_leido;
	PORTC = (rx_leido>>6); //Shifteado para los primeros 2 bits
}

