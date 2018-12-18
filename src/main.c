/*********************************************************************************************
* Fichero:	main.c
* Autor:	Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:	punto de entrada de C
* Version:
*********************************************************************************************/

// Si se trata de una simulación, definir, en "simular.h" simulacion
// Si se utiliza la placa, comentar ,en "simular.h", "#define simulacion"
#include "simular.h"

/*--- ficheros de cabecera para la placa ---*/
#ifndef simulacion
#include "8led.h"
#include "button.h"
#include "led.h"
#include "timer0.h"
#include "timer2.h"
#include "excepcion.h"
#include "44blib.h"
#include "44b.h"
#include "lcd.h"
#include "tp.h"
#endif

/*--- ficheros de cabecera comunes ---*/
#include "reversi8_2018.h"
#include "pila.h"
#include "latido.h"
#include "botones_antirebotes.h"
#include "jugada.h"
#include "stdlib.h"

extern void ex_undefined();
extern void ex_data_abort();
extern void ex_swi();
extern void cambio_de_modo();

/*--- codigo de funciones ---*/
void Main(void)
{
	/* Inicializa controladores */
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	button_iniciar();	// inicializamos los pulsadores.
	D8Led_init();       // inicializamos el 8led
	timer0_inicializar();
	timer2_inicializar();
	init_excepcion();
	Lcd_Init();
	TS_init();

	/* Inicializa dispositivos */
	leds_off();
	pila_init();
	timer0_empezar();
	timer2_empezar();
	botones_antirebotes_init();
	latido_init();
	jugada_init();

	/* Descomentar para provocar excepciones */
	//ex_undefined();
	//ex_data_abort();
	//ex_swi();

	cambio_de_modo(); // Cambio a modo usuario

	reversi_main();
}
