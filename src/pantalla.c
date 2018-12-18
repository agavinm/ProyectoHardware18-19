/*********************************************************************************************
* Fichero: pantalla.c
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: modulo pantalla
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "pantalla.h"

#include "simular.h"
#ifndef simulacion

#include "lcd.h"
#include "tp.h"

#endif

#define minX_tablero 19
#define maxX_tablero 179
#define minY_tablero 39
#define maxY_tablero 199
#define xPan_inicial 41 // Primera fila tablero
#define yPan_inicial 25 // Primera columna tablero
#define pan_interval 20 // Intervalo entre cada fila/columna
#define retardo 200 // milisegundos
#define ESCRIBIR 1

// Estados de las casillas del tablero
enum {
CASILLA_VACIA = 0,
FICHA_BLANCA = 1,
FICHA_NEGRA = 2
};

typedef enum estado_automata {pantalla_esperar, pantalla_pulsar} estado_automata;

/*--- variables globales ---*/
static uint32_t siguiente; // milisegundos
static uint32_t tiempo_patron_antes = 0, tiempo_partida_antes = 0; // segundos
static volatile estado_automata estado_auto;
static pantalla estado_pant;
static int pulsacionAntes, pulsacionAhora;

/*--- codigo de las funciones ---*/
void pantalla_init() {
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	pulsacionAntes = TS_puls();
	pulsacionAhora = pulsacionAntes;
	estado_auto = pantalla_esperar;
	estado_pant = p_no_pulsada;

	Lcd_Dma_Trans(); // dma transport virtual LCD screen to LCD actual screen
}

void pantalla_help() {
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	Lcd_DspAscII8x16(10,10,BLACK,"      Reversi8 -- Instrucciones        ");
	Lcd_DspAscII8x16(10,30,DARKGRAY,"El usuario maneja las fichas negras y  "); // longitud max 39
	Lcd_DspAscII8x16(10,40,DARKGRAY,"empieza colocando.                     ");
	Lcd_DspAscII8x16(10,50,DARKGRAY,"La maquina maneja las fichas blancas.  ");
	Lcd_DspAscII8x16(10,70,DARKGRAY,"El movimiento consiste en colocar una  ");
	Lcd_DspAscII8x16(10,80,DARKGRAY,"ficha de forma que flanquee una o      ");
	Lcd_DspAscII8x16(10,90,DARKGRAY,"varias fichas del color contrario y    ");
	Lcd_DspAscII8x16(10,100,DARKGRAY,"voltear esas fichas para que pasen    ");
	Lcd_DspAscII8x16(10,110,DARKGRAY,"a mostrar el propio color.            ");
	Lcd_DspAscII8x16(10,120,DARKGRAY,"Se voltean todas las fichas que se    ");
	Lcd_DspAscII8x16(10,130,DARKGRAY,"han flanqueado en ese turno al        ");
	Lcd_DspAscII8x16(10,140,DARKGRAY,"colocar la ficha del color contrario, ");
	Lcd_DspAscII8x16(10,150,DARKGRAY,"de tal manera que se forme una linea  ");
	Lcd_DspAscII8x16(10,160,DARKGRAY,"recta de fichas del mismo color entre ");
	Lcd_DspAscII8x16(10,170,DARKGRAY,"dos fichas del color contrario.       ");
	Lcd_DspAscII8x16(10,190,DARKGRAY,"Gana el jugador con mas fichas de su  ");
	Lcd_DspAscII8x16(10,200,DARKGRAY,"color en el tablero.                  ");

	pantalla_pulsar_continuar(ESCRIBIR);
}

/* Transforma x del tablero en x de pantalla */
int pantalla_xTab_xPan(char xTab) {
	return xTab * pan_interval + xPan_inicial;
}

/* Transforma y del tablero en y de pantalla */
int pantalla_yTab_yPan(char yTab) {
	return yTab * pan_interval + yPan_inicial;
}

