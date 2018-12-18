/*********************************************************************************************
* Fichero:		timer0.c
* Autores: 		Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer0.h"
#include "44b.h"
#include "44blib.h"

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer0_ISR(void) __attribute__((interrupt("IRQ")));

#define preescalado_clear 0xFFFFFF00
#define preescalado 0x00000009 // bits 7-0 (=0) preescalado de timer0 (cuanto menor, mayor frecuencia)
#define entrada_mux_clear 0xFFFFFFF0
#define entrada_mux 0x00000000 // bits 3-0 (=0) entrada_mux de timer0 (la 00 es el divisor 2)
#define max_int 64000 // máximo valor para el contador
#define comp_int 0 // valor de comparación
#define bit_timer0_intmod_irq 0xFFFFDFFF //bit 13 (=0) perteneciente a timer0 en rINTMOD que corresponde a IRQ
#define bit_timer0_autoreload 0x0008 // auto-reload on
#define bit_timer0_manual 0x0002 // manual update on
#define bit_timer0_start 0x0001 // start
#define bit_timer0_clean 0xFFFFFFF0 // bits 3-0 (=0) pertenecientes a timer0 en rTCON

static volatile int timer0_num_int = 0;

/*--- codigo de las funciones ---*/
void timer0_ISR(void)
{
	timer0_num_int = timer0_num_int+1;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER0; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer0_inicializar()
{
	/* Configuración controlador de interrupciones aplicando máscaras */
	//rINTMOD = 0x0; // Configura las líneas como de tipo IRQ
	rINTMOD &= bit_timer0_intmod_irq; // Configura timer2 como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la línea IRQ (FIQ no)
	rINTMSK &= ~(BIT_TIMER0); // habilitamos en vector de máscaras de interrupción el Timer2 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER2 están definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER0 = (unsigned) timer0_ISR;

	/* Configura el Timer2 aplicando máscaras */
	rTCFG0 &= preescalado_clear;
	rTCFG0 |= preescalado; // ajusta el preescalado, cuanto más pequeño más rápido
	rTCFG1 &= entrada_mux_clear;
	rTCFG1 |= entrada_mux; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	// No hacen falta máscaras, exclusivos del timer2
	rTCNTB0 = max_int;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = comp_int;// valor de comparación
}


void timer0_empezar()
{
	rTCON &= bit_timer0_clean; // Se reestablece el control de timer2, por si se llama a esta función con el bit start a on del timer2

	rTCON |= bit_timer0_autoreload; // autoreload on
	rTCON |= bit_timer0_manual; // manual update on
	timer0_num_int = 0;
	rTCON ^= bit_timer0_manual; // manual update off
	rTCON |= bit_timer0_start; // start
}

// devuelve el tiempo en milisegundos
uint32_t timer0_parar()
{
	rTCON &= bit_timer0_clean; // bits timer2 a 0
	return timer0_leer();
}

// devuelve el tiempo en milisegundos
uint32_t timer0_leer()
{
	uint32_t ticks1 = (max_int * timer0_num_int) + (max_int - rTCNTO0);
	uint32_t ticks2 = (max_int * timer0_num_int) + (max_int - rTCNTO0);

	// Al consultar primero la variable timer2_num_int y después el contador, puede haber una medición que sea inferior a la real (subestimación), pero nunca superior (sobreestimación)
	// La frecuencia real es 32, ya que al usar el divisor 2 (entrada_mux) se divide entre 2 la frecuencia
	// Se utiliza el operador de desplazamiento en lugar del operador de división para dividir entre 32 porque es más eficiente en la placa
	if (ticks2 - ticks1 >= max_int) return ticks2 / 3200; // Devuelve en milisegundos (Frecuencia real 3.2MHz)
	else return ticks1 / 3200;
}

uint32_t interr()
{
	return timer0_num_int;
}
