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
#define LED_BLK_MS		1000

#define DAC_MS			50
#define DAC_HIGH_VAL	600
#define DAC_LOW_VAL		300

#define ADC_MS			10
#define ADC_MAX			1023
#define VOLTS_MAX		3.300f

#define N_GAIN			5
#define N_OFFSET		5

#define SIGNAL_MIN		1000.0f

/*==================[internal data declaration]==============================*/
const float gain[] = {0.8, 0.9, 1.0, 1.1, 1.2};
const float offset[] = {0.2, 0.1, 0.0, -0.1, -0.2};
uint8_t gain_idx = 2;
uint8_t offset_idx = 2;

float signal_max, signal_min, signal_offset;

/* String declaration */
const char* max_str = "MAX:";
const char* min_str = "MIN:";
const char* gain_str = "GAIN:";
const char* off_str = "OFF:";

#define MAX_STR_N	4
#define MIN_STR_N	4
#define GAIN_STR_N	5
#define OFF_STR_N	4


/*==================[internal functions declaration]=========================*/
void ISR_RITHandler(void)
{
	static uint32_t led_ms = LED_BLK_MS;
	static uint32_t dac_ms = DAC_MS;
	static uint32_t dac_val = DAC_HIGH_VAL;
	static uint32_t adc_ms = ADC_MS;

	/* DAC Test signal generation */
	dac_ms--;
	if(dac_ms == 0)
	{
		if(dac_val == DAC_HIGH_VAL)
			dac_val = DAC_LOW_VAL;
		else
			dac_val = DAC_HIGH_VAL;

		SetDac(dac_val);
		dac_ms = DAC_MS;
	}

	/* LED and UART */
	led_ms--;
	if(led_ms == 0)
	{
		WriteUartNBytes(UART2, max_str, MAX_STR_N);
		SendUartFloatAscii(UART2, signal_max, 3);
		WriteUartNBytes(UART2, min_str, MIN_STR_N);
		SendUartFloatAscii(UART2, signal_min, 3);
		WriteUartNBytes(UART2, gain_str, GAIN_STR_N);
		SendUartFloatAscii(UART2, gain[gain_idx], 3);
		WriteUartNBytes(UART2, off_str, OFF_STR_N);
		SendUartFloatAscii(UART2, offset[offset_idx], 3);

		LED_Toggle(LED_G);
		led_ms = LED_BLK_MS;
	}

	/* ADC */
	adc_ms--;
	if(adc_ms == 0)
	{
		AdcStartNow();
		adc_ms = ADC_MS;
	}

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
	uint32_t adc_val, adc_val_prev = 0;
	float volts;

	/* Initialize drivers */
	LED_Init();
	KEY_Init();
	InitDac(0);
	InitAdc(ADC_CH1);
	InitUart(UART2, 115200);
	RIT_Init(TIMER_MS_BASE);

	while(1)
	{
		/* KEY1: Increase GAIN (decrease index) */
		if( (KEY_IsPressed(KEY1) == true) && (gain_idx != 0) )
		{
			while(KEY_IsPressed(KEY1) ==  true) {}
			gain_idx--;
		}
		/* KEY2: Decrease GAIN (increase index) */
		if( (KEY_IsPressed(KEY2) == true) && (gain_idx < N_GAIN-1) )
		{
			while(KEY_IsPressed(KEY2) ==  true) {}
			gain_idx++;
		}
		/* KEY3: Increase OFFSET (decrease index) */
		if( (KEY_IsPressed(KEY3) == true) && (offset_idx != 0) )
		{
			while(KEY_IsPressed(KEY3) ==  true) {}
			offset_idx--;
		}
		/* KEY4: Decrease OFFSET (increase index) */
		if( (KEY_IsPressed(KEY4) == true) && (offset_idx < N_OFFSET-1) )
		{
			while(KEY_IsPressed(KEY4) ==  true) {}
			offset_idx++;
		}

		/* Reading A/D Converter, and process signal */
		signal_max = 0.0;
		signal_min = SIGNAL_MIN;
		adc_val = ReadAdcPooling(ADC_CH1);
		if(adc_val != adc_val_prev)
		{
			volts = VOLTS_MAX*(float)adc_val / ADC_MAX;
			volts = volts*gain[gain_idx] + offset[offset_idx];

			/* Signal MIN/MAX */
			if(volts > signal_max)
				signal_max = volts;
			if(volts < signal_min)
				signal_min = volts;

			/* Save current value as previous */
			adc_val_prev = adc_val;
			signal_max = 0;
			signal_min = SIGNAL_MIN;
		}
	}
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

