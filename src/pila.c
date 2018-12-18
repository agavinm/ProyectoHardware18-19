/*********************************************************************************************
* Fichero: pila.c
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: pila de depuración
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "pila.h"

#include "simular.h"
#ifndef simulacion

#include "option.h"		// Definición de _ISR_STARTADDRESS 
#include "timer2.h"

#endif


static uint32_t inicio = _ISR_STARTADDRESS-0xf00+256*7;		// Dirección de inicio declarada en 44binit.asm para la pila // 0xc7fee00
static uint32_t fin = _ISR_STARTADDRESS-0xf00+256*5;		// Dirección de fin de la pila, se le resta tres para que esté alineado con la dirección de inicio
static uint32_t mask_24bits = 0x00ffffff;

/*--- variables globales ---*/
static uint32_t* pos;						// Puntero utilizado internamente para guardar datos en la pila

/*--- codigo de las funciones ---*/
void pila_init()
{
	pos = inicio;
}

// ID_evento = bits 0-7
// auxData = bits 0-23
void push_debug(uint8_t ID_evento, uint32_t auxData)
{
	uint32_t tiempo = timer2_leer(); // tiempo en microsegundos cuando ocurre

	if (pos == fin) {
		pos = inicio;
	}

	uint32_t valorA = ID_evento << 24;
	uint32_t valorB = auxData & mask_24bits;
	uint32_t valor = valorA | valorB;

	// Comentar
	//tiempo = auxData;
	//valor = ID_evento;

	*pos = tiempo;
	pos--;			// Aumenta 32bits
	*pos = valor;
	pos--;
}
