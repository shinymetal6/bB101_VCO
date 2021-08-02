/*
 * pushbutton.c
 *
 *  Created on: Jul 5, 2021
 *      Author: fil
 */

#include "main.h"
uint8_t		reset_parameters_cntr = 0;

void EmergencyReset(void)
{
	bzero((uint8_t *)&SystemFlags,sizeof(SystemFlags));
	bzero((uint8_t *)&Oscillator[0],sizeof(Oscillator));
	bzero((uint8_t *)&SystemFlags,sizeof(SystemFlags));
	bzero((uint8_t *)&osc_buffer,sizeof(osc_buffer));
	bzero((uint8_t *)&oscout_buffer,sizeof(oscout_buffer));
	InitialParameters();
	InitOscillators();
	Main_Menu();
	MenusDrawEmptyStatus();
	MenusDrawStatus();
	reset_parameters_cntr = 0;

}

uint32_t poll_pushbtn(void)
{
	if ( SystemFlags.buttons_debounce != 0 )
	{
		SystemFlags.buttons_debounce --;
		return GPIO_PIN_RESET;
	}
	if (( HAL_GPIO_ReadPin(TACT0_GPIO_Port, TACT0_Pin) == GPIO_PIN_RESET))
	{
		SystemFlags.buttons_flag |= BUTTON_TACT0;
	}
	if (( HAL_GPIO_ReadPin(TACT1_GPIO_Port, TACT1_Pin) == GPIO_PIN_RESET))
	{
		SystemFlags.buttons_flag |= BUTTON_TACT1;
	}
	if (( HAL_GPIO_ReadPin(TACT2_GPIO_Port, TACT2_Pin) == GPIO_PIN_RESET))
	{
		SystemFlags.buttons_flag |= BUTTON_TACT2;
	}
	if ((SystemFlags.buttons_flag  & BUTTON_MASK ) != 0 )
	{
		if ((SystemFlags.buttons_flag  & BUTTON_EMERGENCY_MASK ) == BUTTON_EMERGENCY_MASK )
		{
			reset_parameters_cntr++;
			if ( reset_parameters_cntr >= 10 )
				EmergencyReset();
		}
		else
		{
			reset_parameters_cntr = 0;
			DoMenus();
		}
		SystemFlags.buttons_debounce = 2;
	}
	return GPIO_PIN_SET;
}
