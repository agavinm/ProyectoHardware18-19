/*********************************************************************************************
* Fichero:		timer0.h
* Autores: 		Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER0_H_
#define _TIMER0_H_

#include "stdint.h"

/*--- declaración de funciones visibles del módulo timer0.c/timer0.h ---*/
void timer0_inicializar();
void timer0_empezar();
uint32_t timer0_parar(); // devuelve el tiempo en milisegundos
uint32_t timer0_leer(); // devuelve el tiempo en milisegundos
uint32_t interr();

#endif /* _TIMER0_H_ */