void pantalla_casilla(char fila, char columna, char casilla, pantalla_color color) {
	int x = pantalla_xTab_xPan(fila);
	int y = pantalla_yTab_yPan(columna);
	if (casilla == FICHA_NEGRA) {
		if (color == p_negro)
			Lcd_DspAscII8x16(y,x,BLACK,"N");
		else
			Lcd_DspAscII8x16(y,x,DARKGRAY,"N");
	}
	else if (casilla == FICHA_BLANCA)
		Lcd_DspAscII8x16(y,x,BLACK,"B");
	else
		Lcd_DspAscII8x16(y,x,WHITE,"N"); // Borrar

	Lcd_Dma_Trans(); // dma transport virtual LCD screen to LCD actual screen
}

/* Devuelve la longitud de valor */
uint32_t longitud(uint32_t valor) {
	uint32_t l = 1;
	while (valor > 9) {
		l++;
		valor /= 10;
	}
	return l;
}

/* Transforma entero a cadena
 * longitud = longitud de entrada
 */
void atoi(uint32_t entrada, uint32_t longitud, char *salida) {
	while (longitud > 0) {
		salida[longitud-1] = (INT8U) (entrada % 10 + '0');
		entrada /= 10;
		longitud--;
	}
}

/* Muestra el tiempo cada segundo */
void pantalla_tiempo(uint32_t tiempo_partida) { // segundos
	// Borrar tiempo antes
	uint32_t lon = longitud(tiempo_partida_antes);
	volatile char t_antes[56] = "";
	atoi(tiempo_partida_antes, lon, &t_antes);

	Lcd_DspAscII8x16(199, 49, WHITE, &t_antes);
	Lcd_Dma_Trans(); // dma transport virtual LCD screen to LCD actual screen //TODO: cuando reinicias el tiempo no se borra

	lon = longitud(tiempo_partida);
	volatile char t_ahora[56] = "";
	atoi(tiempo_partida, lon, &t_ahora);

	// Mostrar tiempo de partida
	tiempo_partida_antes = tiempo_partida;
	Lcd_DspAscII8x16(199, 39, BLACK, "Tiempo total:");
	Lcd_DspAscII8x16(199, 49, BLACK, &t_ahora);
	Lcd_Dma_Trans(); // dma transport virtual LCD screen to LCD actual screen
}

