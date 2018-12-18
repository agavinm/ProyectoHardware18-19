/*********************************************************************************************
* Fichero: latido.c
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: modulo latido
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "latido.h"

#include "simular.h"
#ifndef simulacion

#include "button.h"		// Definición de _ISR_STARTADDRESS
#include "led.h"

#endif

#define periodo 250 // milisegundos

/*--- variables globales ---*/
static uint32_t siguiente; // milisegundos
static unsigned char estado_led;

/*--- codigo de las funciones ---*/
void latido_init()
{
	siguiente = 0;
	estado_led = 0;
}

void latido_gestionar(uint32_t ahora) // milisegundos
{
	if (ahora >= siguiente) {
		siguiente += periodo;
		if (estado_led == 0) {
			estado_led = 1;
			led1_on();
		}
		else {
			estado_led = 0;
			led1_off();
		}
	}
}
