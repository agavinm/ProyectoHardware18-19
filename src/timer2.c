/*********************************************************************************************
* Fichero:		timer2.c
* Autores: 		Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:		funciones de control del timer2 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44b.h"
#include "44blib.h"

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer2_ISR(void) __attribute__((interrupt("IRQ")));

#define preescalado 0xFFFF00FF // bits 15-8 (=0) preescalado de timer2 (cuanto menor, mayor frecuencia)
#define entrada_mux 0xFFFFF0FF // bits 11-8 (=0) entrada_mux de timer2 (la 00 es el divisor 2)
#define max_int 65535 // máximo valor para el contador
#define comp_int 0 // valor de comparación
#define bit_timer2_intmod_irq 0xFFFFF7FF //bit 11 (=0) perteneciente a timer2 en rINTMOD que corresponde a IRQ
#define bit_timer2_autoreload 0x8000 // auto-reload on
#define bit_timer2_manual 0x2000 // manual update on
#define bit_timer2_start 0x1000 // start
#define bit_timer2_clean 0xFFFF0FFF // bits 15-12 (=0) pertenecientes a timer2 en rTCON

static volatile int timer2_num_int = 0;

/*--- codigo de las funciones ---*/
void timer2_ISR(void)
{
	timer2_num_int = timer2_num_int+1;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer2_inicializar()
{
	/* Configuración controlador de interrupciones aplicando máscaras */
	//rINTMOD = 0x0; // Configura las líneas como de tipo IRQ
	rINTMOD &= bit_timer2_intmod_irq; // Configura timer2 como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la línea IRQ (FIQ no)
	rINTMSK &= ~(BIT_TIMER2); // habilitamos en vector de máscaras de interrupción el Timer2 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER2 están definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER2 = (unsigned) timer2_ISR;

	/* Configura el Timer2 aplicando máscaras */
	rTCFG0 &= preescalado; // ajusta el preescalado, cuanto más pequeño más rápido
	rTCFG1 &= entrada_mux; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	// No hacen falta máscaras, exclusivos del timer2
	rTCNTB2 = max_int;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB2 = comp_int;// valor de comparación
}


void timer2_empezar()
{
	rTCON &= bit_timer2_clean; // Se reestablece el control de timer2, por si se llama a esta función con el bit start a on del timer2

	rTCON |= bit_timer2_autoreload; // autoreload on
	rTCON |= bit_timer2_manual; // manual update on
	timer2_num_int = 0;
	rTCON ^= bit_timer2_manual; // manual update off
	rTCON |= bit_timer2_start; // start
}

// devuelve el tiempo en microsegundos
uint32_t timer2_parar()
{
	rTCON &= bit_timer2_clean; // bits timer2 a 0
	return timer2_leer();
}

// devuelve el tiempo en microsegundos
uint32_t timer2_leer()
{
	uint32_t ticks1 = (max_int * timer2_num_int) + (max_int - rTCNTO2);
	uint32_t ticks2 = (max_int * timer2_num_int) + (max_int - rTCNTO2);

	// Al consultar primero la variable timer2_num_int y después el contador, puede haber una medición que sea inferior a la real (subestimación), pero nunca superior (sobreestimación)
	// La frecuencia real es 32, ya que al usar el divisor 2 (entrada_mux) se divide entre 2 la frecuencia
	// Se utiliza el operador de desplazamiento en lugar del operador de división para dividir entre 32 porque es más eficiente en la placa
	if (ticks2 - ticks1 >= max_int) return ticks2 >> 5;
	else return ticks1 >> 5;
}
