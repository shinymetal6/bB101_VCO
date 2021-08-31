/*
 * usb_midi.c
 *
 *  Created on: Dec 23, 2020
 *      Author: fil
 */

#include "main.h"
#include "math.h"
#include <stdio.h>
#include <string.h>

__attribute__ ((aligned (4)))	uint8_t		midi_rx_buf[MIDI_RXBUF_SIZE];
__attribute__ ((aligned (4)))	uint8_t		midi_tx_buf[MIDI_RXBUF_SIZE];
//__attribute__ ((aligned (4)))	uint8_t		pgm_copy[PROGRAM_SIZE*2];

uint16_t SYSEX_ConvertMidiBufOut(uint8_t *buf_out , uint16_t len)
{
uint32_t	r=0,k=0;

	midi_tx_buf[k] = SYSEX_STARTC;
	k++;
	midi_tx_buf[k] = SYSEX_START;
	k++;
	r = 0;
	while ( r < len )
	{
		midi_tx_buf[k] = buf_out[r];
		r++;
		k++;
		if ( (k & 0x03 ) == 0)
		{
			if (( len - r ) == 2)
				midi_tx_buf[k] = SYSEX_END_3;
			else if (( len - r ) == 1)
				midi_tx_buf[k] = SYSEX_END_2;
			else if (( len - r ) == 0)
				midi_tx_buf[k] = SYSEX_END_1;
			else
				midi_tx_buf[k] = SYSEX_STARTC;
			k++;
		}
	}
	midi_tx_buf[k] = SYSEX_END;
	while ((k & 0x03 ) != 0 )
		k++;
	return k;
}

//#define		TEST_MIDI_OUT	1
void DumpProgram(uint8_t program_number)
{
uint16_t	i;
uint32_t	Len;

	for(i=0;i<MIDI_PROGRAM_SIZE;i++)
		midi_tx_buf[i] = 0;

#ifdef TEST_MIDI_OUT
	midi_tx_buf[0] = SYSEX_STARTC;
	midi_tx_buf[1] = SYSEX_START;
	midi_tx_buf[2] = 'a';
	midi_tx_buf[3] = 'b';
	midi_tx_buf[4] = SYSEX_STARTC;
	midi_tx_buf[5] = 'c';
	midi_tx_buf[6] = 'd';
	midi_tx_buf[7] = 'e';
	midi_tx_buf[8] = SYSEX_STARTC;
	midi_tx_buf[9] = 'f';
	midi_tx_buf[10] = 'g';
	midi_tx_buf[11] = 'h';
	midi_tx_buf[12] = SYSEX_STARTC;
	midi_tx_buf[13] = 'i';
	midi_tx_buf[14] = 'j';
	midi_tx_buf[15] = 'k';
	midi_tx_buf[16] = SYSEX_STARTC;
	midi_tx_buf[17] = 'l';
	midi_tx_buf[18] = 'm';
	midi_tx_buf[19] = 'n';
	midi_tx_buf[20] = SYSEX_STARTC;
	midi_tx_buf[21] = 'o';
	midi_tx_buf[22] = 'p';
	midi_tx_buf[23] = 'q';
	midi_tx_buf[24] = SYSEX_STARTC;
	midi_tx_buf[25] = 'r';
	midi_tx_buf[26] = 's';
	midi_tx_buf[27] = 't';
	midi_tx_buf[28] = SYSEX_STARTC;
	midi_tx_buf[29] = 'u';
	midi_tx_buf[30] = 'v';
	midi_tx_buf[31] = 'w';
	midi_tx_buf[32] = SYSEX_STARTC;
	midi_tx_buf[33] = 'x';
	midi_tx_buf[34] = 'y';
	midi_tx_buf[35] = 'z';
	midi_tx_buf[36] = SYSEX_STARTC;
	midi_tx_buf[37] = '0';
	midi_tx_buf[38] = '1';
	midi_tx_buf[39] = '2';
	midi_tx_buf[40] = SYSEX_END_3;
	midi_tx_buf[41] = '3';
	midi_tx_buf[42] = '4';
	midi_tx_buf[43] = SYSEX_END;
	Len = MIDI_PROGRAM_SIZE;
#else
	ee24_read_program(program_number);
	Len = SYSEX_ConvertMidiBufOut((uint8_t *)&Program,sizeof(Program));
#endif

	MIDI_Transmit_FS(midi_tx_buf,  Len);
}

void DumpCurrentProgram(void)
{
uint16_t	i;
uint32_t	Len;

	for(i=0;i<MIDI_PROGRAM_SIZE;i++)
		midi_tx_buf[i] = 0;
	ee24_get_current_program();
	Len = SYSEX_ConvertMidiBufOut((uint8_t *)&CurrentProgram,sizeof(Program));
	MIDI_Transmit_FS(midi_tx_buf,  Len);
}

