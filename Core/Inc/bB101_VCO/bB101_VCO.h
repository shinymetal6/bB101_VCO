/*
 * bB100_VCO.h
 *
 *  Created on: Jun 1, 2021
 *      Author: fil
 */

#ifndef INC_BB101_VCO_BB101_VCO_H_
#define INC_BB101_VCO_BB101_VCO_H_

extern	DAC_HandleTypeDef hdac1;
extern	DMA_HandleTypeDef hdma_dac1_ch2;

extern	I2C_HandleTypeDef hi2c1;

extern	SPI_HandleTypeDef hspi1;

extern	TIM_HandleTypeDef htim6;
extern	TIM_HandleTypeDef htim7;
extern	TIM_HandleTypeDef htim16;

extern	ADC_HandleTypeDef hadc1;
extern	ADC_HandleTypeDef hadc2;

/* System */
#define	SAMPLE_FREQUENCY				44100
#define	SIGNAL_LEN						256
#define	DAC_BIT							12
#define	BIT_RESOLUTION					DAC_BIT
#define	DAC_RESOLUTION					(1<<DAC_BIT)
#define	HALF_DAC_RESOLUTION				(DAC_RESOLUTION/2)
#define	NUMBER_OF_AUDIO_SAMPLES			SIGNAL_LEN
#define	HALF_NUMBER_OF_AUDIO_SAMPLES	(SIGNAL_LEN/2)
#define	NUMOSCILLATORS_BITSHIFT			5 /* means 32 */
#define	NUMOSCILLATORS					(1<<NUMOSCILLATORS_BITSHIFT)
#define	NORMALIZE_CONTROL				4
#define	NORMALIZE_SHIFT					DAC_BIT
#define	SYSTICK_CNTR_MAX				9
#define	SYSTICK_LEDCNTR_HALF			6
#define	SYSTICK_LEDCNTR_FULL			8

#define	AUDIO_DAC						&hdac1
#define	CONTROL_ADC2					&hadc2
#define	CONTROL_ADC1					&hadc1
#define	AUDIO_TIMER						&htim6
#define	CONTROL_TIMER					&htim7
#define	BACKLIGHT_TIMER					htim16
#define	CONTROL_ADC1					&hadc1
#define	CONTROL_ADC2					&hadc2

#define	HAVE_SINGLE_VOICE				1
#define	HAVE_DUAL_VOICE					2
#define	HAVE_QUAD_VOICE					4
#define	VOICES							HAVE_QUAD_VOICE

#define	SYSEX_MAX_LEN						128
#define	USB_BUF_MAX_LEN						256
#define	BB_MACHINE_NAME						"bB101"
#define	BB_MACHINE_VERSION					"1.0"
#define	BB_EE_MACHINE_FAMILY				'1'
#define	BB_EE_MACHINE_MODEL					0x01

typedef struct _SystemParametersTypeDef
{
	char		Header[8];
	char		Version[4];
	uint32_t 	audio_sampling_frequency;
	uint32_t 	base_audio_sampling_frequency;
	float 		max_detune_percent;
	uint8_t 	fill[39];	// fill to 64 bytes for ee write / read
}SystemParametersTypeDef;

