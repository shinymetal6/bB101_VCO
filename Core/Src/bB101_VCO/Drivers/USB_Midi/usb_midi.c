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

static float detune_helper(uint8_t hundred, uint8_t tenth,uint8_t unit , uint8_t decimal)
{
uint32_t	number;
	number = ((hundred & 0x0f) * 1000) + ((tenth & 0x0f) * 100) + ((unit & 0x0f) * 10 ) + ((decimal & 0x0f));
	return (float )number * 0.1F;
}

uint32_t buf2int(uint8_t *ptr,uint32_t len)
{
uint32_t	i,start_val=1 , value=0;

	for(i=0;i<len;i++)
		start_val *=10;
	start_val /= 10;

	for(i=0;i<len;i++)
	{
		value += (ptr[i] & 0x0f)*start_val;
		start_val /= 10;
	}
	return value;
}
uint32_t	cutoff , resonance;
uint16_t	delay_from_midi;
void apply_vcf_params(void)
{

	if (( SystemFlags.vcf_flags & VCF_CONTROL_MASK) == VCF_CONTROL_MIDI)
	{
		cutoff = buf2int(&SystemFlags.sysex_buffer[2],4);
		resonance = buf2int(&SystemFlags.sysex_buffer[6],2);
		VCFParameters.filterCutoff  = (uint16_t )((float) cutoff * 0.372F);
		VCFParameters.filterResonance = ((resonance+1)*4096)/100;
		SystemFlags.vcf_flags &= ~VCF_TYPE_MASK;
		SystemFlags.vcf_flags |= (SystemFlags.sysex_buffer[8] & VCF_TYPE_MASK); // filter type
	}
}

void apply_delay_params(void)
{
	if (( SystemFlags.delay_flags & DLY_ENABLED) == DLY_ENABLED )
	{
		if (( SystemFlags.delay_flags & DLY_MIXER_MIDI_MASK) == DLY_MIXER_MIDI_MASK )
		{
			SystemFlags.delay_value = buf2int(&SystemFlags.sysex_buffer[2],3)+1;
			if ( SystemFlags.delay_value > DELAY_MAX_VALUE )
				SystemFlags.delay_value = DELAY_MAX_VALUE;
		}
	}
}

