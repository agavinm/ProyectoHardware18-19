/*********************************************************************************************
* Fichero: reversi8_2018.c
* Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip: Reversi8
* Version:
*********************************************************************************************/

#include "reversi8_2018.h"
#include "latido.h"
#include "jugada.h"

#include "simular.h"
#ifndef simulacion

#include "timer0.h"
#include "timer2.h"

#endif

// Tamaño del tablero
enum { DIM=8 };

// Valores que puede devolver la función patron_volteo())
enum {
	NO_HAY_PATRON = 0,
  PATRON_ENCONTRADO = 1
};

enum {
	PARTIDA_ACABADA = 0,
	PARTIDA_SIGUE = 1
};

// Estados de las casillas del tablero
enum {
CASILLA_VACIA = 0,
FICHA_BLANCA = 1,
FICHA_NEGRA = 2
};

// candidatas: indica las posiciones a explorar
// Se usa para no explorar todo el tablero innecesariamente
// Sus posibles valores son NO, SI, CASILLA_OCUPADA
static const char  NO              = 0;
static const char  SI              = 1;
static const char  CASILLA_OCUPADA = 2;

static uint32_t tiempo_patron = 0;

/////////////////////////////////////////////////////////////////////////////
// TABLAS AUXILIARES
// declaramos las siguientes tablas como globales para que sean más fáciles visualizarlas en el simulador
// __attribute__ ((aligned (8))): specifies a minimum alignment for the variable or structure field, measured in bytes, in this case 8 bytes

static const char __attribute__ ((aligned (8))) tabla_valor[DIM][DIM] =
{
    {8,2,7,3,3,7,2,8},
    {2,1,4,4,4,4,1,2},
    {7,4,6,5,5,6,4,7},
    {3,4,5,0,0,5,4,3},
    {3,4,5,0,0,5,4,3},
    {7,4,6,5,5,6,4,7},
    {2,1,4,4,4,4,1,2},
    {8,2,7,3,3,7,2,8}
};


// Tabla de direcciones. Contiene los desplazamientos de las 8 direcciones posibles
static const char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
static const char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};

//////////////////////////////////////////////////////////////////////////////////////
// Variables globales que no deberían serlo
// tablero, fila, columna y ready son varibles que se deberían definir como locales dentro de reversi8.
// Sin embargo, las hemos definido como globales para que sea más fácil visualizar el tablero y las variables en la memoria
//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Tablero sin inicializar
////////////////////////////////////////////////////////////////////
static char __attribute__ ((aligned (8))) tablero[DIM][DIM] = {
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
	    };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 indica CASILLA_VACIA, 1 indica FICHA_BLANCA y 2 indica FICHA_NEGRA
// pone el tablero a cero y luego coloca las fichas centrales.
void init_table(char tablero[][DIM], char candidatas[][DIM])
{
    int i, j;

    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
            tablero[i][j] = CASILLA_VACIA;
    }
#if 0
    for (i = 3; i < 5; ++i) {
	for(j = 3; j < 5; ++j) {
	    tablero[i][j] = i == j ? FICHA_BLANCA : FICHA_NEGRA;
	}
    }

    for (i = 2; i < 6; ++i) {
	for (j = 2; j < 6; ++j) {
	    if((i>=3) && (i < 5) && (j>=3) && (j<5)) {
		candidatas[i][j] = CASILLA_OCUPADA;
	    } else {
		candidatas[i][j] = SI; //CASILLA_LIBRE;
	    }
	}
    }
#endif
    // arriba hay versión alternativa
    tablero[3][3] = FICHA_BLANCA;
    tablero[4][4] = FICHA_BLANCA;
    tablero[3][4] = FICHA_NEGRA;
    tablero[4][3] = FICHA_NEGRA;

    candidatas[3][3] = CASILLA_OCUPADA;
    candidatas[4][4] = CASILLA_OCUPADA;
    candidatas[3][4] = CASILLA_OCUPADA;
    candidatas[4][3] = CASILLA_OCUPADA;

    // casillas a explorar:
    candidatas[2][2] = SI;
    candidatas[2][3] = SI;
    candidatas[2][4] = SI;
    candidatas[2][5] = SI;
    candidatas[3][2] = SI;
    candidatas[3][5] = SI;
    candidatas[4][2] = SI;
    candidatas[4][5] = SI;
    candidatas[5][2] = SI;
    candidatas[5][3] = SI;
    candidatas[5][4] = SI;
    candidatas[5][5] = SI;
}

