/*
 * usb_midi.h
 *
 *  Created on: Dec 23, 2020
 *      Author: fil
 */

#ifndef INC_BB100_USB_MIDI_H_
#define INC_BB100_USB_MIDI_H_


#define	MIDI_RXBUF_SIZE			256
#define	USB_TXBUF_SIZE			256
extern	uint32_t	usb_packet_ready;
extern	uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
extern	uint8_t	USB_RxBuffer[64];
extern	char	USB_TxBuffer[USB_TXBUF_SIZE];
extern	uint32_t USB_RxBufferLen;
extern	void CheckUSB(void);

#define	CMD_MASK				0xF0
#define	CHANNEL_MASK			0x0F

#define	CABLEN_MASK				0xF0
#define	CIN_MASK				0x0F

#define	MISC					0x00
#define	CABLE					0x01
#define	TWO_BYTES				0x02
#define	THREE_BYTES				0x03
#define	SYSEX_STARTC			0x04
#define	SYSEX_END_1				0x05
#define	SYSEX_END_2				0x06
#define	SYSEX_END_3				0x07

#define	NOTE_OFF				0x08
#define	NOTE_ON					0x09
#define	POLY_PRESSURE			0x0A
#define	CONTROL_CHANGE			0x0B
#define	PROGRAM_CHANGE			0x0C
#define	CHANNEL_PRESSURE		0x0D
#define	PITCH_BEND				0x0E
#define	SINGLEBYTE				0x0F

#define	SYSEX_START				0xF0
#define	SYSEX_END				0xF7

#define	OSC_ADSR_CMD			0x40
#define	OSC_DETUNE_CMD			0x41
#define	OSC_WAVE_CMD			0x42
#define	OSC_DUTY_CMD			0x43
#define	OSC_VOLUME_CMD			0x44
#define	OSC_ALLWAVE_CMD			0x4A
#define	OSC_ALLDUTY_CMD			0x4B
#define	OSC_ALLVOLUME_CMD		0x4C

#define	VCF_SET_CMD				0x50
#define	DLY_SET_CMD				0x60

#define	SYS_PARAM_CMD			0x20
#define	SYS_MACHINE_CMD			0x21
#define	SYS_COMMAND_CMD			0x30

#define	SYSEX_RESET_CMD			0x55
#define	SYSEX_DUMP_SYSFLAGS_CMD	0x11
#define	SYSEX_DUMP_PROGRAM_CMD	0x22
#define	SYSEX_DUMP_CURRENT_PROGRAM_CMD	0x23

#define	CC_OSCWAVE0				0x00
#define	CC_OSCWAVE1				0x01
#define	CC_OSCWAVE2				0x02
#define	CC_OSCWAVE3				0x03
#define	CC_OSCVOLUME0			0x04
#define	CC_OSCVOLUME1			0x05
#define	CC_OSCVOLUME2			0x06
#define	CC_OSCVOLUME3			0x07
#define	CC_OSCDUTY0				0x08
#define	CC_OSCDUTY1				0x09
#define	CC_OSCDUTY2				0x0A
#define	CC_OSCDUTY3				0x0B
#define	CC_OSCDETUNE0			0x0C
#define	CC_OSCDETUNE1			0x0D
#define	CC_OSCDETUNE2			0x0E
#define	CC_OSCDETUNE3			0x0F
#define	CC_AFXENABLE			0x20
#define	CC_AFXCONTROL			0x21
#define	CC_AFXTYPE				0x22
#define	CC_AFXFREQUENCY			0x23
#define	CC_AFXRESONANCE			0x24
#define	CC_AFXCONTROLSOURCE		0x25
#define	CC_DLYENABLE			0x30
#define	CC_DELAYLEN				0x31
#define	CC_DELAYBALANCE			0x32
#define	CC_DELAYSOURCE			0x33
#define	CC_ATIME				0x40
#define	CC_DTIME				0x41
#define	CC_SVAL					0x42
#define	CC_RTIME				0x43
#define	CC_SEQUENCE				0x50

typedef struct _OscParametersCmdTypeDef
{
	uint8_t 	opening_flag;
	uint8_t 	command;
	uint8_t 	oscillator_number;
	uint8_t 	a_level;
	uint8_t 	d_level;
	uint8_t 	s_level;
	uint8_t 	r_level;
	uint8_t 	a_value;
	uint8_t 	d_value;
	uint8_t 	waveform;
	uint8_t 	detune;
	uint8_t 	phase;
	uint8_t 	closing_flag;
}OscParametersCmdTypeDef;

#define	MIDI_PROGRAM_SIZE	44

extern	void UsbMidiCheck(void);

#endif /* INC_BB100_USB_MIDI_H_ */
