/*********************************************************************************************
* Fichero: pila.h
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: pila de depuración
* Version:
*********************************************************************************************/

#ifndef _PILA_H_
#define _PILA_H_

#include "stdint.h"


// ID casos pila:
enum {
	p_desconocida = 0x5,
	p_boton_none  = 0x10,
	p_boton_izq   = 0x11,
    p_boton_der   = 0x12,
    p_ex_dabort   = 0x23,
    p_ex_undef    = 0x24,
    p_ex_swi      = 0x25
};


void pila_init();

// ID_evento = bits 0-7
// auxData = bits 0-23
void push_debug(uint8_t ID_evento, uint32_t auxData);

#endif
