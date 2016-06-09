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
#define ADC_MAX			1023
#define VOLTS_MAX		3.3f

/*==================[internal data declaration]==============================*/


/*==================[internal functions declaration]=========================*/
void ISR_RITHandler(void)
{
	static uint8_t led_ms = LED_BLK_MS;
	static uint8_t adc_ms = ADC_MS;

	/* Led blinking */
	if(--led_ms == 0)
	{
		LED_Toggle(LED_RGB_B);
		led_ms = LED_BLK_MS;
	}

	/* ADC */
	if(--adc_ms == 0)
	{
		ADC_StartNow();
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
	uint32_t adc_val = 0;
	uint8_t byte, digit, volts_int, volts_dec, num;
	float volts;

	/* Initialize drivers */
	LED_Init();
	ADC_Init(ADC_CH1);
	InitUart(UART2, 115200);
	RIT_Init(TIMER_MS_BASE);

	while(1)
	{
		/* Reading UART2 */
		byte = ReadUartByte(UART2);

		switch(byte)
		{
			case 'r':
			case 'R':
				LED_Toggle(LED_R);
				break;
			case 'a':
			case 'A':
				LED_Toggle(LED_Y);
				break;
			case 'v':
			case 'V':
				LED_Toggle(LED_G);
				break;
		}

		/* Reading A/D Converter */
		adc_val = ADC_ReadPooling(ADC_CH1);

		volts = VOLTS_MAX*adc_val / ADC_MAX;
		volts_int = (uint8_t)volts;
		volts = volts - volts_int;
		volts_dec = (uint8_t)(volts*1000);

		WriteUartByte(UART2, volts_int + '0');		/* Integer part */
		WriteUartByte(UART2, '.');					/* Decimal point */

		num = volts_dec / 100;
		WriteUartByte(UART2, num + '0');
		volts_dec = volts_dec - num*100;

		num = volts_dec / 10;
		WriteUartByte(UART2, num + '0');
		volts_dec = volts_dec - num*10;

		WriteUartByte(UART2, volts_dec + '0');

		WriteUartByte(UART2, '\r');
	}
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