void UsbSysExApplyValues(void)
{
uint8_t	osc_number, offset;

	if ( SystemFlags.sysex_buffer[1] == SYS_COMMAND_CMD)
	{
		if (( SystemFlags.sysex_buffer[2] == SYSEX_RESET_CMD ) && ( SystemFlags.sysex_buffer[3] == SYSEX_RESET_CMD ))
			EmergencyReset();
		if ( SystemFlags.sysex_buffer[2] == SYSEX_DUMP_PROGRAM_CMD )
			DumpProgram(SystemFlags.sysex_buffer[3] & (NUM_PROGRAMS_MAX-1));
		if ( SystemFlags.sysex_buffer[2] == SYSEX_DUMP_CURRENT_PROGRAM_CMD )
			DumpCurrentProgram();
	}


	if ( SystemFlags.sysex_buffer[1] == OSC_ADSR_CMD)
	{
		SystemFlags.Atime = SystemFlags.sysex_buffer[2];
		SystemFlags.Dtime = SystemFlags.sysex_buffer[3];
		SystemFlags.Rtime = SystemFlags.sysex_buffer[4];
		SystemFlags.Sval  = SystemFlags.sysex_buffer[5];
		if ( SystemFlags.Atime > ADSR_W )
			SystemFlags.Atime = ADSR_W;
		if ( SystemFlags.Dtime > ADSR_W )
			SystemFlags.Dtime = ADSR_W;
		if ( SystemFlags.Rtime > ADSR_W )
			SystemFlags.Rtime = ADSR_W;
		if ( SystemFlags.Sval > ADSR_H )
			SystemFlags.Sval = ADSR_H;

		for(osc_number = 0; osc_number < NUMOSCILLATORS;osc_number+=VOICES)
		{
			SetADSR_oscParams(osc_number,0);
		}
		DisplayADSR();
	}

	if ( SystemFlags.sysex_buffer[1] == OSC_WAVE_CMD)
	{
		offset = SystemFlags.sysex_buffer[2] & 0x03;
		SystemFlags.osc_waves[offset] = SystemFlags.sysex_buffer[3];
		for(osc_number = 0; osc_number < NUMOSCILLATORS;osc_number+=VOICES)
			Oscillator[osc_number+offset].waveform = SystemFlags.sysex_buffer[3];
		SystemFlags.oscillator_flags |= OSC_WAVE_PENDING;
	}

	if ( SystemFlags.sysex_buffer[1] == OSC_ALLWAVE_CMD)
	{
		SystemFlags.oscillator_flags &= OSC_SRCFLAG;
		for(osc_number = 0; osc_number < NUMOSCILLATORS;osc_number++)
			Oscillator[osc_number].waveform = SystemFlags.sysex_buffer[3];
		for(osc_number = 0; osc_number < VOICES;osc_number++)
			SystemFlags.osc_waves[osc_number] = SystemFlags.sysex_buffer[3];

		SystemFlags.oscillator_flags |= OSC_WAVE_PENDING;
	}

	if ( SystemFlags.sysex_buffer[1] == OSC_DUTY_CMD)
	{
		offset = SystemFlags.sysex_buffer[2] & 0x03;
		SystemFlags.osc_duty_percent[offset] = SystemFlags.sysex_buffer[3];
		ChangeOscillatorDuty(offset);
		SystemFlags.oscillator_flags |= OSC_DUTY_PENDING;
	}

	if ( SystemFlags.sysex_buffer[1] == OSC_ALLDUTY_CMD)
	{
		offset = SystemFlags.sysex_buffer[2] & 0x03;
		for(osc_number = 0; osc_number < VOICES;osc_number++)
		{
			SystemFlags.osc_duty_percent[osc_number] = SystemFlags.sysex_buffer[3];
			ChangeOscillatorDuty(osc_number);
		}
		SystemFlags.oscillator_flags |= OSC_DUTY_PENDING;
	}

	if ( SystemFlags.sysex_buffer[1] == OSC_VOLUME_CMD)
	{
		offset = SystemFlags.sysex_buffer[2] & 0x03;
		SystemFlags.osc_volume[offset] = SystemFlags.sysex_buffer[3];
		ChangeOscillatorVolume(offset);
		//DisplayVolume();
	}

	if ( SystemFlags.sysex_buffer[1] == OSC_ALLVOLUME_CMD)
	{
		offset = SystemFlags.sysex_buffer[2] & 0x03;
		for(osc_number = 0; osc_number < VOICES;osc_number++)
		{
			SystemFlags.osc_volume[offset] = SystemFlags.sysex_buffer[3];
			ChangeOscillatorVolume(osc_number);
		}
	}

	if ( SystemFlags.sysex_buffer[1] == OSC_DETUNE_CMD)
	{
		osc_number = SystemFlags.sysex_buffer[2];
		Oscillator[osc_number].detune = detune_helper(SystemFlags.sysex_buffer[3],SystemFlags.sysex_buffer[4],SystemFlags.sysex_buffer[5],SystemFlags.sysex_buffer[6]);
		SetDetune(osc_number);
	}

	if ( SystemFlags.sysex_buffer[1] == VCF_SET_CMD)
		apply_vcf_params();
	if ( SystemFlags.sysex_buffer[1] == DLY_SET_CMD)
		apply_delay_params();
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
				/*
			case CONTROL_CHANGE			:
				l_index += UsbMidiParseControlChange (channel,midi_note,velocity);
				break;
			case PROGRAM_CHANGE			:
				l_index += UsbMidiParseProgramChange (channel,midi_note,velocity);
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


