/*
 * menus.h
 *
 *  Created on: Jul 16, 2021
 *      Author: fil
 */

#ifndef INC_BB101_VCO_MENUS_MENUS_H_
#define INC_BB101_VCO_MENUS_MENUS_H_
#define	LINE_SPACING	12
#define	TITLE_LINE_SPACING	12
#define	MENU_X_TITLE	8
#define	MENU_Y_TITLE	0
#define	STD_X_SPACE		1
#define	LINE1_X			STD_X_SPACE
#define	LINE1_Y			MENU_Y_TITLE+TITLE_LINE_SPACING
#define	LINE2_X			STD_X_SPACE
#define	LINE2_Y			LINE1_Y+LINE_SPACING
#define	LINE3_X			STD_X_SPACE
#define	LINE3_Y			LINE2_Y+LINE_SPACING
#define	LINE4_X			STD_X_SPACE
#define	LINE4_Y			LINE3_Y+LINE_SPACING
#define	LINE5_X			STD_X_SPACE
#define	LINE5_Y			LINE4_Y+LINE_SPACING
#define	LINE6_X			0
#define	LINE6_Y			LINE5_Y+LINE_SPACING

#define	LINE_X_STATUS	80
#define	LINE_Y_STATUS	0
#define	LINE_Y_STATUS_DELTA	11
#define	OSC_H			14

#define	ADSR_X			LINE_X_STATUS
#define	ADSR_Y			LINE_Y_STATUS+24
#define	ADSR_W			(160-LINE_X_STATUS)
#define	ADSR_H			32


#define	LINE_X_INPUT	LINE_X_STATUS+14
#define	LINE_X_INPUT_MIDI	LINE_X_STATUS-3
#define	LINE_Y_INPUT	LINE_Y_STATUS

#define	LINE_X_TUNER	LINE_X_STATUS+49
#define	LINE_Y_TUNER	LINE_Y_STATUS

#define	LINE_X_OSC1W	LINE_X_STATUS+8
#define	LINE_X_OSC2W	LINE_X_OSC1W+8
#define	LINE_X_OSC3W	LINE_X_OSC2W+8
#define	LINE_X_OSC4W	LINE_X_OSC3W+8
#define	LINE_Y_OSCW		LINE_Y_STATUS+9

#define	LINE_X_DUTY_DELTA	21

#define	LINE_X_DUTY		LINE_X_STATUS
#define	LINE_Y_TDUTY	LINE_Y_STATUS+LINE_Y_STATUS_DELTA+OSC_H+6
#define	LINE_Y_DUTY		LINE_Y_STATUS+LINE_Y_STATUS_DELTA+OSC_H+16

#define	LINE_X_DETUNE_DELTA	21

#define	LINE_X_DETUNE	LINE_X_STATUS
#define	LINE_Y_TDETUNE	LINE_Y_STATUS+LINE_Y_STATUS_DELTA+OSC_H+6
#define	LINE_Y_DETUNE	LINE_Y_STATUS+LINE_Y_STATUS_DELTA+OSC_H+16

#define	LINE_X_VOLUME_DELTA	21

#define	LINE_X_VOLUME	LINE_X_STATUS
#define	LINE_Y_TVOLUME	LINE_Y_STATUS+LINE_Y_STATUS_DELTA+OSC_H+6
#define	LINE_Y_VOLUME	LINE_Y_STATUS+LINE_Y_STATUS_DELTA+OSC_H+16

#define	LINE_X_FILTER	LINE_X_STATUS + 38
#define	LINE_Y_FILTER	LINE_Y_STATUS+LINE_Y_STATUS_DELTA+OSC_H+35

#define	LINE_X_CONTROL	LINE_X_STATUS + 57
#define	LINE_Y_CONTROL	LINE_Y_FILTER

#define	LINE_X_DELAY	LINE_X_STATUS
#define	LINE_Y_DELAY	LINE_Y_CONTROL+LINE_Y_STATUS_DELTA

#define	LINE_X_DELAYTYPE	LINE_X_STATUS+38
#define	LINE_Y_DELAYTYPE	LINE_Y_DELAY

#define	LINE_X_SEQUENCE	LINE_X_STATUS+57
#define	LINE_Y_SEQUENCE	LINE_Y_DELAY

#define	ADSR_X_ORIGIN		LINE_X_DUTY
#define	ADSR_Y_ORIGIN		ADSR_Y+ADSR_H

enum MENU_STATUS
{
	MENU_STATE_TOP,
	MENU_STATE_OSC_TOP,
	MENU_STATE_OSC_WAVE,
	MENU_STATE_OSC_DUTY,
	MENU_STATE_OSC_DETUNE,
	MENU_STATE_OSC_VOLUME,
	MENU_STATE_ADSR,
	MENU_STATE_AFX,
	MENU_STATE_DELAY,
	MENU_STATE_SYSTEM,
	MENU_STATE_PGM
};

#define	MENU_MAX_TOP_LINE			4
#define	MENU_MAX_OSC_LINE			6
#define	MENU_MAX_OSC_WAVE_LINE		5
#define	MENU_MAX_OSC_DUTY_LINE		5
#define	MENU_MAX_OSC_DETUNE_LINE	5
#define	MENU_MAX_OSC_VOLUME_LINE	5
#define	MENU_MAX_OSC_ADSR_LINE		5
#define	MENU_MAX_EFF_LINE			5
#define	MENU_MAX_DLY_LINE			5
#define	MENU_MAX_SYSTEM_LINE		6
#define	MENU_MAX_PGM_LINE			4

typedef struct _ScreenTypeDef
{
	char		line[9];
	uint16_t	x,y;
	uint16_t	fcolor;
	uint16_t	bcolor;
}ScreenTypeDef;


extern	void Main_Menu(void);
extern	void DoMenus(void);
extern	void MenusDrawStatus(void);
extern	void MenusDrawEmptyStatus(void);
extern	void Draw_Waveform(uint8_t oscillator_offset,uint8_t waveform);
extern	uint8_t ChangeOscillatorDuty(uint8_t oscillator_offset , uint8_t duty );
extern	uint8_t ChangeOscillatorDetune(uint8_t oscillator_offset );
extern	uint8_t ChangeOscillatorVolume(uint8_t oscillator_offset );
extern	void DisplayVolume(void);
extern	void DisplayDuty(void);
extern	void DisplayDetune(void);
extern	void Draw_AFX_Params(void);
extern	void Draw_Effects(void);
extern	void View_Sequence(void);
extern	void View_Delay(void);
extern	void View_Delay_Val(void);


extern	const uint16_t logo[12800];
extern	const uint16_t sine[196];
extern	const uint16_t sqrw[196];
extern	const uint16_t tria[196];
extern	const uint16_t noise[196];


#endif /* INC_BB101_VCO_MENUS_MENUS_H_ */