////////////////////////////////////////////////////////////////////////////////
// Espera a que ready valga 1.
// CUIDADO: si el compilador coloca esta variable en un registro, no funcionará.
// Hay que definirla como "volatile" para forzar a que antes de cada uso la cargue de memoria

void esperar_mov(char *ready)
{
    while (*ready == 0) {};  // bucle de espera de respuestas hasta que el se modifique el valor de ready (hay que hacerlo manualmente)

    *ready = 0;  //una vez que pasemos el bucle volvemos a fijar ready a 0;
}

////////////////////////////////////////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// IMPORTANTE: AL SUSTITUIR FICHA_VALIDA() Y PATRON_VOLTEO()
// POR RUTINAS EN ENSAMBLADOR HAY QUE RESPETAR LA MODULARIDAD.
// DEBEN SEGUIR SIENDO LLAMADAS A FUNCIONES Y DEBEN CUMPLIR CON EL ATPCS
// (VER TRANSPARENCIAS Y MATERIAL DE PRACTICAS):
//  - DEBEN PASAR LOS PARAMETROS POR LOS REGISTROS CORRESPONDIENTES
//  - GUARDAR EN PILA SOLO LOS REGISTROS QUE TOCAN
//  - CREAR UN MARCO DE PILA TAL Y COMO MUESTRAN LAS TRANSPARENCIAS
//    DE LA ASIGNATURA (CON EL PC, FP, LR,....)
//  - EN EL CASO DE LAS VARIABLES LOCALES, SOLO HAY QUE APILARLAS
//    SI NO SE PUEDEN COLOCAR EN UN REGISTRO.
//    SI SE COLOCAN EN UN REGISTRO NO HACE FALTA
//    NI GUARDARLAS EN PILA NI RESERVAR UN ESPACIO EN LA PILA PARA ELLAS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posición indicadas por la fila y columna actual.
// Además informa si la posición es válida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es válida o está vacia.
char ficha_valida(char tablero[][DIM], char f, char c, int *posicion_valida)
{
    char ficha;

    // ficha = tablero[f][c];
    // no puede accederse a tablero[f][c]
    // ya que algún índice puede ser negativo

    if ((f < DIM) && (f >= 0) && (c < DIM) && (c >= 0) && (tablero[f][c] != CASILLA_VACIA))
    {
        *posicion_valida = 1;
        ficha = tablero[f][c];
    }
    else
    {
        *posicion_valida = 0;
        ficha = CASILLA_VACIA;
    }
    return ficha;
}

////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo es una función recursiva que busca el patrón de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando. Se usa para saber cuantas fichas habría que voltear
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color)
{
    int posicion_valida; // indica si la posición es valida y contiene una ficha de algún jugador
    int patron; //indica si se ha encontrado un patrón o no
    char casilla;   // casilla es la casilla que se lee del tablero
    FA = FA + SF;
    CA = CA + SC;
    casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
    // mientras la casilla está en el tablero, no está vacía,
    // y es del color rival seguimos buscando el patron de volteo
    if ((posicion_valida == 1) && (casilla != color))
    {
        *longitud = *longitud + 1;
        patron = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
        //printf("longitud: %d \n", *longitud);
        //printf("fila: %d; columna: %d \n", FA, CA);
        return patron;
    }
    // si la ultima posición era válida y la ficha es del jugador actual,
    // entonces hemos encontrado el patrón
    else if ((posicion_valida == 1) && (casilla == color))
    {
        if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
            {
            return PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
            //printf("PATRON_ENCONTRADO \n");
            }
        else {
            return NO_HAY_PATRON; // si no hay que voltear no hay patrón
            //printf("NO_HAY_PATRON \n");
            }
    }
    // en caso contrario es que no hay patrón
    else
    {
        return NO_HAY_PATRON;
        //printf("NO_HAY_PATRON \n");
    }
}

// Versión de patron_volteo en arm con la función ficha_valida incrustada en el codigo (inlining)
//extern int patron_volteo_arm_arm(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color);

// Versión de patron_volteo en arm con una llamada a ficha_valida en c
//extern int patron_volteo_arm_c(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color);

// Comprueba las distintas versiones de patron_volteo, si el resultado de las versiones es el mismo, actúa como patron_volteo, en caso contrario no termina
int patron_volteo_test(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color)
{
	// En 03 es mejor el compilador
	tiempo_patron = timer2_leer();
	int result = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
	tiempo_patron = timer2_leer() - tiempo_patron;
	return result;
}

