/*
 * delay_line.c
 *
 *  Created on: Jul 1, 2021
 *      Author: fil
 */

#include "main.h"

__attribute__ ((aligned (16))) uint8_t	delay_line[DELAY_LINE_SIZE];

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
