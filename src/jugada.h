/*********************************************************************************************
* Fichero: jugada.h
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: modulo jugada
* Version:
*********************************************************************************************/

#ifndef _JUGADA_H_
#define _JUGADA_H_

#include "stdint.h"

#define R_DIM 8

void jugada_init();
void jugada_ganador(char g);
char jugada_gestionar(uint32_t ahora, uint32_t tiempo_patron, char *fila, char *columna, char tablero[][R_DIM]);

#endif
