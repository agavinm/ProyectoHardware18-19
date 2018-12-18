/*********************************************************************************************
* Fichero: pantalla.h
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: modulo pantalla
* Version:
*********************************************************************************************/

#ifndef _PANTALLA_H_
#define _PANTALLA_H_

#include "stdint.h"

#define tam_tablero 8
typedef enum pantalla {p_no_pulsada, p_pulsada} pantalla;
typedef enum pantalla_color {p_gris, p_negro} pantalla_color;

void pantalla_init();
void pantalla_help();
void pantalla_casilla(char fila, char columna, char casilla, pantalla_color color);
void pantalla_tiempo(uint32_t tiempo_partida);
void pantalla_mostrar_partida(uint32_t tiempo_patron, char tablero[][tam_tablero]);
void pantalla_gestionar(uint32_t ahora);
pantalla pantalla_pulsada();
void pantalla_pulsar_continuar(char escribir);
void pantalla_pulsar_cancelar(char escribir);
void pantalla_ganador(char ganador);

#endif
