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

/** \brief Blinking Bare Metal driver led
 **
 **
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */

/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Baremetal Bare Metal LED Driver
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

#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif
#include "led.h"


/*==================[macros and definitions]=================================*/
#define PIN_LEDS_PKG		2

#define PIN_LED0_R			0
#define PIN_LED0_G			1
#define PIN_LED0_B			2
#define PIN_LED1			14
#define PIN_LED2			11
#define PIN_LED3			12

#define GPIO0				0
#define GPIO1				1
#define GPIO5				5

#define GPIO5_LED_MASK		(1<<0) | (1<<1) | (1<<2)
#define GPIO0_LED_MASK		1<<14
#define GPIO1_LED_MASK		(1<<11) | (1<<12)

#define GPIO_INPUT			0
#define GPIO_OUTPUT			1


/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void LED_Init(void)
{
	Chip_GPIO_Init(LPC_GPIO_PORT);
	Chip_SCU_PinMux(PIN_LEDS_PKG, PIN_LED0_R, MD_PUP, FUNC4);	// Mapping P2_0 to GPIO5[0]
	Chip_SCU_PinMux(PIN_LEDS_PKG, PIN_LED0_G, MD_PUP, FUNC4);	// Mapping P2_1 to GPIO5[1]
	Chip_SCU_PinMux(PIN_LEDS_PKG, PIN_LED0_B, MD_PUP, FUNC4);	// Mapping P2_2 to GPIO5[2]

	Chip_SCU_PinMux(PIN_LEDS_PKG, PIN_LED1, MD_PUP, FUNC0);		// Mapping P2_10 to GPIO0[14]
	Chip_SCU_PinMux(PIN_LEDS_PKG, PIN_LED2, MD_PUP, FUNC0);		// Mapping P2_11 to GPIO1[11]
	Chip_SCU_PinMux(PIN_LEDS_PKG, PIN_LED3, MD_PUP, FUNC0);		// Mapping P2_12 to GPI01[12]

	/* GPIO5 LEDs pins to OUTPUT */
	Chip_GPIO_SetDir(LPC_GPIO_PORT, GPIO5, GPIO5_LED_MASK, GPIO_OUTPUT);
	/* GPIO0 LEDs pins to OUTPUT */
	Chip_GPIO_SetDir(LPC_GPIO_PORT, GPIO0, GPIO0_LED_MASK, GPIO_OUTPUT);
	/* GPIO1 LEDs pins to OUTPUT */
	Chip_GPIO_SetDir(LPC_GPIO_PORT, GPIO1, GPIO1_LED_MASK, GPIO_OUTPUT);

	/* Turning-off all LEDs */
	Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO5, PIN_LED0_R);
	Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO5, PIN_LED0_G);
	Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO5, PIN_LED0_B);
	Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO0, PIN_LED1);
	Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO1, PIN_LED2);
	Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO1, PIN_LED3);
}

void LED_TurnOn(uint8_t led)
{
	switch(led)
	{
	case LED_RGB_R:
		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, GPIO5, PIN_LED0_R);
		break;
	case LED_RGB_G:
		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, GPIO5, PIN_LED0_G);
		break;
	case LED_RGB_B:
		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, GPIO5, PIN_LED0_B);
		break;
	case LED_R:
		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, GPIO0, PIN_LED1);
		break;
	case LED_Y:
		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, GPIO1, PIN_LED2);
		break;
	case LED_G:
		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, GPIO1, PIN_LED3);
		break;
	}
}

void LED_TurnOff(uint8_t led)
{
	switch(led)
	{
	case LED_RGB_R:
		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO5, PIN_LED0_R);
		break;
	case LED_RGB_G:
		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO5, PIN_LED0_G);
		break;
	case LED_RGB_B:
		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO5, PIN_LED0_B);
		break;
	case LED_R:
		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO0, PIN_LED1);
		break;
	case LED_Y:
		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO1, PIN_LED2);
		break;
	case LED_G:
		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, GPIO1, PIN_LED3);
		break;
	}
}

void LED_Toggle(uint8_t led)
{
	switch(led)
	{
	case LED_RGB_R:
		Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, GPIO5, PIN_LED0_R);
		break;
	case LED_RGB_G:
		Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, GPIO5, PIN_LED0_G);
		break;
	case LED_RGB_B:
		Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, GPIO5, PIN_LED0_B);
		break;
	case LED_R:
		Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, GPIO0, PIN_LED1);
		break;
	case LED_Y:
		Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, GPIO1, PIN_LED2);
		break;
	case LED_G:
		Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, GPIO1, PIN_LED3);
		break;
	}
}

void LED_TurnOnAll(void)
{
	/* ToDo */
}

void LED_TurnOffAll(void)
{
	/* ToDo */
}

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




/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

