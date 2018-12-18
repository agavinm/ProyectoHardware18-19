/*********************************************************************************************
* Fichero:	button.h
* Autor:	Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

typedef enum estado_button {button_none, button_iz, button_dr} estado_button;
typedef void(function_ptr)(estado_button boton);

/*--- declaracion de funciones visibles del módulo button.c/button.h ---*/
void button_iniciar(void);
estado_button button_estado();
void button_empezar(function_ptr *callback);
void button_habilitar_interr();

#endif /* _BUTTON_H_ */
