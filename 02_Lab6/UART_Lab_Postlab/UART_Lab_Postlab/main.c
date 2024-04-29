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
#include "ADC_lib/ADC.h" //Mis configs de ADC

//CONSTANTES
#define MenuMode 0x01
#define PotMode 0x02
#define AsciiMode 0x04
#define ErrorMode 0x08

//VARIABLES GLOBALES
volatile uint8_t rx_leido;
uint8_t mis_flags = MenuMode; //|R|R|R|R|Err. Mode|Ascii Mode|Pot Mode|Menu Mode|
uint8_t adchleido = 0;

//PROTOTIPOS DE FUNCIÓN
void setup(void);
void setupUART(void);
void enviarChar(char Caracter);
void enviarString(char* Texto);

//PRINCIPAL
int main(void)
{
	cli();
	setup();
	setupADC();
    setupUART();
	sei();
	
	
    while (1) 
    {
		enviarString("1. Leer Potenciometro\n2. Enviar ascii\n");	//Desplegar el menú
		//Espero la respuesta de la terminal
		while (mis_flags == MenuMode); //Hasta que salga de Menu mode prosigo
		
		//Puede estar en PotMode o AsciiMode
		
		//Si es PotMode
		if (mis_flags == PotMode){
			adchleido = fetchADCH();
			enviarString("El valor es: ");
			//Envío de caracter por caracter a UART
			uint8_t c = 0; //Centenas
			uint8_t d = 0; //Decenas
			uint8_t u = 0; //Unidades
			while(adchleido > 99){
				c++;
				adchleido -= 100;
			}
			enviarChar((char) c+48);
			while(adchleido > 9){
				d++;
				adchleido -= 10;
			}
			enviarChar((char) d+48);
			while(adchleido != 0){
				u++;
				adchleido -= 1;
			}
			enviarChar((char) u+48);
			enviarChar('\n');
			//Volver a modo menú
			mis_flags = MenuMode;
		} // FIN POT MODE
		else{
			if (mis_flags == AsciiMode){
				//Si es AsciiMode Ya lo trabajo dentro de su interrupción, solo debo a esperar el cambio de modo
				enviarString("Introduzca el caracter: ");
				while (mis_flags == AsciiMode);
				enviarChar((char) rx_leido);
				enviarChar('\n');
			} // FIN ASCII MODE
			else{
				//Es Error mode entonces despliego un mensaje de error
				enviarString("Elija un modo valido\n");
			}
		}		
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
	while (!(UCSR0A & (1<<UDRE0))); //Hasta que esté vacío puedo enviar el dato
	UDR0 = Caracter;
}

void enviarString(char* Texto){
	uint8_t i; //Contador para array de texto
	for(i = 0; Texto[i] != '\0'; i++){
		//Mientras el caracter no sea nulo, se envía
		enviarChar(Texto[i]);
	}
}

//INTERRUPCIONES

ISR(USART_RX_vect){
	rx_leido = UDR0; //Temporal para el leído
	if (mis_flags == MenuMode){
		//Es la opción de menú entonces determino si es 1, 2 u otra cosa.
		if (rx_leido == '1'){ //Si es la opción 1 es modo Potenciómetro
			mis_flags = PotMode; //Cambio a modo Potenciómetro
		}
		else{
			if (rx_leido == '2'){ //Si es la opción 2 es modo Ascii
				mis_flags = AsciiMode; //Cambio a modo Ascii
			}
			else
			{
				mis_flags = ErrorMode; //Si no es una de estas entra a modo error
			}
		}
	}
	else{ // Si NO es Menu Mode, quiere decir que recibí una transmisión de modo Ascii
		PORTB = rx_leido; //Primeros 6 bits
		PORTC = (rx_leido>>6); //Shifteado para los últimos 2 bits	
		mis_flags = MenuMode; //Vuelvo a modo Menu
	}
}