typedef struct _SystemFlagsDef
{
	uint16_t 	current_brightness;
	uint8_t 	audio_flags;
	uint8_t 	oscillator_flags;
	uint8_t		osc_waves[4];
	uint8_t 	osc_duty[4];
	uint8_t 	osc_detune[4];
	uint8_t 	osc_volume[4];	// range 0..10
	uint8_t 	osc_duty_percent[4];
	uint8_t 	Atime,Dtime,Sval,Rtime;
	uint8_t 	vcf_flags;
	uint8_t 	effect_flags;
	uint8_t 	delay_flags;
	uint16_t 	delay_value;
	uint8_t 	control_flags;
	uint8_t 	program_number;
	uint8_t 	systick_counter;
	uint8_t 	led_counter;
	uint8_t 	buttons_flag;
	uint8_t 	buttons_debounce;
	uint8_t 	menu_state;
	uint8_t 	menu_line_counter;
	uint8_t		midi_rx_length;
	uint8_t		midi_rx_buffer[USB_BUF_MAX_LEN];
	uint8_t		sysex_buffer[SYSEX_MAX_LEN];
	uint8_t		sysex_len;
	uint16_t 	control_adc1_buf[4];
	uint16_t 	control_adc2_buf[4];
	float 		delay_sample_multiplier,delay_feedback_multiplier;
	uint16_t 	last_tuner_val;
	float 		tuner_delta_multiplier;
	uint16_t 	last_fm_val;
	float 		fm_delta_multiplier;
	float 		oscillator_tuner_constant;
	uint16_t	delay_insertion_pointer;
	uint16_t	delay_extraction_pointer;
	float 		cv_voltage;
	float 		cv_voltage_div_10;
}SystemFlagsDef;

/*
ADSR_TIME_UNIT is 1 / 44100 *128 = 2,902494331 mSec. , rounded to 3 mSec.
 */
#define	ADSR_TIME_UNIT	(1 / SAMPLE_FREQUENCY ) *  HALF_NUMBER_OF_AUDIO_SAMPLES))

/* oscillator_flags values */

#define	OSC_VOLUME_PENDING			0x01
#define	OSC_DETUNE_PENDING			0x02
#define	OSC_DUTY_PENDING			0x04
#define	OSC_WAVE_PENDING			0x08
#define	OSC_FM_PENDING				0x20
#define	OSC_TUNE_PENDING			0x40
#define	OSC_SRCFLAG					0x80	/* 0 is from Cv , 1 is from MIDI */

/* buttons_flags values */
#define	BUTTON_TACT0	1
#define	BUTTON_TACT1	2
#define	BUTTON_TACT2	4
#define	BUTTON_MASK		0x07
#define	BUTTON_EMERGENCY_MASK		0x03

/* control_flags values */
/*
#define	CONTROL_OSC_FROM_CV			0x01
#define	CONTROL_OSC_FROM_MIDI		0x02
*/
#define	CONTROL_OSC_VCF_DLY			0x01
#define	CONTROL_FM					0x04
#define	CONTROL_VCA					0x08
#define	CONTROL_ADC_FLAG			0x10
#define	CONTROL_TICK_FLAG			0x20
#define	CONTROL_SYSTICK_FLAG		0x40
#define	CONTROL_MIDIRX_FLAG			0x80

/* audio_flags values */
#define	AUDIO_VCO_HALF_FLAG			0x01
#define	AUDIO_READY_FLAG			0x80

/* vcf_flags values */
#define	VCF_CONTROL_MASK			0x0F
#define	VCF_CONTROL_POT				0x01
#define	VCF_CONTROL_MIDI			0x02
#define	VCF_CONTROL_CV				0x04
#define	VCF_TYPE_LP					0x10
#define	VCF_TYPE_BP					0x20
#define	VCF_TYPE_HP					0x40
#define	VCF_ENABLED					0x80
#define	VCF_TYPE_MASK				(VCF_TYPE_BP | VCF_TYPE_LP | VCF_TYPE_HP)

/* effect_flags */
#define	EFFECT_MOOG1				0x01
#define	EFFECT_MOOG2				0x02
/* delay_flags values */

/*
#define	DLY_FLANGER_POT				0x00
#define	DLY_REVERB_POT				0x01
#define	DLY_FLANGER_MIDI			0x02
#define	DLY_REVERB_MIDI				0x03
#define	DLY_FLANGER_CV				0x04
#define	DLY_REVERB_CV				0x05
#define	DLY_FLANGER_DIGITAL			0x06
#define	DLY_REVERB_DIGITAL			0x07
*/
#define	DLY_MIXER_FLANGER_POT		0x00
#define	DLY_MIXER_REVERB_POT		0x01
#define	DLY_MIXER_FLANGER_MIDI		0x02
#define	DLY_MIXER_REVERB_MIDI		0x03
#define	DLY_ENABLED					0x80

