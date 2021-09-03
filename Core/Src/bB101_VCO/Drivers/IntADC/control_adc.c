/*
 * control_adc.c
 *
 *  Created on: Jun 29, 2021
 *      Author: fil
 */

#include "main.h"

static void draw_led(void)
{
	SystemFlags.led_counter++;
	if ( SystemFlags.led_counter > SYSTICK_LEDCNTR_HALF )
	{
		ST7735_FillRectangle(1, 1, 4, 4, ST7735_RED);
		ST7735_DrawPixel(1, 1, ST7735_BLACK);
		ST7735_DrawPixel(4, 1, ST7735_BLACK);
		ST7735_DrawPixel(1, 4, ST7735_BLACK);
		ST7735_DrawPixel(4, 4, ST7735_BLACK);
	}
	if ( SystemFlags.led_counter >= SYSTICK_LEDCNTR_FULL )
	{
		SystemFlags.led_counter = 0;
		ST7735_FillRectangle(1, 1, 4, 4, ST7735_BLACK);
	}
}

void bB101_Vco_ControlLoop(void)
{
	if (( SystemFlags.control_flags & CONTROL_TICK_FLAG ) == CONTROL_TICK_FLAG)
	{
		if (( SystemFlags.control_flags & CONTROL_SYSTICK_FLAG ) == CONTROL_SYSTICK_FLAG)
			SystemFlags.control_flags &= ~CONTROL_SYSTICK_FLAG;
		SystemFlags.control_flags &= ~CONTROL_TICK_FLAG;
		SystemFlags.systick_counter++;
		if ( SystemFlags.systick_counter > SYSTICK_CNTR_MAX )
		{
			SystemFlags.systick_counter = 0;
			SystemFlags.control_flags |= CONTROL_SYSTICK_FLAG;
			bB101_Print_Tuner();
			poll_pushbtn();
			draw_led();
			if (( SystemFlags.control_flags & CONTROL_ROLLBACK2ADSR ) == CONTROL_ROLLBACK2ADSR)
			{
				if (( SystemFlags.rollback_flags & ROLL_VOLUME ) == ROLL_VOLUME)
				{
					DisplayVolume();
					SystemFlags.rollback_flags &= ~ROLL_VOLUME;
				}
				if (( SystemFlags.rollback_flags & ROLL_DUTY ) == ROLL_DUTY)
				{
					DisplayDuty();
					SystemFlags.rollback_flags &= ~ROLL_DUTY;
				}
				if (( SystemFlags.rollback_flags & ROLL_DETUNE ) == ROLL_DETUNE)
				{
					DisplayDetune();
					SystemFlags.rollback_flags &= ~ROLL_DETUNE;
				}

				if ( SystemFlags.tonormaldisplay_counter != 0 )
					SystemFlags.tonormaldisplay_counter--;
				if ( SystemFlags.tonormaldisplay_counter == 0 )
				{
					DisplayADSR();
					SystemFlags.control_flags &= ~CONTROL_ROLLBACK2ADSR;
				}
			}
		}
	}

	if (( SystemFlags.control_flags & CONTROL_ADC_FLAG ) == CONTROL_ADC_FLAG)
	{
		if (( SystemFlags.delay_flags & DLY_MIXER_POT_MASK) == 0 )
		{
			SystemFlags.delay_sample_multiplier = (float )DELAY_FEEDBACK_POTCONTROL/4096.0F;
			SystemFlags.delay_feedback_multiplier = 1.0-SystemFlags.delay_sample_multiplier;
		}

		if (( SystemFlags.last_tuner_val != (TUNER_CONTROL & 0xffc0 )) || (( SystemFlags.oscillator_flags & OSC_FM_PENDING) == OSC_FM_PENDING))
		{
			if (( SystemFlags.control_flags & CONTROL_FM) == CONTROL_FM)
			{
				SystemFlags.tuner_delta_multiplier = ((float )VCO_FM - 2048.0F )/ 2048.0F;
				SystemFlags.oscillator_tuner_constant = (SystemFlags.cv_voltage + SystemFlags.cv_voltage_div_10 * (float )VCO_FM / 2048.0F ) ;
			}
			else
			{
				SystemFlags.tuner_delta_multiplier = ((float )TUNER_CONTROL - 2048.0F )/ 204.8F;
				SystemFlags.oscillator_tuner_constant = (SystemFlags.cv_voltage + SystemFlags.cv_voltage_div_10 * (float )TUNER_CONTROL / 2048.0F ) ;
			}

			SystemFlags.last_tuner_val = TUNER_CONTROL & 0xffc0;
			SystemFlags.oscillator_flags |= OSC_TUNE_PENDING;
			SystemFlags.oscillator_flags &= ~OSC_FM_PENDING;

		}

		if (( SystemFlags.control_flags & CONTROL_FM) == CONTROL_FM)
		{
			if ( SystemFlags.last_fm_val != ((VCO_FM >> 4 ) << 4) )
			{
				SystemFlags.oscillator_flags |= OSC_FM_PENDING;
				SystemFlags.last_fm_val = (VCO_FM >> 4 ) << 4;
			}
		}

		if (( SystemFlags.afx_flags & AFX_ENABLED) == AFX_ENABLED )
		{
			if (( SystemFlags.afx_flags & AFX_CONTROL_MASK ) == AFX_CONTROL_POT)
			{
				switch(SystemFlags.afxtype_flags)
				{
				case	AFXTYPE_MOOG1	:
					AFXParameters.filterCutoff  	= (float )AFX_CUTOFF_POTCONTROL    / 4096.0F;
					AFXParameters.filterResonance 	= (float )AFX_RESONANCE_POTCONTROL / 4096.0F;
					Set_Filter1_Coefficients();
					break;
				case	AFXTYPE_MOOG2	:
					AFXParameters.filterCutoff  	= (float )AFX_CUTOFF_POTCONTROL    / 4096.0F;
					AFXParameters.filterResonance 	= (float )AFX_RESONANCE_POTCONTROL / 4096.0F;
					Set_Filter2_Coefficients();
					break;
				case	AFXTYPE_PHASER	:
					Phaser_Rate_set(AFX_CUTOFF_POTCONTROL >> 5);
					Phaser_Feedback_set((float )AFX_RESONANCE_POTCONTROL / 47.0F);
					break;
				default	:	break;
				}
			}
			if (( SystemFlags.afx_flags & AFX_CONTROL_MASK ) == AFX_CONTROL_MIDI)
			{
				switch(SystemFlags.afxtype_flags)
				{
				case	AFXTYPE_MOOG1	:
					Set_Filter1_Coefficients();
					break;
				case	AFXTYPE_MOOG2	:
					Set_Filter2_Coefficients();
					break;
				case	AFXTYPE_PHASER	:
					Phaser_Rate_set(AFXParameters.phaser_rate);
					Phaser_Feedback_set(AFXParameters.phaser_feedback);
					break;
				default	:	break;
				}
			}
		}

		if (( SystemFlags.oscillator_flags & OSC_SRCFLAG ) != OSC_SRCFLAG)
		{
	        float voltage = (float )VCO_CV / 409.6F;
	        float vpow = powf(2.0F , voltage);
	        SystemFlags.cv_voltage = (110.0F / 6.727171F * vpow);
	        SystemFlags.cv_voltage_div_10 = SystemFlags.cv_voltage/10.0F;
		}

		SystemFlags.control_flags &= ~CONTROL_ADC_FLAG;
	}
}

