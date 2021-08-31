/*
 * oscillator_core.c
 *
 *  Created on: Jun 11, 2021
 *      Author: fil
 */

#include "main.h"
__attribute__((section(".table"))) const int16_t        sinetab[WAVETABLE_SIZE] = {
		0x0,0x1,0x2,0x6,0xa,0xf,0x16,0x1e,
		0x27,0x32,0x3d,0x4a,0x58,0x67,0x78,0x89,
		0x9c,0xb0,0xc5,0xdb,0xf2,0x10a,0x123,0x13e,
		0x159,0x175,0x193,0x1b1,0x1d1,0x1f1,0x212,0x235,
		0x258,0x27c,0x2a0,0x2c6,0x2ed,0x314,0x33c,0x365,
		0x38e,0x3b8,0x3e3,0x40e,0x43a,0x467,0x494,0x4c2,
		0x4f0,0x51f,0x54e,0x57d,0x5ad,0x5dd,0x60e,0x63f,
		0x670,0x6a1,0x6d3,0x705,0x737,0x769,0x79b,0x7cd,
		0x800,0x832,0x864,0x896,0x8c8,0x8fa,0x92c,0x95e,
		0x98f,0x9c0,0x9f1,0xa22,0xa52,0xa82,0xab1,0xae0,
		0xb0f,0xb3d,0xb6b,0xb98,0xbc5,0xbf1,0xc1c,0xc47,
		0xc71,0xc9a,0xcc3,0xceb,0xd12,0xd39,0xd5f,0xd83,
		0xda7,0xdca,0xded,0xe0e,0xe2e,0xe4e,0xe6c,0xe8a,
		0xea6,0xec1,0xedc,0xef5,0xf0d,0xf24,0xf3a,0xf4f,
		0xf63,0xf76,0xf87,0xf98,0xfa7,0xfb5,0xfc2,0xfcd,
		0xfd8,0xfe1,0xfe9,0xff0,0xff5,0xff9,0xffd,0xffe,
		0xfff,0xffe,0xffd,0xff9,0xff5,0xff0,0xfe9,0xfe1,
		0xfd8,0xfcd,0xfc2,0xfb5,0xfa7,0xf98,0xf87,0xf76,
		0xf63,0xf4f,0xf3a,0xf24,0xf0d,0xef5,0xedc,0xec1,
		0xea6,0xe8a,0xe6c,0xe4e,0xe2e,0xe0e,0xded,0xdca,
		0xda7,0xd83,0xd5f,0xd39,0xd12,0xceb,0xcc3,0xc9a,
		0xc71,0xc47,0xc1c,0xbf1,0xbc5,0xb98,0xb6b,0xb3d,
		0xb0f,0xae0,0xab1,0xa82,0xa52,0xa22,0x9f1,0x9c0,
		0x98f,0x95e,0x92c,0x8fa,0x8c8,0x896,0x864,0x832,
		0x800,0x7cd,0x79b,0x769,0x737,0x705,0x6d3,0x6a1,
		0x670,0x63f,0x60e,0x5dd,0x5ad,0x57d,0x54e,0x51f,
		0x4f0,0x4c2,0x494,0x467,0x43a,0x40e,0x3e3,0x3b8,
		0x38e,0x365,0x33c,0x314,0x2ed,0x2c6,0x2a0,0x27c,
		0x258,0x235,0x212,0x1f1,0x1d1,0x1b1,0x193,0x175,
		0x159,0x13e,0x123,0x10a,0xf2,0xdb,0xc5,0xb0,
		0x9c,0x89,0x78,0x67,0x58,0x4a,0x3d,0x32,
		0x27,0x1e,0x16,0xf,0xa,0x6,0x2,0x1,
};

__attribute__ ((aligned (4)))	uint16_t	pipe0[HALF_NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (4)))	uint16_t	pipe1[HALF_NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (16)))	uint32_t	osc_buffer[HALF_NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (16)))	uint16_t	oscout_buffer[HALF_NUMBER_OF_AUDIO_SAMPLES];

uint16_t        tritab[VOICES][WAVETABLE_SIZE];
uint16_t        squareval[VOICES];

#define	ADSR_CNTR_CONSTANT	1

void SetADSR_oscParams(uint32_t osc_number, uint8_t velocity )
{
	Oscillator[osc_number].a_level = 0;
	Oscillator[osc_number].d_level = 0xfff;
	Oscillator[osc_number].s_level = 0xfff;
	Oscillator[osc_number].r_level = 0xfff;

	Oscillator[osc_number].a_value = (velocity << 5) | 0x1f;
	Oscillator[osc_number].d_value = SystemFlags.Sval * (Oscillator[osc_number].a_value/32);

	Oscillator[osc_number].adsr_state = OSC_A_STATE;
	Oscillator[osc_number].adsr_prescaler = ADSR_CNTR_CONSTANT;
	Oscillator[osc_number].adsr_counter = 0;

	Oscillator[osc_number].a_step = Oscillator[osc_number].a_value / SystemFlags.Atime;
	Oscillator[osc_number].d_step = Oscillator[osc_number].d_value / SystemFlags.Dtime;
	Oscillator[osc_number].r_step = Oscillator[osc_number].s_level / SystemFlags.Rtime;

	if ( Oscillator[osc_number].a_step == 0 )
		Oscillator[osc_number].a_step = 1;
	if ( Oscillator[osc_number].d_step == 0 )
		Oscillator[osc_number].d_step = 1;
	if ( Oscillator[osc_number].r_step == 0 )
		Oscillator[osc_number].r_step = 1;
}

