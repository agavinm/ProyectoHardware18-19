/*********************************************************************************************
* Fichero: botones_antirebotes.h
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: modulo botones antirebotes
* Version:
*********************************************************************************************/

#ifndef _BOTONES_ANTIREBOTES_H_
#define _BOTONES_ANTIREBOTES_H_

#include "stdint.h"

typedef enum boton {b_ninguno, b_izquierdo, b_derecho} boton;

void botones_antirebotes_init();
void botones_antirebotes_gestionar(uint32_t ahora);
boton botones_antirebotes_pulsado();

#endif
