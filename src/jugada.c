/*********************************************************************************************
* Fichero: jugada.c
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: modulo jugada
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "jugada.h"
#include "pantalla.h"
#include "botones_antirebotes.h"

typedef enum estado_automata {iniciar, reiniciar, mover, parpadear, ganar, terminar} estado_automata;

#define max_numero R_DIM-1
#define periodo_parpadeo 250 // milisegundos
#define periodo_estado 2000
#define periodo_ganador 5000
#define FICHA_NEGRA 2
#define CASILLA_VACIA 0
#define ESCRIBIR 1
#define BORRAR 0

/*--- variables globales ---*/
static boton boton_antes;
static char f, c, casilla;
static uint32_t siguiente_parpadeo, siguiente_estado, antes; // milisegundos
static uint32_t tiempo_partida, tiempo_partida_antes; // segundos
static boton boton_ahora;
static estado_automata estado_auto;
static pantalla panta_ahora;
static char ganador;

/*--- codigo de las funciones ---*/
void jugada_init()
{
	pantalla_init();

	boton_antes = b_ninguno;
	estado_auto = iniciar;
	siguiente_parpadeo = 0;
	siguiente_estado = 0;
	antes = 0;
	pantalla_help();
}

void jugada_ganador(char g) {
	estado_auto = ganar;
	ganador = g;
}

/* Devuelve 1 si se ha terminado la jugada, 0 en otro caso. */
char jugada_gestionar(uint32_t ahora, uint32_t tiempo_patron, char *fila, char *columna, char tablero[][R_DIM]) // milisegundos
{
	pantalla_gestionar(ahora);
	botones_antirebotes_gestionar(ahora);

	boton_ahora = botones_antirebotes_pulsado();
	panta_ahora = pantalla_pulsada();

	tiempo_partida += (ahora-antes);
	antes = ahora;
	if (estado_auto != iniciar && estado_auto != ganar && estado_auto != terminar
					&& tiempo_partida >= (tiempo_partida_antes + 1000)) {
		tiempo_partida_antes = tiempo_partida;
		pantalla_tiempo(tiempo_partida / 1000);
	}

	switch (estado_auto) {
	case iniciar:
		if (boton_ahora != boton_antes || panta_ahora == p_pulsada) {
			boton_antes = boton_ahora;
			estado_auto = mover;
			tiempo_partida = 0;
			tiempo_partida_antes = 0;
			antes = ahora;
			f = 0;
			c = 0;
			casilla = tablero[f][c];
			pantalla_mostrar_partida(tiempo_patron, tablero);
		}

		return 0;

	case reiniciar:
		estado_auto = mover;
		f = 0;
		c = 0;
		casilla = tablero[f][c];
		pantalla_mostrar_partida(tiempo_patron, tablero);

		return 0;

	case mover:
		// Introducir fila o columna
		if (boton_ahora != boton_antes) {
			boton_antes = boton_ahora;
			if (boton_ahora == b_izquierdo) {
				pantalla_casilla(f, c, CASILLA_VACIA, p_negro);
				pantalla_casilla(f, c, casilla, p_negro);
				if (c == max_numero) c = 0;
				else c++;
				casilla = tablero[f][c];
			}
			else if (boton_ahora == b_derecho) {
				pantalla_casilla(f, c, CASILLA_VACIA, p_negro);
				pantalla_casilla(f, c, casilla, p_negro);
				if (f == max_numero) f = 0;
				else f++;
				casilla = tablero[f][c];
			}
		}
		pantalla_casilla(f, c, FICHA_NEGRA, p_gris);
		pantalla_pulsar_continuar(ESCRIBIR);

		if (panta_ahora == p_pulsada) {
			estado_auto = parpadear;
			pantalla_pulsar_continuar(BORRAR);
			pantalla_pulsar_cancelar(ESCRIBIR);
			siguiente_parpadeo = ahora;
			siguiente_estado = ahora + periodo_estado;
		}
		return 0;

	case parpadear:
		// Parpadea ficha seleccionada
		if (ahora >= siguiente_parpadeo) {
			siguiente_parpadeo += periodo_parpadeo;
			if (casilla == CASILLA_VACIA) {
				casilla = FICHA_NEGRA;
			}
			else {
				casilla = CASILLA_VACIA;
			}
			pantalla_casilla(f, c, casilla, p_gris);
		}

		if (boton_ahora != boton_antes || panta_ahora == p_pulsada) {
			boton_antes = boton_ahora;
			estado_auto = mover;
			pantalla_pulsar_cancelar(BORRAR);
			casilla = tablero[f][c];
			pantalla_casilla(f, c, CASILLA_VACIA, p_negro);
			pantalla_casilla(f, c, casilla, p_negro); // Restablecer la ficha que habia
			return 0;
		}
		else if (ahora >= siguiente_estado) {
			*fila = f;
			*columna = c;
			estado_auto = reiniciar;
			return 1;
		}
		else
			return 0;

	case ganar:
		pantalla_ganador(ganador);
		estado_auto = terminar;
		siguiente_estado = ahora + periodo_ganador;
		return 0;

	case terminar:
		if (ahora >= siguiente_estado)
			jugada_init(); // reinicio
		return 0;
	}
}