static uint16_t i_adsr(uint32_t osc_number, uint8_t	angle )
{
uint16_t ret_val = 0;	//Oscillator[osc_number].midi_note

	if ( Oscillator[osc_number].state != OSC_OFF )
	{
		Oscillator[osc_number].oscillator_age++;
		{
			switch(Oscillator[osc_number].adsr_state)
			{
			case	OSC_A_STATE:
				Oscillator[osc_number].a_level +=  Oscillator[osc_number].a_step;
				if ( Oscillator[osc_number].a_level >= Oscillator[osc_number].a_value )
				{
					Oscillator[osc_number].d_level = Oscillator[osc_number].a_level;
					Oscillator[osc_number].adsr_state = OSC_D_STATE;
				}
				ret_val = Oscillator[osc_number].a_level;
				break;
			case	OSC_D_STATE:
				Oscillator[osc_number].d_level -=  Oscillator[osc_number].d_step;
				if ( Oscillator[osc_number].d_level <= Oscillator[osc_number].d_value )
				{
					Oscillator[osc_number].s_level = Oscillator[osc_number].d_level;
					Oscillator[osc_number].adsr_state = OSC_S_STATE;
				}
				ret_val = Oscillator[osc_number].d_level;
				break;
			case	OSC_S_STATE:
				if ( Oscillator[osc_number].state == OSC_GO_OFF)
				{
					Oscillator[osc_number].adsr_state = OSC_R_STATE;
					Oscillator[osc_number].r_level = Oscillator[osc_number].s_level;
				}
				ret_val = Oscillator[osc_number].s_level;
				break;
			case	OSC_R_STATE:
				Oscillator[osc_number].r_level -=  Oscillator[osc_number].r_step;
				if ( Oscillator[osc_number].r_level <= Oscillator[osc_number].r_step )
					Oscillator[osc_number].adsr_state = ADSR_CLOSE_STATE;
				ret_val = Oscillator[osc_number].r_level;
				break;
			case	ADSR_CLOSE_STATE:
				if ( angle > 224 )
				{
					Oscillator[osc_number].state = OSC_OFF;
					Oscillator[osc_number].current_volume = 0;
					Oscillator[osc_number].midi_note = INVALID_MIDI_NOTE;
					Oscillator[osc_number].oscillator_age = 0;
				}
				ret_val = 0;
				break;
			}
		}
	}
	return (ret_val * Oscillator[osc_number].current_volume ) >> NORMALIZE_SHIFT;
}

void RunOscillator32(void)
{
uint16_t	i;
uint8_t		angle,osc_number,osc_active = 0 , shft_num = 0;
uint16_t	adsr_value;

	for ( i=0;i<HALF_NUMBER_OF_AUDIO_SAMPLES;i++)
	{
		osc_buffer[i] = 0;
		for(osc_number=0;osc_number<NUMOSCILLATORS;osc_number++)
		{
			if ( Oscillator[osc_number].state != OSC_OFF )
			{
				angle = (uint8_t )(Oscillator[osc_number].current_phase >> 8);
				adsr_value = i_adsr(osc_number,angle );

				if ( Oscillator[osc_number].waveform == SINE )
				{
					//osc_buffer[i] += (uint32_t )((float )(sinetab[angle] * adsr_value )* Oscillator[osc_number].volume);
					osc_buffer[i] += (sinetab[angle] * adsr_value );
				}
				if ( Oscillator[osc_number].waveform == TRIANGLE )
				{
					//osc_buffer[i] += (uint32_t )((float )(tritab[osc_number & 0x03][angle] * adsr_value )* Oscillator[osc_number].volume);
					osc_buffer[i] += (tritab[osc_number & 0x03][angle] * adsr_value );
				}
				if ( Oscillator[osc_number].waveform == SQUARE )
				{
					if ( angle > squareval[osc_number & 0x03])
						osc_buffer[i] += ((DAC_RESOLUTION-1) * adsr_value );
					//osc_buffer[i] += (uint32_t )((float )((DAC_RESOLUTION-1) * adsr_value )* Oscillator[osc_number].volume);
				}
				osc_buffer[i] = (uint32_t )((float )osc_buffer[i] * Oscillator[osc_number].volume);

				Oscillator[osc_number].current_phase += Oscillator[osc_number].delta_phase;
				if ((( SystemFlags.oscillator_flags & OSC_TUNE_PENDING ) == OSC_TUNE_PENDING) && ( Oscillator[osc_number].midi_note != INVALID_MIDI_NOTE))
				{
					float	delta_phase;
					float	freq;
					float 	deltaPre;
					float 	delta=0;

					deltaPre = midi_freq[Oscillator[osc_number].midi_note+1] - midi_freq[Oscillator[osc_number].midi_note];
					delta = SystemFlags.tuner_delta_multiplier * deltaPre;
					freq = midi_freq[Oscillator[osc_number].midi_note] + Oscillator[osc_number].detune + delta;
					delta_phase = (float )WAVETABLE_SIZE / ((float )SystemParameters.audio_sampling_frequency / freq);
					Oscillator[osc_number].current_phase += (uint16_t )(delta_phase * (float )INT_PRECISION);
				}
			}
		}
	}
	for(osc_number=0;osc_number<NUMOSCILLATORS;osc_number++)
	{
		if ( Oscillator[osc_number].state != OSC_OFF )
			osc_active++;
	}
	SystemFlags.control_flags &= ~OSC_TUNE_PENDING;
	shft_num = DAC_BIT + 1 + (uint8_t )(sqrtf((float )osc_active));
	for ( i=0;i<HALF_NUMBER_OF_AUDIO_SAMPLES;i++)
		//oscout_buffer[i] = HALF_DAC_RESOLUTION + ((osc_buffer[i] >> shft_num) & (DAC_RESOLUTION-1));
		oscout_buffer[i] = HALF_DAC_RESOLUTION + ((osc_buffer[i] >> 18) & (DAC_RESOLUTION-1));
}