////////////////////////////////////////////////////////////////////////////////
// voltea n fichas en la dirección que toque
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// FA y CA son la fila y columna a analizar
void voltear(char tablero[][DIM], char FA, char CA, char SF, char SC, int n, char color)
{
    int i;

    for (i = 0; i < n; i++)
    {
        FA = FA + SF;
        CA = CA + SC;
        tablero[FA][CA] = color;
    }
}
////////////////////////////////////////////////////////////////////////////////
// comprueba si hay que actualizar alguna ficha
// no comprueba que el movimiento realizado sea válido
// f y c son la fila y columna a analizar
// char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
// char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
char actualizar_tablero(char tablero[][DIM], char f, char c, char color)
{
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque
    int i, flip, patron;
    char algun_patron = NO_HAY_PATRON;

    for (i = 0; i < DIM; i++) // 0 es Norte, 1 NE, 2 E ...
    {
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        patron = patron_volteo_test(tablero, &flip, f, c, SF, SC, color);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO )
        {
            voltear(tablero, f, c, SF, SC, flip, color);
            algun_patron = PATRON_ENCONTRADO;
        }
    }
    return algun_patron;
}

/////////////////////////////////////////////////////////////////////////////////
// Recorre todo el tablero comprobando en cada posición si se puede mover
// En caso afirmativo, consulta la puntuación de la posición y si es la mejor
// que se ha encontrado la guarda
// Al acabar escribe el movimiento seleccionado en f y c

// Candidatas
// NO    0
// SI    1
// CASILLA_OCUPADA 2
int elegir_mov(char candidatas[][DIM], char tablero[][DIM], char *f, char *c)
{
    int i, j, k, found;
    int mf = -1; // almacena la fila del mejor movimiento encontrado
    int mc;      // almacena la columna del mejor movimiento encontrado
    char mejor = 0; // almacena el mejor valor encontrado
    int patron, longitud;
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque

    // Recorremos todo el tablero comprobando dónde podemos mover
    // Comparamos la puntuación de los movimientos encontrados y nos quedamos con el mejor
    for (i=0; i<DIM; i++)
    {
        for (j=0; j<DIM; j++)
        {   // indica en qué casillas quizá se pueda mover
            if (candidatas[i][j] == SI)
            {
                if (tablero[i][j] == CASILLA_VACIA)
                {
                    found = 0;
                    k = 0;

                    // en este bucle comprobamos si es un movimiento válido
                    // (es decir si implica voltear en alguna dirección)
                    while ((found == 0) && (k < DIM))
                    {
                        SF = vSF[k];    // k representa la dirección que miramos
                        SC = vSC[k];    // 1 es norte, 2 NE, 3 E ...

                        // nos dice qué hay que voltear en cada dirección
                        longitud = 0;
                        patron = patron_volteo_test(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        //  //printf("%d ", patron);
                        if (patron == PATRON_ENCONTRADO)
                        {
                            found = 1;
                            if (tabla_valor[i][j] > mejor)
                            {
                                mf = i;
                                mc = j;
                                mejor = tabla_valor[i][j];
                            }
                        }
                        k++;
                        // si no hemos encontrado nada probamos con la siguiente dirección
                    }
                }
            }
        }
    }
    *f = (char) mf;
    *c = (char) mc;
    // si no se ha encontrado una posición válida devuelve -1
    return mf;
}
////////////////////////////////////////////////////////////////////////////////
// Cuenta el número de fichas de cada color.
// Los guarda en la dirección b (blancas) y n (negras)
void contar(char tablero[][DIM], int *b, int *n)
{
    int i,j;

    *b = 0;
    *n = 0;

    // recorremos todo el tablero contando las fichas de cada color
    for (i=0; i<DIM; i++)
    {
        for (j=0; j<DIM; j++)
        {
            if (tablero[i][j] == FICHA_BLANCA)
            {
                (*b)++;
            }
            else if (tablero[i][j] == FICHA_NEGRA)
            {
                (*n)++;
            }
        }
    }
}

void actualizar_candidatas(char candidatas[][DIM], char f, char c)
{
    // donde ya se ha colocado no se puede volver a colocar
    // En las posiciones alrededor sí
    candidatas[f][c] = CASILLA_OCUPADA;
    if (f > 0)
    {
        if (candidatas[f-1][c] != CASILLA_OCUPADA)
            candidatas[f-1][c] = SI;

        if ((c > 0) && (candidatas[f-1][c-1] != CASILLA_OCUPADA))
            candidatas[f-1][c-1] = SI;

        if ((c < 7) && (candidatas[f-1][c+1] != CASILLA_OCUPADA))
            candidatas[f-1][c+1] = SI;
    }
    if (f < 7)
    {
        if (candidatas[f+1][c] != CASILLA_OCUPADA)
            candidatas[f+1][c] = SI;

        if ((c > 0) && (candidatas[f+1][c-1] != CASILLA_OCUPADA))
            candidatas[f+1][c-1] = SI;

        if ((c < 7) && (candidatas[f+1][c+1] != CASILLA_OCUPADA))
            candidatas[f+1][c+1] = SI;
    }
    if ((c > 0) && (candidatas[f][c-1] != CASILLA_OCUPADA))
        candidatas[f][c-1] = SI;

    if ((c < 7) && (candidatas[f][c+1] != CASILLA_OCUPADA))
        candidatas[f][c+1] = SI;
}

void reversi_iniciar_candidatas(char candidatas[][DIM]) {
	int i, j;
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			candidatas[i][j] = NO;
		}
	}
}

