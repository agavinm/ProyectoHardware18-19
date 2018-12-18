/*********************************************************************************************
* Fichero:		simular.h
* Autores: 		Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
* Descrip:		funciones y variables de simulación de la placa
* Version:
*********************************************************************************************/

#ifndef _SIMULAR_H_
#define _SIMULAR_H_

//#define simulacion // comentar si no es una simulación
#ifdef simulacion

#include "stdint.h"


/*--- declaración de funciones visibles del módulo simular.c/simular.h ---*/

// common
#define _ISR_STARTADDRESS 0xc7fff00

#ifndef ULONG
#define ULONG	unsigned long
#endif

#define UINT	unsigned int
#define USHORT	unsigned short
#define UCHAR	unsigned char
#define U32 	unsigned int
#define INT32U  unsigned int
#define INT32	int
#define U16 	unsigned short
#define INT16U  unsigned short
#define INT16	short int
#define S32 	int
#define S16 	short int
#define U8  	unsigned char
#define INT8U 	unsigned char
#define byte	unsigned char
#define INT8 	char
#define	S8  	char

#ifndef FALSE
#define FALSE		0
#endif
#ifndef TRUE
#define TRUE		1
#endif

#define OK		1
#define FAIL	0
#define FileEnd	1
#define	NotEnd	0

void sys_init();

// 8led
void D8Led_init();
void D8Led_symbol(int value);

// button
typedef enum estado_button {button_none, button_iz, button_dr} estado_button;
typedef void(function_ptr)(estado_button boton);

void button_iniciar(void);
estado_button button_estado();
void button_empezar(function_ptr *callback);
void button_habilitar_interr();

// excepcion
void init_excepcion();

// led
void leds_off();						// todos los leds apagados (*)
void led1_on();							// led 1 encendido (*)
void led1_off();						// led 1 apagado

// lcd
/* tamano pantalla */
#define TLCD_160_240 (0)
#define VLCD_240_160 (1)
#define CLCD_240_320 (2)
#define MLCD_320_240 (3)
#define ELCD_640_480 (4)
#define SLCD_160_160 (5)
#define LCD_TYPE MLCD_320_240

#if (LCD_TYPE == TLCD_160_240)
#define SCR_XSIZE (160)
#define SCR_YSIZE (240)
#define LCD_XSIZE (160)
#define LCD_YSIZE (240)
#elif (LCD_TYPE == VLCD_240_160)
#define SCR_XSIZE (240)
#define SCR_YSIZE (160)
#define LCD_XSIZE (240)
#define LCD_YSIZE (160)
#elif (LCD_TYPE == CLCD_240_320)
#define SCR_XSIZE (240)
#define SCR_YSIZE (320)
#define LCD_XSIZE (240)
#define LCD_YSIZE (320)
#elif (LCD_TYPE == MLCD_320_240)
#define SCR_XSIZE (320)
#define SCR_YSIZE (240)
#define LCD_XSIZE (320)
#define LCD_YSIZE (240)
#elif (LCD_TYPE == ELCD_640_480)
#define SCR_XSIZE (640)
#define SCR_YSIZE (480)
#define LCD_XSIZE (640)
#define LCD_YSIZE (480)
#elif (LCD_TYPE == SLCD_160_160)
#define SCR_XSIZE (160)
#define SCR_YSIZE (160)
#define LCD_XSIZE (160)
#define LCD_YSIZE (160)
#endif

/* screen color */
#define MODE_MONO (1)
#define MODE_GREY4 (4)
#define MODE_GREY16 (16)
#define MODE_COLOR (256)

#define Ascii_W 8
#define XWIDTH 6
#define BLACK 0xf
#define WHITE 0x0
#define LIGHTGRAY 0x5
#define DARKGRAY 0xa
#define TRANSPARENCY 0xff

#define HOZVAL (LCD_XSIZE / 4 - 1)
#define HOZVAL_COLOR (LCD_XSIZE * 3 / 8 - 1)
#define LINEVAL (LCD_YSIZE - 1)
#define MVAL (13)
#define M5D(n) ((n)&0x1fffff)
#define MVAL_USED 0

/* tamano array */
#define ARRAY_SIZE_MONO (SCR_XSIZE / 8 * SCR_YSIZE)
#define ARRAY_SIZE_GREY4 (SCR_XSIZE / 4 * SCR_YSIZE)
#define ARRAY_SIZE_GREY16 (SCR_XSIZE / 2 * SCR_YSIZE)
#define ARRAY_SIZE_COLOR (SCR_XSIZE / 1 * SCR_YSIZE)

/* clkval */
#define CLKVAL_MONO (12)
#define CLKVAL_GREY4 (12)
#define CLKVAL_GREY16 (12)
#define CLKVAL_COLOR (10)

#define LCD_BUF_SIZE (SCR_XSIZE * SCR_YSIZE / 2)
#define LCD_ACTIVE_BUFFER (0xc300000)
#define LCD_VIRTUAL_BUFFER (0xc300000 + LCD_BUF_SIZE)

#define LCD_PutPixel(x, y, c)                                                  \
  (*(INT32U *)(LCD_VIRTUAL_BUFFER + (y)*SCR_XSIZE / 2 + ((x)) / 8 * 4)) =      \
      (*(INT32U *)(LCD_VIRTUAL_BUFFER + (y)*SCR_XSIZE / 2 + ((x)) / 8 * 4)) &  \
          (~(0xf0000000 >> ((((x)) % 8) * 4))) |                               \
      ((c) << (7 - ((x)) % 8) * 4)
#define LCD_Active_PutPixel(x, y, c)                                           \
  (*(INT32U *)(LCD_ACTIVE_BUFFER + (y)*SCR_XSIZE / 2 + (319 - (x)) / 8 * 4)) = \
      (*(INT32U *)(LCD_ACTIVE_BUFFER + (y)*SCR_XSIZE / 2 +                     \
                   (319 - (x)) / 8 * 4)) &                                     \
          (~(0xf0000000 >> (((319 - (x)) % 8) * 4))) |                         \
      ((c) << (7 - (319 - (x)) % 8) * 4)

#define GUISWAP(a, b)                                                          \
  {                                                                            \
    a ^= b;                                                                    \
    b ^= a;                                                                    \
    a ^= b;                                                                    \
  }

//Funciones
void Lcd_Init(void);
void Lcd_Active_Clr(void);
void Lcd_Clr(void);
void Lcd_DspAscII8x16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U *s);
void Lcd_Draw_Box(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom,
                  INT8U ucColor);
void Lcd_Dma_Trans(void);

// TP
void TS_init(void);
int TS_puls();

// timer0
void timer0_inicializar();
void timer0_empezar();
uint32_t timer0_parar();
uint32_t timer0_leer();

// timer2
void timer2_inicializar();
void timer2_empezar();
uint32_t timer2_parar();
uint32_t timer2_leer();


#endif

#endif /* _SIMULAR_H_ */