void RunOscillator4(void)
{
float	delta_phase;
uint16_t	i;
uint8_t		angle,osc_number;

	for ( i=0;i<HALF_NUMBER_OF_AUDIO_SAMPLES;i++)
	{
		osc_buffer[i] = 0;
		for(osc_number=0;osc_number<VOICES;osc_number++)
		{
			angle = (uint8_t )(Oscillator[osc_number].current_phase >> 8);
			switch(Oscillator[osc_number].waveform)
			{
			case	SINE	:	osc_buffer[i] += (uint16_t )((float )sinetab[angle] * Oscillator[osc_number].volume);break;
			case 	TRIANGLE:	osc_buffer[i] += (uint16_t )((float )tritab[osc_number & 0x03][angle] * Oscillator[osc_number].volume);break;
			case 	SQUARE	:
				if ( angle > squareval[osc_number & 0x03])
					osc_buffer[i] += (uint16_t )((float )(DAC_RESOLUTION-1) * Oscillator[osc_number].volume);
				break;
			default			:	osc_buffer[i] += (uint16_t )((float )sinetab[angle] * Oscillator[osc_number].volume);break;	// SINE is default
			}
			Oscillator[osc_number].current_phase += Oscillator[osc_number].delta_phase;

			if ( VCO_CV != 0 )
			{
				delta_phase = (float )WAVETABLE_SIZE / ((float )SystemParameters.audio_sampling_frequency / ( SystemFlags.oscillator_tuner_constant + Oscillator[osc_number].detune));
				Oscillator[osc_number].delta_phase = (uint16_t )(delta_phase * (float )INT_PRECISION);
			}
			else
				Oscillator[osc_number].current_phase = 0;
		}
		oscout_buffer[i] = osc_buffer[i] >> 2;
	}
	return;
}

/* phase range 0..255 for square and tri */
static void squareCompile(uint16_t	osc_number , uint32_t phase)
{
uint16_t	k;

	k = osc_number;
	k &= (VOICES-1);
	squareval[k] = phase;
	SystemFlags.osc_duty[k] = phase;
}

static void triCompile(uint16_t	osc_number , uint32_t phase)
{
float 	step_up,step_down,current;
uint16_t	i , k;

	k = osc_number;
	k &= (VOICES-1);

	step_up = (float )(DAC_RESOLUTION-1) / (float )phase;
	step_down = (float )(DAC_RESOLUTION-1)  / (float )( WAVETABLE_SIZE - phase);
	current=0;
	for(i=0;i<WAVETABLE_SIZE;i++)
	{
		if ( i < phase )
		{
			tritab[k][i] = (uint32_t )current;
			current += step_up;
		}
		else
		{
			tritab[k][i] = (uint32_t )current;
			current -= step_down;
		}
	}
	SystemFlags.osc_duty[k] = phase;
}

void ChangeOscillatorWavePhase(uint16_t	osc_number ,	uint8_t waveform , uint16_t phase )
{
	switch (waveform)
	{
	case	TRIANGLE:	triCompile(osc_number,phase);break;
	case	SQUARE	:	squareCompile(osc_number,phase);break;
	default : break;
	}
}

void InitOscillatorsTables(void)
{
uint16_t	i;
	for(i=0;i<VOICES;i++)
	{
		squareCompile(i , 127);
		triCompile(i , 127);
	}
}

