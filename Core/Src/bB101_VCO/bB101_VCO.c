/*
 * bB100_Swarm.c
 *
 *  Created on: Jun 1, 2021
 *      Author: fil
 */

#include "main.h"

__attribute__ ((aligned (4)))	uint16_t	signal_out[SIGNAL_LEN];
__attribute__ ((aligned (4)))	uint16_t	pipe0[SIGNAL_LEN];
__attribute__ ((aligned (4)))	uint16_t	pipe1[SIGNAL_LEN];
__attribute__ ((aligned (16)))	uint32_t	osc_buffer[NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (16)))	uint16_t	oscout_buffer[NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (4)))	SystemParametersTypeDef	SystemParameters;
__attribute__ ((aligned (4)))	SystemFlagsDef			SystemFlags;

void store_system_parameters(void)
{
	ee24_write(0, (uint8_t *)&SystemParameters, sizeof(SystemParameters), 100);
}

void initialize_flash(void)
{
	sprintf(SystemParameters.Header,BB_MACHINE_NAME);
	sprintf(SystemParameters.Version,BB_MACHINE_VERSION);
	SystemParameters.audio_sampling_frequency = SystemParameters.base_audio_sampling_frequency = SAMPLE_FREQUENCY;
	SystemParameters.max_detune_percent = MAX_DETUNE_PERCENT_F;
	store_system_parameters();
}

//#define	CLEAR_FLASH	1
void InitFromFlash(void)
{
	ee24_read(0, (uint8_t *)&SystemParameters, sizeof(SystemParameters), 100);
#ifndef CLEAR_FLASH
	if ( strncmp(BB_MACHINE_NAME,SystemParameters.Header,sizeof(SystemParameters.Header)) != 0 )
#endif
		initialize_flash();
}

void InitialParameters(void)
{
	SystemFlags.oscillator_flags |= OSC_SRCFLAG;
	SystemFlags.control_flags = CONTROL_OSC_VCF_DLY;
	SystemFlags.vcf_flags = VCF_CONTROL_POT | VCF_TYPE_LP;
	SystemFlags.effect_flags |= EFFECT_MOOG1;
	SystemFlags.delay_flags = DLY_MIXER_FLANGER_POT;
	SystemFlags.delay_flags |= DLY_ENABLED;
	SystemFlags.osc_waves[0] = SINE;
	SystemFlags.osc_waves[1] = SINE;
	SystemFlags.osc_waves[2] = SINE;
	SystemFlags.osc_waves[3] = SINE;
	SystemFlags.osc_duty_percent[0] = 50;
	SystemFlags.osc_duty_percent[1] = 50;
	SystemFlags.osc_duty_percent[2] = 50;
	SystemFlags.osc_duty_percent[3] = 50;
	SystemFlags.osc_duty[0] = (uint8_t )((float )SystemFlags.osc_duty_percent[0] * 2.55F );
	SystemFlags.osc_duty[1] = (uint8_t )((float )SystemFlags.osc_duty_percent[1] * 2.55F );
	SystemFlags.osc_duty[2] = (uint8_t )((float )SystemFlags.osc_duty_percent[2] * 2.55F );
	SystemFlags.osc_duty[3] = (uint8_t )((float )SystemFlags.osc_duty_percent[3] * 2.55F );
	SystemFlags.osc_volume[0] = 10;
	SystemFlags.osc_volume[1] = 10;
	SystemFlags.osc_volume[2] = 10;
	SystemFlags.osc_volume[3] = 10;
	SystemFlags.osc_detune[0] = 50;
	SystemFlags.osc_detune[1] = 50;
	SystemFlags.osc_detune[2] = 50;
	SystemFlags.osc_detune[3] = 50;
	SystemFlags.Atime = 20;
	SystemFlags.Dtime = 40;
	SystemFlags.Rtime = 60;
	SystemFlags.Sval  = 20;

	SystemFlags.menu_state = MENU_STATE_TOP;
	SystemFlags.menu_line_counter = 1;
}