void pantalla_mostrar_partida(uint32_t tiempo_patron, char tablero[][tam_tablero]) {
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	Lcd_DspAscII8x16(9,19,BLACK,"C");
	Lcd_DspAscII8x16(0,29,BLACK,"F");
	
	// Numeración filas
	Lcd_DspAscII8x16(5,41,BLACK,"0");
	Lcd_DspAscII8x16(5,61,BLACK,"1");
	Lcd_DspAscII8x16(5,81,BLACK,"2");
	Lcd_DspAscII8x16(5,101,BLACK,"3");
	Lcd_DspAscII8x16(5,121,BLACK,"4");
	Lcd_DspAscII8x16(5,141,BLACK,"5");
	Lcd_DspAscII8x16(5,161,BLACK,"6");
	Lcd_DspAscII8x16(5,181,BLACK,"7");
	
	// Numeración columnas
	Lcd_DspAscII8x16(25,21,BLACK,"0");
	Lcd_DspAscII8x16(45,21,BLACK,"1");
	Lcd_DspAscII8x16(65,21,BLACK,"2");
	Lcd_DspAscII8x16(85,21,BLACK,"3");
	Lcd_DspAscII8x16(105,21,BLACK,"4");
	Lcd_DspAscII8x16(125,21,BLACK,"5");
	Lcd_DspAscII8x16(145,21,BLACK,"6");
	Lcd_DspAscII8x16(165,21,BLACK,"7");
	
	int x, y;
	for (y = minY_tablero; y < maxY_tablero; y += 20) {
		for (x = minX_tablero; x < maxX_tablero; x += 20) {
			Lcd_Draw_Box(x, y+20, x+20, y, BLACK);
		}
	}

	//Mostrar casillas
	int i, j;
	for (i = 0; i < tam_tablero; i++) {
		for (j = 0; j < tam_tablero; j++) {
			if (tablero[i][j] != CASILLA_VACIA) {
				x = pantalla_xTab_xPan(i);
				y = pantalla_yTab_yPan(j);
				if (tablero[i][j] == FICHA_NEGRA)
					Lcd_DspAscII8x16(y,x,BLACK,"N");
				else
					Lcd_DspAscII8x16(y,x,BLACK,"B");
			}
		}
	}

	// Borrar tiempo patron antes
	uint32_t lon = longitud(tiempo_patron_antes);
	volatile char t_antes[56] = "";
	atoi(tiempo_patron_antes, lon, &t_antes);

	Lcd_DspAscII8x16(199, 79, WHITE, &t_antes);
	Lcd_Dma_Trans(); // dma transport virtual LCD screen to LCD actual screen

	// Mostrar tiempo de patron
	lon = longitud(tiempo_patron);
	volatile char t_ahora[56] = "";
	atoi(tiempo_patron, lon, &t_ahora);
	tiempo_patron_antes = tiempo_patron;

	Lcd_DspAscII8x16(199, 69, BLACK, "Tiempo patron:");
	Lcd_DspAscII8x16(199, 79, BLACK, &t_ahora);

	Lcd_Dma_Trans(); // dma transport virtual LCD screen to LCD actual screen
}

void pantalla_gestionar(uint32_t ahora) { // milisegundos
	switch (estado_auto) {
		case pantalla_esperar:
			pulsacionAhora = TS_puls();
			if (pulsacionAhora != pulsacionAntes) {
				pulsacionAntes = pulsacionAhora;
				siguiente = ahora + retardo;
				estado_auto = pantalla_pulsar;
				estado_pant = p_pulsada;
			}
			break;

		case pantalla_pulsar:
			if (ahora >= siguiente) {
				// Reset pulsaciones
				pulsacionAhora = TS_puls();
				pulsacionAntes = pulsacionAhora;
				estado_auto = pantalla_esperar;
			}
			break;
	}
}

pantalla pantalla_pulsada()
{
	pantalla antes = estado_pant;
	estado_pant = p_no_pulsada;
	return antes;
}

void pantalla_pulsar_continuar(char escribir) {
	if (escribir == ESCRIBIR)
		Lcd_DspAscII8x16(10,220,BLACK,"   Pulse para jugar                    "); // fila max
	else
		Lcd_DspAscII8x16(10,220,WHITE,"   Pulse para jugar                    "); // fila max

	Lcd_Dma_Trans(); // dma transport virtual LCD screen to LCD actual screen
}
void pantalla_pulsar_cancelar(char escribir) {
	if (escribir == ESCRIBIR)
		Lcd_DspAscII8x16(10,220,BLACK,"   Pulse para cancelar                 "); // fila max
	else
		Lcd_DspAscII8x16(10,220,WHITE,"   Pulse para cancelar                 "); // fila max

	Lcd_Dma_Trans(); // dma transport virtual LCD screen to LCD actual screen
}

void pantalla_ganador(char ganador) {
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	Lcd_DspAscII8x16(10,10,BLACK,"      Reversi8 -- Partida terminada    ");
	if (ganador == FICHA_BLANCA)
		Lcd_DspAscII8x16(10,40,BLACK,"Has perdido, lastima...                ");
	else
		Lcd_DspAscII8x16(10,40,BLACK,"Has ganado, enhorabuena                ");
	Lcd_DspAscII8x16(10,50,DARKGRAY,"Se va a reiniciar la partida           ");

	Lcd_Dma_Trans(); // dma transport virtual LCD screen to LCD actual screen
}