#define	DLY_MIXER_POT_MASK			0x02
#define	DLY_MIXER_MIDI_MASK			0x02
#define	DLY_REVERB_NFLANGER_BIT		1
#define	DLY_TYPE_MASK				0x0f

#define	DELAY_MAX_VAL				900
#define	DELAY_UNIT					(16383.0F / (float )DELAY_MAX_VAL)

/* Buttons */
#define	TACT0_BUTTON				0x01
#define	TACT1_BUTTON				0x02
#define	TACT2_BUTTON				0x04

/* Machine sequence */
#define	VCO_VCF_DLY					0
#define	VCO_DLY_VCF					1
#define	VCO_ONLY					2
#define	VCO_VCF						3
#define	VCO_DLY						4

/*
#define	J6		SystemFlags.control_adc2_buf[3]
#define	J1		SystemFlags.control_adc1_buf[1]
#define	J3		SystemFlags.control_adc1_buf[2]
#define	J8		SystemFlags.control_adc1_buf[3]
*/
/* J1 */
#define	VCO_AM						SystemFlags.control_adc1_buf[1]
/* J3 */
#define	VCO_CV						SystemFlags.control_adc1_buf[2]
/* J6 */
#define	VCO_FM						SystemFlags.control_adc2_buf[3]

/* RVAR2 */
#define	VCF_CUTOFF_POTCONTROL		SystemFlags.control_adc2_buf[0]
/* RVAR4 */
#define	TUNER_CONTROL				SystemFlags.control_adc2_buf[1]
/* RVAR5 */
#define	VCF_RESONANCE_POTCONTROL	SystemFlags.control_adc2_buf[2]
/* RVAR8 */
#define	DELAY_FEEDBACK_POTCONTROL	SystemFlags.control_adc1_buf[0]

extern uint16_t	signal_out[SIGNAL_LEN];

#include <Generators/oscillators.h>
#include <IntADC/control_adc.h>
#include <Effects/moog_vcf.h>
#include <Effects/delay_line.h>
#include <Effects/vca.h>
#include <Generators/echo.h>
#include <USB_Midi/midi_note2freq.h>
#include <USB_Midi/usb_midi.h>
#include <usbd_midi_if.h>
#include <Lcd/fonts.h>
#include <Lcd/Lcd.h>
#include <Lcd/st7735.h>
#include <EEprom/ee24_driver.h>
#include <Buttons/pushbutton.h>
#include <menus.h>
#include <adsr_display.h>

#include <string.h>
#include "math.h"

/* Back declarations for forward files ... */
extern	SystemParametersTypeDef	SystemParameters;
extern	SystemFlagsDef			SystemFlags;

extern	void get_limits(uint16_t *start,uint16_t *end);
extern	uint32_t get_bufferhalf(uint32_t channel);
extern	void bB101_Vco_Init(void);
extern	void InitialParameters(void);
extern	void bB101_Vco_AudioLoop(void);
extern	void bB101_Print_Tuner(void);
extern	void bB101_Vco_ControlLoop(void);

extern	void SysTimer_callback(void);
extern	void ADC_callback(void);

extern	void store_system_parameters(void);
extern	void InitFromFlash(void);

extern	const uint16_t logo[12800] ;

#define pi           3.14159265358979323846
#define	INT_2_NORMALIZED_FLOAT(x)	((float )(x) - 2048.0F) / 2048.0F
#define	FLOAT_2_NORMALIZED_INT(x)	((float )(x) * 2048.0F) + 2048.0F




#endif /* INC_BB101_VCO_BB101_VCO_H_ */
