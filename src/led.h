/*********************************************************************************************
* Fichero:		led.c
* Autor:		Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:		funciones de control de los LED de la placa
* Version:
*********************************************************************************************/

#ifndef _LED_H_
#define _LED_H_

/*--- declaracion de funciones visibles del módulo led.c/led.h ---*/
/* (*) la función se invoca en el código suministrado */
void leds_on();							// todos los leds encendidos
void leds_off();						// todos los leds apagados (*)
void led1_on();							// led 1 encendido (*)
void led1_off();						// led 1 apagado
void led2_on();							// led 2 encendido
void led2_off();						// led 2 apagado
void leds_switch();						// invierte el valor de los leds (*)
void Led_Display(int LedStatus);		// control directo del LED

#endif /* _LED_H_ */