uint8_t UsbMidiParseNoteOFF(uint8_t channel ,uint8_t midi_note , uint8_t velocity)
{
	if (( SystemFlags.oscillator_flags & OSC_SRCFLAG ) == OSC_SRCFLAG)
		DisableOscillator(channel,midi_note,velocity);
	return 4;
}

uint8_t UsbMidiParseNoteON(uint8_t	channel , uint8_t midi_note , uint8_t velocity)
{
	if (( SystemFlags.oscillator_flags & OSC_SRCFLAG ) == OSC_SRCFLAG)
	{
		if ( velocity == 0 )
			DisableOscillator(channel,midi_note,velocity);
		else
			EnableOscillator(channel,midi_note,velocity);
	}
	return 4;
}

void UsbSysExApplyValues(void)
{
	if ( SystemFlags.sysex_buffer[1] == SYS_COMMAND_CMD)
	{
		if (( SystemFlags.sysex_buffer[2] == SYSEX_RESET_CMD ) && ( SystemFlags.sysex_buffer[3] == SYSEX_RESET_CMD ))
			EmergencyReset();
		if ( SystemFlags.sysex_buffer[2] == SYSEX_DUMP_PROGRAM_CMD )
			DumpProgram(SystemFlags.sysex_buffer[3] & (NUM_PROGRAMS_MAX-1));
		if ( SystemFlags.sysex_buffer[2] == SYSEX_DUMP_CURRENT_PROGRAM_CMD )
			DumpCurrentProgram();
	}
}

uint8_t UsbMidiParseSysEx(uint8_t	channel , uint8_t sub_command,uint32_t len)
{
uint8_t	i,j=1,k=0;

	if ( len > SYSEX_MAX_LEN )
		return 0;
	if ( SystemFlags.midi_rx_buffer[1] != SYSEX_START )
		return 0;
	SystemFlags.sysex_len = 0;
	for(i=0;i<len;i+=4)
	{
		if ( SystemFlags.midi_rx_buffer[i] == SYSEX_STARTC )
		{
			SystemFlags.sysex_buffer[k] = SystemFlags.midi_rx_buffer[j];
			SystemFlags.sysex_buffer[k+1] = SystemFlags.midi_rx_buffer[j+1];
			SystemFlags.sysex_buffer[k+2] = SystemFlags.midi_rx_buffer[j+2];
			k +=3;
			j +=4;
			SystemFlags.sysex_len+=3;
		}
		if ( SystemFlags.midi_rx_buffer[i] == SYSEX_END_1 )
		{
			SystemFlags.sysex_buffer[k] = SystemFlags.midi_rx_buffer[j];
			SystemFlags.sysex_len+=0;
			i = len;
		}
		if ( SystemFlags.midi_rx_buffer[i] == SYSEX_END_2 )
		{
			SystemFlags.sysex_buffer[k] = SystemFlags.midi_rx_buffer[j];
			SystemFlags.sysex_buffer[k+1] = SystemFlags.midi_rx_buffer[j+1];
			SystemFlags.sysex_len+=1;
			i = len;
		}
		if ( SystemFlags.midi_rx_buffer[i] == SYSEX_END_3 )
		{
			SystemFlags.sysex_buffer[k] = SystemFlags.midi_rx_buffer[j];
			SystemFlags.sysex_buffer[k+1] = SystemFlags.midi_rx_buffer[j+1];
			SystemFlags.sysex_buffer[k+2] = SystemFlags.midi_rx_buffer[j+2];
			SystemFlags.sysex_len+=2;
			i = len;
		}
	}
	if ( SystemFlags.sysex_buffer[SystemFlags.sysex_len] == SYSEX_END )
		UsbSysExApplyValues();
	return len;
}

static void change_wave(uint8_t channel, uint8_t value)
{
uint8_t	osc_number;

	SystemFlags.osc_waves[channel] = value;
	for(osc_number = 0; osc_number < NUMOSCILLATORS;osc_number+=VOICES)
		Oscillator[osc_number+channel].waveform = value;
	SystemFlags.oscillator_flags |= OSC_WAVE_PENDING;
}

static void change_volume(uint8_t channel, uint8_t value)
{
	SystemFlags.osc_volume[channel] = 0;
	if ( value != 0 )
		SystemFlags.osc_volume[channel] = (uint8_t )((float )value * 0.078740157F) +1;
	ChangeOscillatorVolume(channel);
	SystemFlags.tonormaldisplay_counter = TIME_FOR_INFO;
	SystemFlags.rollback_flags |= ROLL_VOLUME;
	SystemFlags.control_flags |= CONTROL_ROLLBACK2ADSR;
}

