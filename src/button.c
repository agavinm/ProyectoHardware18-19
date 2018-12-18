/*********************************************************************************************
* Fichero:	button.c
* Autor:	Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "timer0.h"
#include "pila.h"
#include <stdio.h>
#include "stdint.h"

// bit 7 es EINT7 (botón derecho) y bit 6 es EINT6 (botón izquierdo)
#define bit_botonD 7
#define bit_botonI 6
#define boton_pulsado 0

/*--- variables globales del módulo ---*/
static estado_button estado = button_none; // estado del botón
static function_ptr *cb_ptr = NULL; // puntero a función de callback

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));


/*--- codigo de funciones ---*/
// Devuelve el bit posición del número n.
int devuelve_bit(int n, int posicion) {
	return (n >> posicion) & 0x1;
}

void Eint4567_ISR(void)
{
	// Comentar si se van a medir tiempos:
	rINTMSK |= BIT_EINT4567; // deshabilitamos interrupcion linea eint4567 en vector de mascaras

	int botones = rPDATG;

	if (devuelve_bit(botones, bit_botonD) == boton_pulsado) {
		estado = button_dr;
		push_debug(p_boton_der, timer0_leer());
	}
	else if (devuelve_bit(botones, bit_botonI) == boton_pulsado) {
		estado = button_iz;
		push_debug(p_boton_izq, timer0_leer());
	}
	else {
		estado = button_none;
		push_debug(p_boton_none, timer0_leer());
	}

	/* Finalizar ISR */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	cb_ptr(estado); // ejecuta la función de callback con el estado del botón actual
}

void button_iniciar(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
	rI_ISPC    |= 0x200000;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    |= 0x0;		// Configura las linas como de tipo IRQ //TODO: Mascaras bien, es una IRQ
	rINTCON    |= 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
	rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rI_ISPC    |= (BIT_EINT4567);
	rEXTINTPND = 0xf;
}

void button_empezar(function_ptr *callback)
{
	cb_ptr = callback;
	rINTMSK &= ~(BIT_EINT4567); // habilitamos interrupcion linea eint4567 en vector de mascaras
}

void button_habilitar_interr() {
	/* Finalizar ISR */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	rINTMSK &= ~(BIT_EINT4567); // habilitamos interrupcion linea eint4567 en vector de mascaras
}

estado_button button_estado()
{
	int botones = rPDATG;
	if (devuelve_bit(botones, bit_botonD) == boton_pulsado) estado = button_dr;
	else if (devuelve_bit(botones, bit_botonI) == boton_pulsado) estado = button_iz;
	else estado = button_none;

	return estado;
}
