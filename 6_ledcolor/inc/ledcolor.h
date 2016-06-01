#ifndef LEDCOLOR_H
#define LEDCOLOR_H

/*==================[inclusions]=============================================*/
#include "led.h"
#include "timer.h"

/*==================[macros]=================================================*/
#define LED_COLOR_N			16
#define COLOR_TIMER_BASE	1
#define COLOR_TIMER_MAX		20


/*==================[typedef]================================================*/
typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} RGB_led_t;

//enum color {ROJO1=0, ROSA1, NARANJA1, ROSA2, VIOLETA1, VERDE1, VERDE2, CELESTE1, CELESTE2, UNDEF1, AZUL1, VIOLETA2, CELESTE3};

/* Variables */
RGB_led_t color[LED_COLOR_N];
uint8_t base_cnt, r_cnt, g_cnt, b_cnt;
uint8_t c_idx;

/*==================[external functions declaration]=========================*/
void ISR_RITHandler(void);
void LedColorInit(void);
void LedColorSet(uint8_t color_idx);

#endif