void animation_in(void)
{
uint16_t brightness=0;

	ST7735_DrawImage(0,0,ST7735_WIDTH, ST7735_HEIGHT,logo);
	BACKLIGHT_TIMER.Instance->CCR1 = brightness;
	while( brightness != FULL_BRIGHTNESS )
	{
		brightness++;
		BACKLIGHT_TIMER.Instance->CCR1 = brightness;
		HAL_Delay(5);
	}
	while( brightness != 0 )
	{
		brightness--;
		BACKLIGHT_TIMER.Instance->CCR1 = brightness;
		HAL_Delay(1);
	}
	return;
}
void animation_out(void)
{
uint16_t brightness=0;
	while( brightness != FULL_BRIGHTNESS )
	{
		brightness++;
		BACKLIGHT_TIMER.Instance->CCR1 = brightness;
		HAL_Delay(1);
	}
	bB101_Vco_ControlLoop();
	brightness = FULL_BRIGHTNESS;
	BACKLIGHT_TIMER.Instance->CCR1 = brightness;
}

#define	SKIP_ANIMATION	1
void bB101_Vco_Init(void)
{
	InitFromFlash();
	InitialParameters();
	LcdInit();
#ifndef	SKIP_ANIMATION
	animation_in();
#else
	ST7735_FillScreen(ST7735_BLACK);
	BACKLIGHT_TIMER.Instance->CCR1 = FULL_BRIGHTNESS;
#endif
	SystemFlags.systick_counter = 0;

	SystemFlags.delay_value = 0;
	HAL_ADC_Start_DMA(CONTROL_ADC1, (uint32_t *)&SystemFlags.control_adc1_buf[0], 4);
	HAL_ADC_Start_DMA(CONTROL_ADC2, (uint32_t *)&SystemFlags.control_adc2_buf[0], 4);
	HAL_TIM_Base_Start_IT(CONTROL_TIMER);

	Main_Menu();
	MenusDrawEmptyStatus();
	MenusDrawStatus();
#ifndef	SKIP_ANIMATION
	animation_out();
#endif

	InitOscillators();

	HAL_DAC_Start_DMA(AUDIO_DAC, DAC_CHANNEL_1, (uint32_t* )signal_out, SIGNAL_LEN, DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start(AUDIO_TIMER);
}

void bB101_Vco_AudioLoop(void)
{
uint8_t	i;
	if (( SystemFlags.audio_flags & AUDIO_READY_FLAG ) == AUDIO_READY_FLAG)
		SystemFlags.audio_flags &= ~AUDIO_READY_FLAG;
	if (( SystemFlags.oscillator_flags & OSC_WAVE_PENDING ) != 0)
	{
		for(i=0;i<4;i++)
		{
			switch ( SystemFlags.osc_waves[i]  )
			{
			case	SINE	:	Draw_Waveform(i,SINE); break;
			case	TRIANGLE:	Draw_Waveform(i,TRIANGLE); break;
			case	SQUARE	:	Draw_Waveform(i,SQUARE); break;
			default			:	Draw_Waveform(i,SINE); break;
			}
		}
		SystemFlags.oscillator_flags &= ~OSC_WAVE_PENDING;
	}
	if (( SystemFlags.oscillator_flags & OSC_DUTY_PENDING ) != 0)
	{
		DisplayDuty();
		SystemFlags.oscillator_flags &= ~OSC_DUTY_PENDING;
	}
	if (( SystemFlags.oscillator_flags & OSC_DETUNE_PENDING ) != 0)
	{
		DisplayDetune();
		SystemFlags.oscillator_flags &= ~OSC_DETUNE_PENDING;
	}
	if (( SystemFlags.oscillator_flags & OSC_VOLUME_PENDING ) != 0)
	{
		SystemFlags.oscillator_flags &= ~OSC_VOLUME_PENDING;
	}
}

