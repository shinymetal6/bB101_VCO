/*
 * irq_manager.c
 *
 *  Created on: Jun 29, 2021
 *      Author: fil
 */

#include "main.h"

void get_limits(uint16_t *start,uint16_t *end)
{
	if (( SystemFlags.audio_flags & AUDIO_VCO_HALF_FLAG ) == AUDIO_VCO_HALF_FLAG)
	{
		*start=0;
		*end = HALF_NUMBER_OF_AUDIO_SAMPLES;
	}
	else
	{
		*start=HALF_NUMBER_OF_AUDIO_SAMPLES;
		*end = NUMBER_OF_AUDIO_SAMPLES;
	}
}

void IrqProcessSamples(void)
{
uint16_t	start,end;

	HAL_GPIO_WritePin(GPIOB, FLAG_Pin, GPIO_PIN_SET);

	if (( SystemFlags.oscillator_flags & OSC_SRCFLAG ) == OSC_SRCFLAG)
		RunOscillator32();
	else
		RunOscillator4();
	if (( SystemFlags.control_flags & CONTROL_OSC_VCF_DLY ) == CONTROL_OSC_VCF_DLY)
	{
		Moog_VCF(&pipe0[0],&oscout_buffer[0]);
		DelayLine(&pipe1[0],&pipe0[0]);
	}
	else
	{
		DelayLine(&pipe0[0],&oscout_buffer[0]);
		Moog_VCF(&pipe1[0],&pipe0[0]);
	}

	get_limits(&start,&end);
	Vca(&signal_out[start],&pipe1[0]);
	UsbMidiCheck();

	HAL_GPIO_WritePin(GPIOB, FLAG_Pin, GPIO_PIN_RESET);
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
	SystemFlags.audio_flags |= (AUDIO_VCO_HALF_FLAG | AUDIO_READY_FLAG);
	IrqProcessSamples();
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
	SystemFlags.audio_flags &= ~AUDIO_VCO_HALF_FLAG;
	SystemFlags.audio_flags |= AUDIO_READY_FLAG;
	IrqProcessSamples();
}

void SysTimer_callback(void)
{
	SystemFlags.control_flags |= CONTROL_TICK_FLAG;
}

void ADC_callback(void)
{
	SystemFlags.control_flags |= CONTROL_ADC_FLAG;
}
