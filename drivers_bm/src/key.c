/* Copyright 2016, XXXXXXXXX  
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
#include "key.h"


/*==================[macros and definitions]=================================*/
#define PIN_PACKAGE_KEYS	1

#define PIN_KEY1			0
#define PIN_KEY2			1
#define PIN_KEY3			2
#define PIN_KEY4			6

#define KEY1_PORT_NUM		4
#define KEY2_PORT_NUM		8
#define KEY3_PORT_NUM		9
#define KEY4_PORT_NUM		9

#define GPIO0				0
#define GPIO1				1

#define GPIO0_KEY_MASK		(1<<4) | (1<<8) | (1<<9)
#define GPIO1_KEY_MASK		1<<9

#define GPIO_INPUT			0
#define GPIO_OUTPUT			1


/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
/* GPIO5 LEDs pins to OUTPUT */

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void InitKey(void)
{
	Chip_GPIO_Init(LPC_GPIO_PORT);

	Chip_SCU_PinMux(PIN_PACKAGE_KEYS, PIN_KEY1, MD_PUP|MD_EZI|MD_ZI, FUNC0);// Mapping P1_0 to GPIO0[4], SW1
	Chip_SCU_PinMux(PIN_PACKAGE_KEYS, PIN_KEY2, MD_PUP|MD_EZI|MD_ZI, FUNC0);// Mapping P1_1 to GPIO0[8], SW2
	Chip_SCU_PinMux(PIN_PACKAGE_KEYS, PIN_KEY3, MD_PUP|MD_EZI|MD_ZI, FUNC0);// Mapping P1_2 to GPIO0[9], SW3
	Chip_SCU_PinMux(PIN_PACKAGE_KEYS, PIN_KEY4, MD_PUP|MD_EZI|MD_ZI, FUNC0);// Mapping P1_6 to GPIO1[9], SW4

	/* GPIO0 KEYs pins to INPUT */
	Chip_GPIO_SetDir(LPC_GPIO_PORT, GPIO0, GPIO0_KEY_MASK, GPIO_INPUT);
	/* GPIO1 KEYs pins to INPUT */
	Chip_GPIO_SetDir(LPC_GPIO_PORT, GPIO1, GPIO1_KEY_MASK, GPIO_INPUT);
}

bool KeyPressed(uint8_t key)
{
	bool press;

	if(key == KEY1)
	{
		press = Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, GPIO0, KEY1_PORT_NUM);
	}
	if(key == KEY2)
	{
		press = Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, GPIO0, KEY2_PORT_NUM);
	}
	if(key == KEY3)
	{
		press = Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, GPIO0, KEY3_PORT_NUM);
	}
	if(key == KEY4)
	{
		press = Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, GPIO1, KEY4_PORT_NUM);
	}

	return press;
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

