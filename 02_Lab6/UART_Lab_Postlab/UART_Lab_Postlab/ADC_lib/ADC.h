/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 UART_Lab_Postlab.c
 Autor: Ian Anleu Rivera
 Proyecto: Lab 6
 Hardware: ATMEGA328P
 Creado: 24/04/2024
 Ultima modificacion: 24/04/2024
    -----------------------------------------------
*/	


#ifndef ADC_H_
#define ADC_H_

#define F_CPU 16000000 //Frecuencia es 16Mhz
#include <avr/io.h>//IO registros
#include <util/delay.h> // Lib para delays
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones

void setupADC(void);
uint8_t fetchADCH(void);

#endif /* ADC_H_ */