static void change_duty(uint8_t channel, uint8_t value) // max_val is 100
{
	//SystemFlags.osc_duty_percent[channel] = value;
	ChangeOscillatorDuty(channel,value);
	SystemFlags.oscillator_flags |= OSC_DUTY_PENDING;
	SystemFlags.tonormaldisplay_counter = TIME_FOR_INFO;
	SystemFlags.rollback_flags |= ROLL_DUTY;
	SystemFlags.control_flags |= CONTROL_ROLLBACK2ADSR;
}

#define	DETUNE_SCALING	(100.0F / 127.0F)
static void change_detune(uint8_t channel, uint8_t value)
{
float	 detune;
	detune = midi_freq[Oscillator[channel].midi_note]*0.2F;
	Oscillator[channel].detune = detune * (value/512.0F);
	SystemFlags.oscillator_flags |= OSC_TUNE_PENDING;
	SystemFlags.osc_detune[channel] = (uint8_t )((float )value * DETUNE_SCALING);
	SystemFlags.tonormaldisplay_counter = TIME_FOR_INFO;
	SystemFlags.rollback_flags |= ROLL_DETUNE;
	SystemFlags.control_flags |= CONTROL_ROLLBACK2ADSR;
}

static void change_noiseweight(uint8_t channel, uint8_t value)
{
	if (( value > 1 ) && ( value < 7 ))
		Oscillator[channel].noise_weight = value;
	else
		Oscillator[channel].noise_weight = 7;
}

static void change_vcf_resonance( uint8_t value )
{
	if (( SystemFlags.vcf_flags & VCF_CONTROL_MASK) == VCF_CONTROL_MIDI)
		VCFParameters.filterResonance  = (float) value * 0.007874016F;
}

static void change_vcf_cutoff( uint8_t value )
{
	if (( SystemFlags.vcf_flags & VCF_CONTROL_MASK) == VCF_CONTROL_MIDI)
		VCFParameters.filterCutoff  = (float) value * 0.007874016F;
}

static void change_vcf_type( uint8_t value )
{
	SystemFlags.vcf_flags &= ~VCF_TYPE_MASK;
	SystemFlags.vcf_flags |= value;
	Draw_Filter_Params();
}

static void change_vcf_source( uint8_t value )
{
	SystemFlags.vcf_flags &= ~VCF_CONTROL_MASK;
	SystemFlags.vcf_flags |= value;
	Draw_Filter_Params();
}

static void change_vcf_enable( uint8_t value )
{
	if ( value != 0 )
		SystemFlags.vcf_flags |= VCF_ENABLED;
	else
		SystemFlags.vcf_flags &= ~VCF_ENABLED;

	Draw_Filter_Params();
	View_Sequence();
}

static void change_DelayLen( uint8_t value )
{
	SystemFlags.delay_value = value*10;
	View_Delay_Val();
}

static void change_DelaySource( uint8_t value )
{
	SystemFlags.delay_flags &= ~DLY_MIXER_SOURCE_MASK;
	SystemFlags.delay_flags |= (value << (DLY_MIXER_SOURCE_MASK-1));
	View_Delay();
}

static void change_DelayEnable( uint8_t value )
{
	SystemFlags.delay_flags = value;
	View_Sequence();
	View_Delay();
	View_Delay_Val();
}


uint8_t	atime,dtime,sval,rtime;

static void change_Atime( uint8_t value )
{
	atime = value;
}

static void change_Dtime( uint8_t value )
{
	dtime = value;
}

static void change_Sval( uint8_t value )
{
	sval = value;
}

static void change_Rtime( uint8_t value )
{
	SystemFlags.Atime = atime;
	SystemFlags.Dtime = dtime;
	SystemFlags.Sval = sval;
	SystemFlags.Rtime = value;
	DisplayADSR();
}

