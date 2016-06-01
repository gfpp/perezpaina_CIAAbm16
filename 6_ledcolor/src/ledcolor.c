#include "ledcolor.h"


void ISR_RITHandler(void)
{
	base_cnt--;
	if(base_cnt == 0)
	{
		base_cnt = COLOR_TIMER_MAX;

		r_cnt = color[c_idx].r;
		g_cnt = color[c_idx].g;
		b_cnt = color[c_idx].b;
	}

	if(r_cnt != 0) r_cnt--;
	if(r_cnt == 0)
		TurnOffLed(LED_RGB_R);
	else
		TurnOnLed(LED_RGB_R);

	if(g_cnt != 0) g_cnt--;
	if(g_cnt == 0)
		TurnOffLed(LED_RGB_G);
	else
		TurnOnLed(LED_RGB_G);

	if(b_cnt != 0) b_cnt--;
	if(b_cnt == 0)
		TurnOffLed(LED_RGB_B);
	else
		TurnOnLed(LED_RGB_B);

	TimerClearFlag();
}

void LedColorInit(void)
{
	r_cnt = 0;
	g_cnt = 0;
	b_cnt = 0;
	base_cnt = 0;

	c_idx = 0;		/* Color index */

	/* Color initialization */
	color[0].r = 20;
	color[0].g = 0;
	color[0].b = 0;

	color[1].r = 15;
	color[1].g = 2;
	color[1].b = 2;

	color[2].r = 15;
	color[2].g = 2;
	color[2].b = 0;

	color[3].r = 15;
	color[3].g = 0;
	color[3].b = 2;

	color[4].r = 10;
	color[4].g = 0;
	color[4].b = 5;

	color[5].r = 0;
	color[5].g = 20;
	color[5].b = 0;

	color[6].r = 5;
	color[6].g = 15;
	color[6].b = 0;

	color[7].r = 3;
	color[7].g = 20;
	color[7].b = 3;

	color[8].r = 0;
	color[8].g = 15;
	color[8].b = 2;

	color[9].r = 5;
	color[9].g = 20;
	color[9].b = 10;

	color[10].r = 0;
	color[10].g = 0;
	color[10].b = 20;

	color[11].r = 5;
	color[11].g = 0;
	color[11].b = 15;

	color[12].r = 0;
	color[12].g = 10;
	color[12].b = 20;

	color[13].r = 15;
	color[13].g = 0;
	color[13].b = 20;

	color[14].r = 0;
	color[14].g = 15;
	color[14].b = 20;

	color[15].r = 20;
	color[15].g = 5;
	color[15].b = 20;

	/* Initialize drivers */
	InitLed();
	InitTimer(COLOR_TIMER_BASE);

	/* Turn off RGB led */
	TurnOffLed(LED_RGB_R);
	TurnOffLed(LED_RGB_G);
	TurnOffLed(LED_RGB_B);
}

void LedColorSet(uint8_t color_idx)
{
	c_idx = color_idx;
	base_cnt = COLOR_TIMER_MAX;

	r_cnt = color[c_idx].r;
	g_cnt = color[c_idx].g;
	b_cnt = color[c_idx].b;
}
