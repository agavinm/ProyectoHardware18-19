/*********************************************************************************************
* Fichero: excepcion.c
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: tratamiento de excepciones DA, U y SWI
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44b.h"
#include "excepcion.h"
#include "8led.h"
#include "timer0.h"
#include "pila.h"
#include "stdint.h"

/*--- variables globales ---*/
#define cod_DA 23		// código modo de ejecución Data Abort
#define cod_U 27		// código modo de ejecución Undefined Instruction
#define cod_SWI 19		// código modo de ejecución Software Interrupt
#define led_DA 13		// Encender 8led por Data Abort
#define led_U 14		// Encender 8led por Undefined Instruction
#define led_SWI 10		// Encender 8led por Software Interrupt

static uint32_t dir_inst = 0;	// Instruction que ha causado la interrupci�n

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void excepcion_handler(void) __attribute__((interrupt("DABORT")));
void excepcion_handler(void) __attribute__((interrupt("UNDEF")));
void excepcion_handler(void) __attribute__((interrupt("SWI")));


/* Devuelve el valor de los últimos 5 bits del registro CPSR */
unsigned int CPSR_leer(void)
{
	uint32_t retVal;
	asm("MRS %[res], cpsr" : [res] "=r" (retVal));
	asm("AND %[res], %[input], #0x1F" : [res] "=r" (retVal) : [input] "r" (retVal));
	return retVal;
}

/*--- codigo de las funciones ---*/
void excepcion_handler(void)
{
	asm("MOV %[res], lr" : [res] "=r" (dir_inst));

	long instr = dir_inst;

	volatile uint32_t cod_excp = CPSR_leer();
	volatile uint8_t id_pila = p_desconocida;
	
	if (cod_excp == cod_DA) {
		dir_inst -= 2;
		id_pila = p_ex_dabort;
		D8Led_symbol(led_DA);
	}
	else if (cod_excp == cod_U) {
		id_pila = p_ex_undef;
		D8Led_symbol(led_U);
	}
	else if (cod_excp == cod_SWI) {
		dir_inst--;
		id_pila = p_ex_swi;
		D8Led_symbol(led_SWI);
	}

	push_debug(id_pila, dir_inst);

	while(1) {}
}

void init_excepcion(void)
{
	pISR_DABORT = (unsigned) excepcion_handler;
	pISR_UNDEF = (unsigned) excepcion_handler;
	pISR_SWI = (unsigned) excepcion_handler;
}
