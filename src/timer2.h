/*********************************************************************************************
* Fichero:		timer2.h
* Autores: 		Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:		funciones de control del timer2 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER2_H_
#define _TIMER2_H_

#include "stdint.h"

/*--- declaración de funciones visibles del módulo timer2.c/timer2.h ---*/
void timer2_inicializar();
void timer2_empezar();
uint32_t timer2_parar(); // devuelve el tiempo en microsegundos
uint32_t timer2_leer(); // devuelve el tiempo en microsegundos

#endif /* _TIMER2_H_ */
