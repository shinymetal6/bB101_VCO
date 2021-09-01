/*
 * delay_line.c
 *
 *  Created on: Jul 1, 2021
 *      Author: fil
 */

#include "main.h"

__attribute__ ((aligned (16))) uint8_t	delay_line[DELAY_LINE_SIZE];
#ifdef OLD
void DelayLine(uint16_t *buffer_out , uint16_t *buffer_in)
{
uint8_t		*line_in_ptr,*line_out_ptr;
uint32_t	i;

	if ((( SystemFlags.delay_flags & DLY_ENABLED ) == 0) || (SystemFlags.delay_value == 0 ))
	{
		for ( i=0;i<HALF_NUMBER_OF_AUDIO_SAMPLES;i++)
			*buffer_out++ = *buffer_in++;
	}
	else
	{
		line_in_ptr   	=  &delay_line[SystemFlags.delay_insertion_pointer];
		line_out_ptr  	=  &delay_line[SystemFlags.delay_extraction_pointer];
		for ( i=0;i<HALF_NUMBER_OF_AUDIO_SAMPLES;i++,buffer_in++,buffer_out++)
		{
			*buffer_out = (uint16_t )(((float )*buffer_in * SystemFlags.delay_sample_multiplier) + (float )(*line_out_ptr << 4) * SystemFlags.delay_feedback_multiplier);
			if ((SystemFlags.delay_flags & DLY_REVERB_NFLANGER_BIT ) == 0)
				*line_in_ptr = *buffer_in >> 4;
			else
				*line_in_ptr = *buffer_out >> 4;
			line_in_ptr++;
			line_out_ptr++;
		}
		SystemFlags.delay_insertion_pointer += HALF_NUMBER_OF_AUDIO_SAMPLES;
		SystemFlags.delay_insertion_pointer &= (DELAY_LINE_SIZE -1 );
		SystemFlags.delay_extraction_pointer += HALF_NUMBER_OF_AUDIO_SAMPLES;
		SystemFlags.delay_extraction_pointer &= (DELAY_LINE_SIZE -1 );
	}
}

void SetDelay(void)
{
	SystemFlags.delay_extraction_pointer = DELAY_LINE_SIZE - DELAY_STEP*SystemFlags.delay_value;
	SystemFlags.delay_extraction_pointer &= (DELAY_LINE_SIZE -1 );
}
#else
void DelayLine(uint16_t *buffer_out , uint16_t *buffer_in)
{
uint32_t	i,del_sample,delay_index;
uint16_t	delay;


	if (( SystemFlags.delay_flags & DLY_MIXER_POT_MASK) == 0 )	// comes from prog
		delay = SystemFlags.delay_value_from_prog;
	else
		delay = SystemFlags.delay_value_from_midi;

	for ( i=0;i<HALF_NUMBER_OF_AUDIO_SAMPLES;i++)
	{
		if ((( SystemFlags.delay_flags & DLY_ENABLED ) == 0) || (delay == 0 ))
			buffer_out[i] = buffer_in[i];
		else
		{
			delay_index = (SystemFlags.delay_insertion_pointer - delay*44) & (DELAY_LINE_SIZE-1);
			del_sample = delay_line[delay_index];
			buffer_out[i] = (uint16_t )(((float )buffer_in[i] * SystemFlags.delay_sample_multiplier) + (float )(del_sample << 4) * SystemFlags.delay_feedback_multiplier);
			if ((SystemFlags.delay_flags & DLY_REVERB_NFLANGER_BIT ) == 0)
				delay_line[SystemFlags.delay_insertion_pointer] = buffer_in[i] >> 4;
			else
				delay_line[SystemFlags.delay_insertion_pointer] = buffer_out[i] >> 4;
			SystemFlags.delay_insertion_pointer++;
			SystemFlags.delay_insertion_pointer &= (DELAY_LINE_SIZE-1);
		}
	}
}

void SetDelay(void)
{
}
#endif
