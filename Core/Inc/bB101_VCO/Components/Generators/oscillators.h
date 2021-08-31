/*
 * oscillators.h
 *
 *  Created on: Mar 30, 2021
 *      Author: fil
 */

#ifndef INC_BB100_OSCILLATORS_H_
#define INC_BB100_OSCILLATORS_H_

#define	WAVETABLE_SIZE					256
#define	INT_PRECISION					256
#define	INT_PRECISION_SHIFT				(INT_PRECISION >> 5)
#define	MAX_DETUNE_PERCENT_F			2.0f
#define	SINE							0
#define	TRIANGLE						1
#define	SQUARE							2
#define	NOISE							3
#define	NOISEMODSINE					4
#define	NOISEMODTRIANGLE				5
#define	NOISEMODSQUARE					6

#define	OSC_OFF							0
#define	OSC_GO_OFF						1
#define	OSC_ON							2

#define	INVALID_MIDI_NOTE				0x80

enum ADSR_STATUS
{
	OSC_A_STATE,
	OSC_D_STATE,
	OSC_S_STATE,
	OSC_R_STATE,
	ADSR_CLOSE_STATE
};

#define	APPLY_TUNE				0x80

typedef struct _OscillatorsTypeDef
{
	uint32_t 				oscillator_age;
	uint8_t 				state;
	uint8_t 				midi_note;
	uint8_t 				waveform;
	uint8_t 				adsr_state;
	uint16_t 				adsr_prescaler;
	uint16_t 				adsr_counter;
	float					Alevel;
	float					Dlevel;
	float					Slevel;
	float					Rlevel;
	float					Avalue;
	float					Dvalue;
	float					Astep;
	float					Dstep;
	float					Rstep;
	uint16_t 				current_phase;
	uint16_t 				delta_phase;
	float 					detune;
	uint8_t 				duty;
	float 					volume;
	uint8_t 				noise_weight;
}OscillatorsTypeDef;


extern	OscillatorsTypeDef	Oscillator[NUMOSCILLATORS];
extern	int16_t	wavetab[WAVETABLE_SIZE];
extern	const int16_t	sinetab[256];

extern	uint32_t osc_buffer[HALF_NUMBER_OF_AUDIO_SAMPLES];
extern	uint16_t noise_buffer_gen[HALF_NUMBER_OF_AUDIO_SAMPLES];
extern	uint32_t osc_buffer_gen[HALF_NUMBER_OF_AUDIO_SAMPLES];
extern	uint16_t oscout_buffer[HALF_NUMBER_OF_AUDIO_SAMPLES];
extern	uint16_t pipe0[HALF_NUMBER_OF_AUDIO_SAMPLES];
extern	uint16_t pipe1[HALF_NUMBER_OF_AUDIO_SAMPLES];

extern	void InitOscillators(void);
extern	void InitOscillatorsTables(void);
extern	void DisableOscillator(uint16_t channel, uint16_t midi_note , uint8_t velocity);
extern	void ChangeOscillatorWavePhase(uint16_t	osc_number,	uint8_t waveform , uint16_t phase );
extern	void SetDetune(uint16_t osc_number);
extern	void EnableOscillator(uint16_t channel, uint16_t midi_note , uint8_t velocity);
extern	void DisableAllOscillator(void);
extern	void SetADSR_oscParams(uint32_t osc_number, uint8_t velocity );
extern	void RunOscillator32(void);
extern	void RunOscillator4(void);


#endif /* INC_BB100_OSCILLATORS_H_ */