uint8_t	UsbMidiParseControlChange(uint8_t cc_index,uint8_t cc_value)
{
	switch(cc_index)
	{
	case CC_OSCWAVE0		:
	case CC_OSCWAVE1		:
	case CC_OSCWAVE2		:
	case CC_OSCWAVE3		:
		change_wave(cc_index & 0x03 ,cc_value & 0x7f);
		break;
	case CC_OSCVOLUME0		:
	case CC_OSCVOLUME1		:
	case CC_OSCVOLUME2		:
	case CC_OSCVOLUME3		:
		change_volume(cc_index & 0x03 ,cc_value & 0x7f);
		break;
	case CC_OSCDUTY0		:
	case CC_OSCDUTY1		:
	case CC_OSCDUTY2		:
	case CC_OSCDUTY3		:
		change_duty(cc_index & 0x03,cc_value & 0x7f);
		break;
	case CC_OSCDETUNE0		:
	case CC_OSCDETUNE1		:
	case CC_OSCDETUNE2		:
	case CC_OSCDETUNE3		:
		change_detune(cc_index & 0x03,cc_value & 0x7f);
		break;
	case CC_NOISE2WAVEWEIGHT0		:
	case CC_NOISE2WAVEWEIGHT1		:
	case CC_NOISE2WAVEWEIGHT2		:
	case CC_NOISE2WAVEWEIGHT3		:
		change_noiseweight(cc_index & 0x03,cc_value & 0x7f);
		break;
	case CC_VCFENABLE		:
		change_vcf_enable(cc_value & 0x7f);
		break;
	case CC_VCFTYPE		:
		change_vcf_type(cc_value & 0x7f);
		break;
	case CC_VCFFREQUENCY		:
		change_vcf_cutoff(cc_value & 0x7f);
		break;
	case CC_VCFRESONANCE		:
		change_vcf_resonance(cc_value & 0x7f);
		break;
	case CC_VCFSOURCE		:
		change_vcf_source(cc_value & 0x7f);
		break;
	case CC_DLYENABLE		:
		change_DelayEnable(cc_value & 0x7f);
		break;
	case CC_DELAYLEN		:
		change_DelayLen(cc_value & 0x7f);
		break;
	case CC_DELAYSOURCE		:
		change_DelaySource(cc_value & 0x7f);
		break;

	case CC_ATIME		:
		change_Atime(cc_value & 0x7f);
		break;
	case CC_DTIME		:
		change_Dtime(cc_value & 0x7f);
		break;
	case CC_SVAL		:
		change_Sval(cc_value & 0x7f);
		break;
	case CC_RTIME		:
		change_Rtime(cc_value & 0x7f);
		break;
	default : break;
	}
	return 4;
}

void UsbMidiParser(void)
{
uint8_t		cmd,channel,midi_note,velocity;
uint32_t	l_index=0,len;

	len = SystemFlags.midi_rx_length;
	SystemFlags.midi_rx_length = 0;
	while ( l_index < len )
	{
		cmd = SystemFlags.midi_rx_buffer[l_index] & CIN_MASK;
		channel = SystemFlags.midi_rx_buffer[l_index+1] & CHANNEL_MASK;
		midi_note = SystemFlags.midi_rx_buffer[l_index+2];
		velocity = SystemFlags.midi_rx_buffer[l_index+3];
		switch(cmd)
		{
			case NOTE_OFF			:
				l_index += UsbMidiParseNoteOFF(channel,midi_note,velocity);
				break;
			case NOTE_ON			:
				l_index += UsbMidiParseNoteON (channel,midi_note,velocity);
				break;
			case SYSEX_END_1			:
				break;
			case SYSEX_STARTC			:
			case SYSEX_END_2			:
			case SYSEX_END_3			:
				l_index += UsbMidiParseSysEx (channel,cmd,len);
				break;
			case CONTROL_CHANGE			:
				l_index += UsbMidiParseControlChange (SystemFlags.midi_rx_buffer[l_index+2],SystemFlags.midi_rx_buffer[l_index+3]);
				break;
				/*
			case PROGRAM_CHANGE			:
				l_index += UsbMidiParseProgramChange (SystemFlags.midi_rx_buffer[l_index+2],SystemFlags.midi_rx_buffer[l_index+3]);
				break;
			case POLY_PRESSURE			:
				l_index += UsbMidiParsePolyPressure (channel,midi_note,velocity);
				break;
			case PITCH_BEND			:
				l_index += UsbMidiParsePitchBend (channel,midi_note,velocity);
				break;
			case SINGLEBYTE			:
				l_index += UsbMidiParseSingleByte (channel,midi_note,velocity);
				break;
			case MISC			:
				l_index += UsbMidiParseMisc (channel,midi_note,velocity);
				break;
			case CABLE			:
				l_index += UsbMidiParseCable (channel,midi_note,velocity);
				break;
			case TWO_BYTES			:
				l_index += UsbMidiParseTwoBytes (channel,midi_note,velocity);
				break;
			case THREE_BYTES			:
				l_index += UsbMidiParseThreeBytes (channel,midi_note,velocity);
				break;
				*/
			default			:	l_index += 4; break;
		}
	}
}

void UsbMidiCheck(void)
{
	if (( SystemFlags.control_flags & CONTROL_MIDIRX_FLAG ) == CONTROL_MIDIRX_FLAG)
	{
		SystemFlags.control_flags &= ~CONTROL_MIDIRX_FLAG;
		UsbMidiParser();
	}
}


