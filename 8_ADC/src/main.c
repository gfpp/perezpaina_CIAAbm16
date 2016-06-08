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
#define LED_BLK_MS		100
#define ADC_MS			10
#define HIGH_THR_MAX	900
#define HIGH_THR_MIN	500
#define LOW_THR_MAX		500
#define LOW_THR_MIN		100
#define	THR_STEP		10

/*==================[internal data declaration]==============================*/


/*==================[internal functions declaration]=========================*/
void ISR_RITHandler(void)
{
	static uint8_t led_ms = LED_BLK_MS;
	static uint8_t adc_ms = ADC_MS;

	/* Led blinking */
	if(--led_ms == 0)
	{
		LED_Toggle(LED_Y);
		led_ms = LED_BLK_MS;
	}

	/* ADC */
	if(--adc_ms == 0)
	{
		AdcStartNow();
		LED_Toggle(LED_RGB_B);
		adc_ms = ADC_MS;
	}

	TimerClearFlag();
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
	uint32_t val = 0;
	uint32_t high_thr = 600;
	uint32_t low_thr = 200;

	/* Initialize drivers */
	LED_Init();
	InitKey();
	InitAdc(ADC_CH1);
	InitDac(val);
	InitTimer(TIMER_MS_BASE);

	while(1)
	{
		val = ReadAdcPooling(ADC_CH1);
		SetDac(val);

		/* KEY1: Increase HIGH threshold */
		if( (KeyPressed(KEY1) == false) && (high_thr < HIGH_THR_MAX) )
		{
			while(KeyPressed(KEY1) ==  false) {}
			high_thr += THR_STEP;
		}
		/* KEY2: Increase HIGH threshold */
		if( (KeyPressed(KEY2) == false) && (high_thr > HIGH_THR_MIN) )
		{
			while(KeyPressed(KEY1) ==  false) {}
			high_thr -= THR_STEP;
		}
		/* KEY3: Increase LOW threshold */
		if( (KeyPressed(KEY3) == false) && (low_thr < LOW_THR_MAX) )
		{
			while(KeyPressed(KEY3) ==  false) {}
			low_thr += THR_STEP;
		}
		/* KEY4: Increase LOW threshold */
		if( (KeyPressed(KEY4) == false) && (low_thr > LOW_THR_MIN) )
		{
			while(KeyPressed(KEY4) ==  false) {}
			low_thr -= THR_STEP;
		}

		if(val > high_thr)
			LED_TurnOn(LED_R);
		else
			LED_TurnOff(LED_R);

		if(val < low_thr)
			LED_TurnOn(LED_G);
		else
			LED_TurnOff(LED_G);
	}
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

