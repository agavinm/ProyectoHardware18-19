/*********************************************************************************************
* Fichero: botones_antirebotes.c
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: modulo botones antirebotes
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "botones_antirebotes.h"

#include "simular.h"
#ifndef simulacion

#include "button.h"		// DefiniciÃ³n de _ISR_STARTADDRESS

#endif

typedef enum estado_automata {boton_esperar, boton_pulsar, boton_monitorizar, boton_levantar} estado_automata;

#define retardo 20 // milisegundos
#define trp 100 // milisegundos
#define trd 100 // milisegundos

/*--- variables globales ---*/
static uint32_t siguiente; // milisegundos
static volatile estado_automata estado_auto;
static volatile estado_button estado_botones;

void callback(estado_button estado)
{
	// Se ha pulsado un botón
	estado_botones = estado;
	estado_auto = boton_pulsar;
}

/*--- codigo de las funciones ---*/
void botones_antirebotes_init()
{
	siguiente = 0;
	estado_botones = button_none;
	estado_auto = boton_esperar;
	button_empezar(&callback); // se inicializa button y estado_botones
}

void botones_antirebotes_gestionar(uint32_t ahora) // milisegundos
{
	switch (estado_auto) {
	case boton_esperar:
		break;

	case boton_pulsar:
		// Se ha pulsado un botón
		if (estado_botones != button_none) {
			estado_auto = boton_monitorizar;
			siguiente = ahora + trp;
		}
		else {
			estado_botones = button_estado();
		}

		break;

	case boton_monitorizar:
		if (ahora >= siguiente) {
			estado_botones = button_estado();

			if (estado_botones == button_none) {
				estado_auto = boton_levantar;
				siguiente = ahora + trd;
			}
			else {
				siguiente += retardo;
			}
		}

		break;

	case boton_levantar:
		if (ahora >= siguiente) {
			estado_auto = boton_esperar;
			button_habilitar_interr(); // habilitar interrupcion button
		}

		break;
	}
}

boton botones_antirebotes_pulsado()
{
	switch (estado_botones) {
	case button_none:
		return b_ninguno;
	case button_dr:
		return b_derecho;
	case button_iz:
		return b_izquierdo;
	}
}
