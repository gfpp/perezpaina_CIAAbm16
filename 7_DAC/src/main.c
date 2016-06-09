/* Copyright 2016, Gonzalo Perez Paina
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief Blinking Bare Metal example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */

/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Baremetal Bare Metal example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "../inc/main.h"       /* <= own header */


/*==================[macros and definitions]=================================*/
#define TIMER_MS_BASE	1
#define DAC_VAL_MAX		900
#define DAC_VAL_MIN		100
#define LED_BLK_MS		500
#define PERIOD_INIT		100
#define PERIOD_MAX		500
#define PERIOD_MIN		50
#define KEY_MS			50

/*==================[internal data declaration]==============================*/


/*==================[internal functions declaration]=========================*/
void ISR_RITHandler(void)
{
	static uint32_t led_ms = LED_BLK_MS;
	static uint32_t period = PERIOD_INIT;
	static uint32_t n_period = PERIOD_INIT;

	static uint32_t dac_val_max = DAC_VAL_MAX;
	static uint32_t dac_val = DAC_VAL_MAX;
	static float dac_inc = DAC_VAL_MAX / PERIOD_INIT;

	static uint8_t key_ms = 0;

	/* Led blinking */
	if(--led_ms == 0)
	{
		LED_Toggle(LED_G);
		led_ms = LED_BLK_MS;
	}
	if(key_ms > 0)
		key_ms--;

	/* KEY1: Increase signal amplitude */
	if( (KEY_IsPressed(KEY1) == true) && (dac_val_max < DAC_VAL_MAX) && (key_ms == 0) )
	{
		dac_val_max += 10;
		dac_inc = (float)dac_val_max / period;
		LED_Toggle(LED_R);
		key_ms = KEY_MS;
	}
	/* KEY2: Decrease signal amplitude */
	if( (KEY_IsPressed(KEY2) == true) && (dac_val_max > DAC_VAL_MIN) && (key_ms == 0) )
	{
		dac_val_max -= 10;
		dac_inc = (float)dac_val_max / period;
		LED_Toggle(LED_Y);
		key_ms = KEY_MS;
	}
	/* KEY3: Increase signal period */
	if( (KEY_IsPressed(KEY3) == true) && (period < PERIOD_MAX) && (key_ms == 0) )
	{
		period++;
		dac_inc = (float)dac_val_max / period;
		LED_Toggle(LED_R);
		key_ms = KEY_MS;
	}
	/* KEY4: Decrease signal period */
	if( (KEY_IsPressed(KEY4) == true) && (period > PERIOD_MIN) && (key_ms == 0) )
	{
		period--;
		dac_inc = (float)dac_val_max / period;
		LED_Toggle(LED_R);
		key_ms = KEY_MS;
	}

	/* DAC signal generation */
	dac_val += (uint32_t)dac_inc;
	if(n_period > 0)
		n_period--;
	else
	{
		n_period = period;
		dac_val = 0;
	}
	SetDac(dac_val);
	RIT_ClearFlag();
}

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/


/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */



int main(void)
{
	uint32_t i;

	/* Initialize drivers */
	LED_Init();
	RIT_Init(TIMER_MS_BASE);
	KEY_Init();
	InitDac(0);

	while(1)
	{
	}
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

