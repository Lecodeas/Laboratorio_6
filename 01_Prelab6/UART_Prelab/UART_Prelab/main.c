/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 PUART_prelab.c
 Autor: Ian Anleu Rivera
 Proyecto: Prelab 6
 Hardware: ATMEGA328P
 Creado: 22/04/2024
 Ultima modificacion: 22/04/2024
    -----------------------------------------------
*/

//HEADER FILES
#define F_CPU 16000000 //Frecuencia es 16Mhz
#include <avr/io.h>//IO registros
#include <util/delay.h> // Lib para delays
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones

//VARIABLES GLOBALES
//Otras variables
volatile uint8_t received_rx = 0;

//PROTOTIPOS DE FUNCIÓN
void setup(void);
void setupUART(void);
void enviarchar(char Caracter);

//INTERRUPCIONES

ISR(USART_RX_vect){
	received_rx = UDR0; //Recibo el dato en una variable que despliego en la salida apropiada
	PORTB = (received_rx); //Output en B
}

//PRINCIPAL
int main(void)
{
	cli();
	setup();
	setupUART();
	sei();

	while (1)
	{
		enviarchar('H');
		_delay_ms(1000);
	}// Fin Main Loop
}

// OTRAS FUNCIONES
void setup(void){
	//ENTRADAS Y SALIDAS -----------------------
	//Entradas
	//No habrá digitales y UART tomará control sobre el PORTD regular
	
	//Salidas
	//PD0 salida por TX
	DDRD = 0;
	DDRD |= (1<<DDD0);
	//Todo PORTB son salidas
	DDRB = 0;
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3)|(1<<DDB4)|(1<<DDB5);
}

void setupUART(void){
	//RX y TX ya están configurados
	UCSR0A = 0; //No modificar, no necesita nada de este registro activado
	UCSR0B = 0;
	UCSR0C = 0;
	
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //Rx int. | Rxen | Txen
	
	UCSR0C |= (1>>UCSZ01)|(1>>UCSZ00); //Async. | No parity | 1 stop | 8 data 
	
	UBRR0 = 103; //Baud 9600
}

void enviarchar(char Caracter){
	while (!(UCSR0A & (1<<UDRE0))); //Revisar si UDR está vacío
	UDR0 = Caracter; //Al estar vacío enviar el caracter en UDR
}