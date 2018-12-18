/*********************************************************************************************
* Fichero:	8led.h
* Autor:	Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:	Funciones de control del display 8-segmentos
* Version:
*********************************************************************************************/

#ifndef _8LED_H_
#define _8LED_H_

/*--- definicion de valores ---*/
/* Mapa de bits de cada segmento
  (valor que se debe escribir en el display para que se ilumine el segmento) */
/* NOTA:
 * Los datos definidos en un tipo enumerado son de tipo int (por defecto)
 */
enum {
    cero      = ~0xED,
    uno	      = ~0x60,
    dos       = ~0xCE,
    tres      = ~0xEA,
    cuatro    = ~0x63,
    cinco     = ~0xAB,
    seis      = ~0x2F,
    siete     = ~0xE0,
    ocho      = ~0xEF,
    nueve     = ~0xE3,
    A         = ~0xE7,
    B         = ~0x2F,
    C         = ~0x8D,
    D         = ~0x6E,
    E         = ~0x8F,
    F         = ~0x87,
    blank     = ~0x00,
    size_8led = 17 };


/*--- declaración de funciones visibles del módulo 8led.c/8led.h ---*/
void D8Led_init(void);
void D8Led_symbol(int value);

#endif /* _8LED_H_ */
