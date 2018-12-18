/*********************************************************************************************
* Fichero:		simular.c
* Autores: 		Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:		funciones de simulación de los periféricos
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "simular.h"
#ifdef simulacion

#include <stdio.h>

// common
void sys_init() {}

// 8led
void D8Led_init() { }
void D8Led_symbol(int value) { }

// button
static estado_button estado = button_none; // estado del boton
static function_ptr *cb_ptr = NULL; // puntero a funcion de callback

void button_iniciar(void) { }

estado_button button_estado() {
	if (estado == button_none) {
		estado = button_iz;
		return button_none;
	}
	else if (estado == button_iz) {
		estado = button_dr;
		return button_iz;
	}
	else {
		estado = button_none;
		return button_dr;
	}
}

void button_empezar(function_ptr *callback) {
	cb_ptr = callback;
	cb_ptr(estado);
}

void button_habilitar_interr() { }

// excepcion
void init_excepcion() { }

// led
static volatile char led1, led2; // 0 apagado, 1 encendido

void leds_off() {
	led1 = 0;
	led2 = 0;
}

void led1_on() {
	led1 = 1;
}

void led1_off() {
	led1 = 1;
}

// lcd
void Lcd_Init(void) { }

void Lcd_Active_Clr(void) { }

void Lcd_Clr(void) { }

void Lcd_DspAscII8x16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U *s) { }

void Lcd_Draw_Box(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom,
                  INT8U ucColor) { }

void Lcd_Dma_Trans(void) { }

// TP
void TS_init(void) { }
int TS_puls() { return 0; }

// timer0
static volatile uint32_t time0; // en milisegundos

void timer0_inicializar() {}

void timer0_empezar() {
	time0 = 0;
}

uint32_t timer0_parar() {
	return timer0_leer();
}

uint32_t timer0_leer() {
	time0 += 250;
	return time0;
}

// timer2
static volatile uint32_t time2; // en microsegundos

void timer2_inicializar() {}

void timer2_empezar() {
	time2 = 0;
}

uint32_t timer2_parar() {
	return timer2_leer();
}

uint32_t timer2_leer() {
	time2 += 250000;
	return time2;
}

#endif