void reversi_copiar_tablero(char entrada[][DIM], char salida[][DIM]) {
	int i, j;
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			salida[i][j] = entrada[i][j];
		}
	}
}

char reversi_terminado(char tablero[][DIM], char *ganador) {
	int blancas = 0, negras = 0;
	char fin = PARTIDA_ACABADA;

	int i, j;
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			if (tablero[i][j] == CASILLA_VACIA)
				fin = PARTIDA_SIGUE;
			else if (tablero[i][j] == FICHA_BLANCA)
				blancas++;
			else
				negras++;
		}
	}

	if (blancas == 0 || negras == 0)
		fin = PARTIDA_ACABADA;

	if (blancas > negras)
		*ganador = FICHA_BLANCA;
	else
		*ganador = FICHA_NEGRA;

	return fin;
}

////////////////////////////////////////////////////////////////////////////////
// Proceso principal del juego
// Utiliza el tablero,
// y las direcciones en las que indica el jugador la fila y la columna
// y la señal de ready que indica que se han actualizado fila y columna
// tablero, fila, columna y ready son variables globales aunque deberían ser locales de reversi8,
// la razón es que al meterlas en la pila no las pone juntas, y así jugar es más complicado.
// en esta versión el humano lleva negras y la máquina blancas
// no se comprueba que el humano mueva correctamente.
// Sólo que la máquina realice un movimiento correcto.
void reversi_main() {
	// Variables utilizadas
	uint32_t ahora;
	volatile char fila, columna;
	char reiniciar = PARTIDA_ACABADA;
	int done;     // la máquina ha conseguido mover o no
	int move = 0; // el humano ha conseguido mover o no
	int blancas, negras; // número de fichas de cada color
	char ganador;
	char __attribute__ ((aligned (8))) candidatas[DIM][DIM];
	char __attribute__ ((aligned (8))) tablero_aux[DIM][DIM];

	// Iniciar tablero
	reversi_iniciar_candidatas(candidatas);
	init_table(tablero, candidatas);

	while(1) {
		ahora = timer0_leer();

		latido_gestionar(ahora);

		// Lógica del juego
		if (jugada_gestionar(ahora, tiempo_patron, &fila, &columna, tablero) != 0) {
	        // si la fila o columna son 8 asumimos que el jugador no puede mover
	        if (((fila) < DIM) && ((columna) < DIM) && tablero[fila][columna] == CASILLA_VACIA)
	        {
	        	reversi_copiar_tablero(tablero, tablero_aux);
	            tablero_aux[fila][columna] = FICHA_NEGRA;
	            move = actualizar_tablero(tablero_aux, fila, columna, FICHA_NEGRA);

	            if (move == PATRON_ENCONTRADO) {
	            	reversi_copiar_tablero(tablero_aux, tablero);
		            actualizar_candidatas(candidatas, fila, columna);
	            }
	        }

	        // escribe el movimiento en las variables globales fila columna
	        done = elegir_mov(candidatas, tablero, &fila, &columna);
	        if (done != -1)
	        {
	            tablero[fila][columna] = FICHA_BLANCA;
	            actualizar_tablero(tablero, fila, columna, FICHA_BLANCA);
	            actualizar_candidatas(candidatas, fila, columna);
	        }

	        reiniciar = reversi_terminado(tablero, &ganador);

			if (reiniciar == PARTIDA_ACABADA) {
				reiniciar = PARTIDA_SIGUE;

				// Iniciar tablero
				reversi_iniciar_candidatas(candidatas);
				init_table(tablero, candidatas);
				jugada_ganador(ganador);
				tiempo_patron = 0;
			}
		}
	}

	contar(tablero, &blancas, &negras); // Si blancas > negras, ganan las blancas, sino las negras
}
