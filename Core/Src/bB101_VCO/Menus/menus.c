/*
 * menus.c
 *
 *  Created on: Jul 16, 2021
 *      Author: fil
 */


#include "main.h"
uint16_t	line_ptr = 0;
__attribute__((section(".table"))) static const ScreenTypeDef	Main_Menu_Screen[] = {
		{
				"bB101",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"OSCs",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"AFX",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Delay",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"System",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};

__attribute__((section(".table"))) static const ScreenTypeDef	Osc_Menu_Screen[] = {
		{
				"OSCs",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Wave",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Duty",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"deTune",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Volume",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"ADSR",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE6_X,
				LINE6_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};
__attribute__((section(".table"))) static const ScreenTypeDef	Osc_WaveMenu_Screen[] = {
		{
				"Waves",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Wave 1",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Wave 2",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Wave 3",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Wave 4",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};
__attribute__((section(".table"))) static const ScreenTypeDef	Osc_DutyMenu_Screen[] = {
		{
				"Duty",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Duty 1",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Duty 2",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Duty 3",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Duty 4",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};

__attribute__((section(".table"))) static const ScreenTypeDef	Osc_DeTuneMenu_Screen[] = {
		{
				"dTune",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"deTune 1",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"deTune 2",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"deTune 3",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"deTune 4",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};

__attribute__((section(".table"))) static const ScreenTypeDef	Osc_VolumeMenu_Screen[] = {
		{
				"Vol",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Volume 1",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Volume 2",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Volume 3",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Volume 4",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};

__attribute__((section(".table"))) static const ScreenTypeDef	Osc_ADSRMenu_Screen[] = {
		{
				"ADSR",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"A Time",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"D Time",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"S Val",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"R Time",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};

__attribute__((section(".table"))) static const ScreenTypeDef	AFX_Menu_Screen[] = {
		{
				"AFX",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Type",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Band",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Control",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Enable",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};


__attribute__((section(".table"))) static const ScreenTypeDef	Delay_Menu_Screen[] = {
		{
				"Delay",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Increment",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Decrement",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Type",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Enable",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};

__attribute__((section(".table"))) static const ScreenTypeDef	System_Menu_Screen[] = {
		{
				"Sys",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Input",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"VCA",
				LINE2_X,
				LINE2_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"FM",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Seq",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Memory",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE6_X,
				LINE6_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};

__attribute__((section(".table"))) static const ScreenTypeDef	System_PGM_Menu_Screen[] = {
		{
				"Mem",
				MENU_X_TITLE,
				MENU_Y_TITLE,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"PGM num",
				LINE1_X,
				LINE1_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"LOAD",
				LINE3_X,
				LINE3_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"STORE",
				LINE4_X,
				LINE4_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				"Return",
				LINE5_X,
				LINE5_Y,
				ST7735_BLUE,
				ST7735_BLACK
		},
		{
				" ",
		},
};

void Menus_Hilight_Line(ScreenTypeDef *screen, uint16_t old_line_number, uint16_t new_line_number)
{
	ST7735_WriteString(screen[old_line_number].x,screen[old_line_number].y,screen[old_line_number].line, Font_7x10, screen[old_line_number].fcolor, screen[old_line_number].bcolor);
	ST7735_WriteString(screen[new_line_number].x,screen[new_line_number].y,screen[new_line_number].line, Font_7x10, ST7735_BLACK, ST7735_BLACK);
	ST7735_WriteString(screen[new_line_number].x,screen[new_line_number].y,screen[new_line_number].line, Font_7x10, screen[new_line_number].bcolor, screen[new_line_number].fcolor);
}

void Draw_Menu(ScreenTypeDef *screen)
{
uint8_t	i=0;
	while(screen[i].line[0] != 0x20 )
	{
		if (( screen == Main_Menu_Screen ) && (i == 0 ))
		{
			ST7735_WriteString(screen[i].x,   screen[i].y,SystemParameters.Header,  Font_7x10, ST7735_BLUE, ST7735_BLACK);
			ST7735_WriteString(screen[i].x+(strlen(SystemParameters.Header)*7)+4,screen[i].y,SystemParameters.Version, Font_7x10, ST7735_BLUE, ST7735_BLACK);
		}
		if ( i == 1 )
		{
			SystemFlags.menu_line_counter = 1;
			ST7735_WriteString(screen[i].x,screen[i].y,screen[i].line, Font_7x10, ST7735_BLACK, ST7735_BLUE);
		}
		else
			ST7735_WriteString(screen[i].x,screen[i].y,screen[i].line, Font_7x10, ST7735_BLUE, ST7735_BLACK);
		i++;
	}
}

void Main_Menu(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	Draw_Menu((ScreenTypeDef *)&Main_Menu_Screen);
	DisplayADSR();
}

void Osc_Menu(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	Draw_Menu((ScreenTypeDef *)&Osc_Menu_Screen);
}

void Osc_WaveMenu(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	Draw_Menu((ScreenTypeDef *)&Osc_WaveMenu_Screen);
}

void Delay_Menu(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	Draw_Menu((ScreenTypeDef *)&Delay_Menu_Screen);
}

void move_menu(ScreenTypeDef *screen, uint16_t old_line_number, uint16_t new_line_number, uint16_t max_line_number)
{
	if (( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0)
	{
		if ( SystemFlags.menu_line_counter == 1 )
			SystemFlags.menu_line_counter = max_line_number;
		else
			SystemFlags.menu_line_counter --;
		SystemFlags.buttons_flag &= ~BUTTON_TACT0;
	}
	if (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1)
	{
		if ( SystemFlags.menu_line_counter >= max_line_number )
			SystemFlags.menu_line_counter = 1;
		else
			SystemFlags.menu_line_counter ++;
		SystemFlags.buttons_flag &= ~BUTTON_TACT1;
	}
	Menus_Hilight_Line(screen,old_line_number,SystemFlags.menu_line_counter);
}

void change_menu(ScreenTypeDef *new_screen,ScreenTypeDef *old_screen)
{
#ifdef	SLOW_FILL
	ST7735_FillScreen(ST7735_BLACK);
#else
uint8_t	i=0;
	SystemFlags.menu_line_counter = 1;
	while(old_screen[i].line[0] != 0x20 )
	{
		ST7735_WriteString(old_screen[i].x,old_screen[i].y,old_screen[i].line, Font_7x10, ST7735_BLACK, ST7735_BLACK);
		i++;
	}
#endif
	Draw_Menu(new_screen);
}

void Draw_Waveform(uint8_t oscillator_offset,uint8_t waveform)
{
	switch (waveform)
	{
	case	SQUARE		:	ST7735_DrawImage(LINE_X_OSC1W+oscillator_offset*16,LINE_Y_OSCW+1, 14, 14, sqrw); break;
	case	TRIANGLE	:	ST7735_DrawImage(LINE_X_OSC1W+oscillator_offset*16,LINE_Y_OSCW+1, 14, 14, tria); break;
	case	NOISE		:	ST7735_DrawImage(LINE_X_OSC1W+oscillator_offset*16,LINE_Y_OSCW+1, 14, 14, noise); break;
	default				:	ST7735_DrawImage(LINE_X_OSC1W+oscillator_offset*16,LINE_Y_OSCW+1, 14, 14, sine); break; // 's' case
	}
}

uint8_t change_oscillator_waveform(uint8_t oscillator_offset )
{
uint8_t	i;

	SystemFlags.osc_waves[oscillator_offset]++;
	if ( SystemFlags.osc_waves[oscillator_offset] > NOISE)
		SystemFlags.osc_waves[oscillator_offset] = SINE;

	for(i=0;i<NUMOSCILLATORS;i+=VOICES)
	{
		Oscillator[i+oscillator_offset].waveform = SystemFlags.osc_waves[oscillator_offset];
	}
	Draw_Waveform(oscillator_offset , SystemFlags.osc_waves[oscillator_offset] );
	return Oscillator[oscillator_offset].waveform;
}

void DisplayDuty(void)
{
uint8_t	i,k;
char 	tmp_buf[16];
	DisplayClearADSR();
	k = LINE_X_DUTY;
	ST7735_WriteString(k,LINE_Y_TDUTY,"Duty", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
	for(i=0;i<4;i++)
	{
		sprintf(tmp_buf,"%d",SystemFlags.osc_duty_percent[i]);
		ST7735_WriteString(k,LINE_Y_DUTY,"  ", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		ST7735_WriteString(k,LINE_Y_DUTY,tmp_buf, Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		k += LINE_X_DUTY_DELTA;
	}
}

void DisplayDetune(void)
{
uint8_t	i,k;
char 	tmp_buf[16];
	DisplayClearADSR();
	k = LINE_X_DETUNE;
	ST7735_WriteString(k,LINE_Y_TDETUNE,"Detune", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
	for(i=0;i<4;i++)
	{
		sprintf(tmp_buf,"%d",SystemFlags.osc_detune[i]);
		ST7735_WriteString(k,LINE_Y_DETUNE,"  ", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		ST7735_WriteString(k,LINE_Y_DETUNE,tmp_buf, Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		k += LINE_X_DETUNE_DELTA;
	}
}

void DisplayVolume(void)
{
uint8_t	i,k;
char 	tmp_buf[16];
	DisplayClearADSR();
	k = LINE_X_VOLUME;
	ST7735_WriteString(k,LINE_Y_TVOLUME,"Volume", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
	for(i=0;i<4;i++)
	{
		sprintf(tmp_buf,"%d",SystemFlags.osc_volume[i]);
		ST7735_WriteString(k,LINE_Y_VOLUME,"  ", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		ST7735_WriteString(k,LINE_Y_VOLUME,tmp_buf, Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		k += LINE_X_DETUNE_DELTA;
	}
}

uint8_t ChangeOscillatorVolume(uint8_t oscillator_offset )
{
uint8_t	i;

	if ( SystemFlags.osc_volume[oscillator_offset] > 10 )
		SystemFlags.osc_volume[oscillator_offset] = 10;

	for(i=0;i<NUMOSCILLATORS;i+=VOICES)
	{
		Oscillator[i+oscillator_offset].volume = (float )SystemFlags.osc_volume[oscillator_offset] / 10.0F;
	}
	return 0;
}

uint8_t ChangeOscillatorDuty(uint8_t oscillator_offset , uint8_t duty)
{
uint8_t	i;
uint32_t	calc_duty;
	if ( duty > 99 )
		duty = 99;

	SystemFlags.osc_duty_percent[oscillator_offset & 0x03] = duty;
	calc_duty = (uint32_t )((float )duty*DUTY_SCALE);
	for(i=0;i<NUMOSCILLATORS;i+=VOICES)
	{
		Oscillator[i+oscillator_offset].duty = (uint8_t )calc_duty;
		ChangeOscillatorWavePhase(i+oscillator_offset,Oscillator[i+oscillator_offset].waveform , Oscillator[i+oscillator_offset].duty );
	}
	return 0;
}

uint8_t ChangeOscillatorDetune(uint8_t oscillator_offset )
{
uint8_t	i;
	for(i=0;i<NUMOSCILLATORS;i+=VOICES)
	{
		if ( SystemFlags.osc_detune[oscillator_offset] < 50 )
		{
			Oscillator[i+oscillator_offset].detune = (50.0F - (float )SystemFlags.osc_detune[oscillator_offset]) / 10.0F;
		}
		else
		{
			Oscillator[i+oscillator_offset].detune = ((float )SystemFlags.osc_detune[oscillator_offset] - 50.0F) / 10.0F;
		}
		SetDetune(i+oscillator_offset);
	}
	return 0;
}

void View_Sequence(void)
{
	ST7735_WriteString(LINE_X_SEQUENCE,LINE_Y_SEQUENCE,"O", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
	if (( SystemFlags.control_flags & CONTROL_OSC_VCF_DLY) == CONTROL_OSC_VCF_DLY)
	{
		if (( SystemFlags.vcf_flags & VCF_ENABLED) == VCF_ENABLED)
			ST7735_WriteString(LINE_X_SEQUENCE+7,LINE_Y_SEQUENCE,"X", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		else
			ST7735_WriteString(LINE_X_SEQUENCE+7,LINE_Y_SEQUENCE,"X", Font_7x10, ST7735_GREY, ST7735_BLACK);

		if (( SystemFlags.delay_flags & DLY_ENABLED) == DLY_ENABLED)
			ST7735_WriteString(LINE_X_SEQUENCE+14,LINE_Y_SEQUENCE,"D", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		else
			ST7735_WriteString(LINE_X_SEQUENCE+14,LINE_Y_SEQUENCE,"D", Font_7x10, ST7735_GREY, ST7735_BLACK);
	}
	else
	{
		if (( SystemFlags.delay_flags & DLY_ENABLED) == DLY_ENABLED)
			ST7735_WriteString(LINE_X_SEQUENCE+7,LINE_Y_SEQUENCE,"D", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		else
			ST7735_WriteString(LINE_X_SEQUENCE+7,LINE_Y_SEQUENCE,"D", Font_7x10, ST7735_GREY, ST7735_BLACK);
		if (( SystemFlags.vcf_flags & VCF_ENABLED) == VCF_ENABLED)
			ST7735_WriteString(LINE_X_SEQUENCE+14,LINE_Y_SEQUENCE,"X", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
		else
			ST7735_WriteString(LINE_X_SEQUENCE+14,LINE_Y_SEQUENCE,"X", Font_7x10, ST7735_GREY, ST7735_BLACK);
	}
}

void View_Delay(void)
{
uint16_t	color;

	if (( SystemFlags.delay_flags & DLY_ENABLED) == DLY_ENABLED)
		color = ST7735_DARKGREEN;
	else
		color = ST7735_GREY;
	switch(SystemFlags.delay_flags & DLY_TYPE_MASK )
	{
	case	DLY_MIXER_FLANGER_POT :
		ST7735_WriteString(LINE_X_DELAYTYPE, LINE_Y_DELAYTYPE, "FP", Font_7x10, color, ST7735_BLACK);
		break;
	case	DLY_MIXER_REVERB_POT :
		ST7735_WriteString(LINE_X_DELAYTYPE, LINE_Y_DELAYTYPE, "RP", Font_7x10, color, ST7735_BLACK);
		break;
	case	DLY_MIXER_FLANGER_MIDI :
		ST7735_WriteString(LINE_X_DELAYTYPE, LINE_Y_DELAYTYPE, "FM", Font_7x10, color, ST7735_BLACK);
		break;
	case	DLY_MIXER_REVERB_MIDI :
		ST7735_WriteString(LINE_X_DELAYTYPE, LINE_Y_DELAYTYPE, "RM", Font_7x10, color, ST7735_BLACK);
		break;
	default :
		ST7735_WriteString(LINE_X_DELAYTYPE, LINE_Y_DELAYTYPE, "FP", Font_7x10, color, ST7735_BLACK);
		break;
	}
}

void View_Delay_Val(void)
{
uint16_t	color = ST7735_GREY;
char 	tmp_buf[16];
uint16_t	delay;

	if (( SystemFlags.delay_flags & DLY_MIXER_POT_MASK) == 0 )	// comes from prog
		delay = SystemFlags.delay_value_from_prog;
	else
		delay = SystemFlags.delay_value_from_midi;


	if ( delay < 10 )
		sprintf(tmp_buf,"  %d",delay);
	else if ( delay < 100 )
		sprintf(tmp_buf," %d",delay);
	else
		sprintf(tmp_buf,"%d",delay);
	if (( SystemFlags.delay_flags & DLY_ENABLED) == DLY_ENABLED)
		color = ST7735_DARKGREEN;
	ST7735_WriteString(LINE_X_DELAY,LINE_Y_DELAY,tmp_buf, Font_7x10, color, ST7735_BLACK);
}

void view_am(void)
{
uint16_t	color;

	if (( SystemFlags.control_flags & CONTROL_VCA) == CONTROL_VCA)
		color = ST7735_DARKGREEN;
	else
		color = ST7735_GREY;
	ST7735_WriteString(LINE_X_INPUT+14,LINE_Y_INPUT,"a", Font_7x10, color, ST7735_BLACK);
}

void view_fm(void)
{
uint16_t	color;

	if (( SystemFlags.control_flags & CONTROL_FM) == CONTROL_FM)
		color = ST7735_DARKGREEN;
	else
		color = ST7735_GREY;
	ST7735_WriteString(LINE_X_INPUT+21,LINE_Y_INPUT,"f", Font_7x10, color, ST7735_BLACK);
}


void Draw_Filter_Params(void)
{
uint16_t	color = ST7735_GREY;
	if (( SystemFlags.vcf_flags & VCF_ENABLED) == VCF_ENABLED)
		color = ST7735_DARKGREEN;

	if (( SystemFlags.vcf_flags & VCF_TYPE_BP ) == VCF_TYPE_BP)
		ST7735_WriteString(LINE_X_FILTER,LINE_Y_FILTER,"BP", Font_7x10, color, ST7735_BLACK);
	if (( SystemFlags.vcf_flags & VCF_TYPE_HP ) == VCF_TYPE_HP)
		ST7735_WriteString(LINE_X_FILTER,LINE_Y_FILTER,"HP", Font_7x10, color, ST7735_BLACK);
	if (( SystemFlags.vcf_flags & VCF_TYPE_LP ) == VCF_TYPE_LP)
		ST7735_WriteString(LINE_X_FILTER,LINE_Y_FILTER,"LP", Font_7x10, color, ST7735_BLACK);
	if (( SystemFlags.vcf_flags & VCF_CONTROL_MIDI ) == VCF_CONTROL_MIDI)
		ST7735_WriteString(LINE_X_CONTROL,LINE_Y_CONTROL,"MIDI", Font_7x10, color, ST7735_BLACK);
	if (( SystemFlags.vcf_flags & VCF_CONTROL_CV ) == VCF_CONTROL_CV)
		ST7735_WriteString(LINE_X_CONTROL,LINE_Y_CONTROL,"CV  ", Font_7x10, color, ST7735_BLACK);
	if (( SystemFlags.vcf_flags & VCF_CONTROL_POT ) == VCF_CONTROL_POT)
		ST7735_WriteString(LINE_X_CONTROL,LINE_Y_CONTROL,"POT ", Font_7x10, color, ST7735_BLACK);
}

void Draw_Effects(void)
{
uint16_t	color = ST7735_GREY;
	if (( SystemFlags.vcf_flags & VCF_ENABLED) == VCF_ENABLED)
		color = ST7735_DARKGREEN;

	if (( SystemFlags.afx_flags & AFX_MOOG1) == AFX_MOOG1)
		ST7735_WriteString(LINE_X_STATUS,LINE_Y_FILTER,"Moog1", Font_7x10, color, ST7735_BLACK);
	if (( SystemFlags.afx_flags & AFX_MOOG2) == AFX_MOOG2)
		ST7735_WriteString(LINE_X_STATUS,LINE_Y_FILTER,"Moog2", Font_7x10, color, ST7735_BLACK);
	if (( SystemFlags.afx_flags & AFX_PHASER) == AFX_PHASER)
		ST7735_WriteString(LINE_X_STATUS,LINE_Y_FILTER,"Phase", Font_7x10, color, ST7735_BLACK);
}
void DoMenus(void)
{
uint16_t	old_line_number;
ScreenTypeDef	*current_screen;

	switch ( SystemFlags.menu_state)
	{
	case	MENU_STATE_TOP:
		current_screen = (ScreenTypeDef *)&Main_Menu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_TOP_LINE);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == 1)
			{
				SystemFlags.menu_state = MENU_STATE_OSC_TOP;
				change_menu((ScreenTypeDef *)&Osc_Menu_Screen,current_screen);
			}
			if ( SystemFlags.menu_line_counter == 2)
			{
				SystemFlags.menu_state = MENU_STATE_AFX;
				change_menu((ScreenTypeDef *)&AFX_Menu_Screen,current_screen);
			}
			if ( SystemFlags.menu_line_counter == 3)
			{
				SystemFlags.menu_state = MENU_STATE_DELAY;
				change_menu((ScreenTypeDef *)&Delay_Menu_Screen,current_screen);
			}
			if ( SystemFlags.menu_line_counter == MENU_MAX_TOP_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_SYSTEM;
				change_menu((ScreenTypeDef *)&System_Menu_Screen,current_screen);
			}
		}
		break;
	case	MENU_STATE_OSC_TOP:
		current_screen = (ScreenTypeDef *)&Osc_Menu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_OSC_LINE);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == MENU_MAX_OSC_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_TOP;
				change_menu((ScreenTypeDef *)&Main_Menu_Screen,current_screen);
			}
			else
			{
				if ( SystemFlags.menu_line_counter == 1)
				{
					SystemFlags.menu_state = MENU_STATE_OSC_WAVE;
					change_menu((ScreenTypeDef *)&Osc_WaveMenu_Screen,current_screen);
				}
				if ( SystemFlags.menu_line_counter == 2)
				{
					SystemFlags.menu_state = MENU_STATE_OSC_DUTY;
					change_menu((ScreenTypeDef *)&Osc_DutyMenu_Screen,current_screen);
					DisplayDuty();
				}
				if ( SystemFlags.menu_line_counter == 3)
				{
					SystemFlags.menu_state = MENU_STATE_OSC_DETUNE;
					change_menu((ScreenTypeDef *)&Osc_DeTuneMenu_Screen,current_screen);
					DisplayDetune();
				}
				if ( SystemFlags.menu_line_counter == 4)
				{
					SystemFlags.menu_state = MENU_STATE_OSC_VOLUME;
					change_menu((ScreenTypeDef *)&Osc_VolumeMenu_Screen,current_screen);
					DisplayVolume();
				}
				if ( SystemFlags.menu_line_counter == 5)
				{
					SystemFlags.menu_state = MENU_STATE_ADSR;
					change_menu((ScreenTypeDef *)&Osc_ADSRMenu_Screen,current_screen);
					DisplayADSR();
				}
			}
		}
		break;
	case	MENU_STATE_OSC_WAVE:
		current_screen = (ScreenTypeDef *)&Osc_WaveMenu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_OSC_WAVE_LINE);
			SystemFlags.buttons_flag &= ~( BUTTON_TACT0 | BUTTON_TACT1);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == MENU_MAX_OSC_WAVE_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_OSC_TOP;
				change_menu((ScreenTypeDef *)&Osc_Menu_Screen,current_screen);
			}
			else
				change_oscillator_waveform(SystemFlags.menu_line_counter-1);
		}
		break;
	case	MENU_STATE_OSC_DUTY:
		current_screen = (ScreenTypeDef *)&Osc_DutyMenu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_OSC_DUTY_LINE);
			SystemFlags.buttons_flag &= ~( BUTTON_TACT0 | BUTTON_TACT1);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == MENU_MAX_OSC_DUTY_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_OSC_TOP;
				change_menu((ScreenTypeDef *)&Osc_Menu_Screen,current_screen);
				DisplayADSR();
			}
			else
			{
				if ( SystemFlags.osc_duty[SystemFlags.menu_line_counter-1] < 95 )
					SystemFlags.osc_duty[SystemFlags.menu_line_counter-1] += 5;
				else
					SystemFlags.osc_duty[SystemFlags.menu_line_counter-1] = 5;

				ChangeOscillatorDuty(SystemFlags.menu_line_counter-1 , SystemFlags.osc_duty_percent[SystemFlags.menu_line_counter]);
				DisplayDuty();
			}
		}
		break;
	case	MENU_STATE_OSC_DETUNE:
		current_screen = (ScreenTypeDef *)&Osc_DeTuneMenu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_OSC_DETUNE_LINE);
			SystemFlags.buttons_flag &= ~( BUTTON_TACT0 | BUTTON_TACT1);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == MENU_MAX_OSC_DETUNE_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_OSC_TOP;
				change_menu((ScreenTypeDef *)&Osc_Menu_Screen,current_screen);
				DisplayADSR();
			}
			else
			{
				if ( SystemFlags.osc_detune[SystemFlags.menu_line_counter-1] < 99 )
					SystemFlags.osc_detune[SystemFlags.menu_line_counter-1] += 1;
				else
					SystemFlags.osc_detune[SystemFlags.menu_line_counter-1] = 0;
				ChangeOscillatorDetune(SystemFlags.menu_line_counter-1);
				DisplayDetune();
			}
		}
		break;
	case	MENU_STATE_OSC_VOLUME:
		current_screen = (ScreenTypeDef *)&Osc_VolumeMenu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_OSC_VOLUME_LINE);
			SystemFlags.buttons_flag &= ~( BUTTON_TACT0 | BUTTON_TACT1);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == MENU_MAX_OSC_VOLUME_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_OSC_TOP;
				change_menu((ScreenTypeDef *)&Osc_Menu_Screen,current_screen);
				DisplayADSR();
			}
			else
			{
				if ( SystemFlags.osc_volume[SystemFlags.menu_line_counter-1] < 10 )
					SystemFlags.osc_volume[SystemFlags.menu_line_counter-1] += 1;
				else
					SystemFlags.osc_volume[SystemFlags.menu_line_counter-1] = 0;
				ChangeOscillatorVolume(SystemFlags.menu_line_counter-1);
				DisplayVolume();
			}
		}
		break;
	case	MENU_STATE_ADSR:
		current_screen = (ScreenTypeDef *)&Osc_ADSRMenu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_OSC_ADSR_LINE);
			SystemFlags.buttons_flag &= ~( BUTTON_TACT0 | BUTTON_TACT1);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == MENU_MAX_OSC_ADSR_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_OSC_TOP;
				change_menu((ScreenTypeDef *)&Osc_Menu_Screen,current_screen);
			}
			else
			{
				if ( SystemFlags.menu_line_counter == 1)
				{
					if ( SystemFlags.Atime < SystemFlags.Dtime )
						SystemFlags.Atime++;
					else
						SystemFlags.Atime=1;
					DisplayADSR();
				}
				if ( SystemFlags.menu_line_counter == 2)
				{
					if ( SystemFlags.Dtime < SystemFlags.Rtime )
						SystemFlags.Dtime++;
					else
						SystemFlags.Dtime=SystemFlags.Atime;
					DisplayADSR();
				}
				if ( SystemFlags.menu_line_counter == 3)
				{
					SystemFlags.Sval--;
					if ( SystemFlags.Sval == 0 )
						SystemFlags.Sval= ADSR_H;
					DisplayADSR();
				}
				if ( SystemFlags.menu_line_counter == 4)
				{
					if ( SystemFlags.Rtime < ADSR_W )
						SystemFlags.Rtime++;
					else
						SystemFlags.Rtime=SystemFlags.Dtime;
					DisplayADSR();
				}
			}
		}
		break;

	case	MENU_STATE_AFX:
		current_screen = (ScreenTypeDef *)&AFX_Menu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_EFF_LINE);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == 1) // band
			{
				if (( SystemFlags.afx_flags & AFX_MOOG1) == AFX_MOOG1)
				{
					SystemFlags.afx_flags &= ~AFX_MOOG1;
					SystemFlags.afx_flags |= AFX_MOOG2;
				}
				else if (( SystemFlags.afx_flags & AFX_MOOG2) == AFX_MOOG2)
				{
					SystemFlags.afx_flags &= ~AFX_MOOG2;
					SystemFlags.afx_flags |= AFX_PHASER;
				}
				else if (( SystemFlags.afx_flags & AFX_PHASER) == AFX_PHASER)
				{
					SystemFlags.afx_flags &= ~AFX_PHASER;
					SystemFlags.afx_flags |= AFX_MOOG1;
				}
				Draw_Effects();
				Clear_VCF_data();
			}

			if ( SystemFlags.menu_line_counter == 2) // band
			{
				switch ( SystemFlags.vcf_flags & VCF_TYPE_MASK)
				{
				case	VCF_TYPE_LP :
					SystemFlags.vcf_flags &= ~VCF_TYPE_MASK;
					SystemFlags.vcf_flags |= VCF_TYPE_BP;
					break;
				case	VCF_TYPE_BP :
					SystemFlags.vcf_flags &= ~VCF_TYPE_MASK;
					SystemFlags.vcf_flags |= VCF_TYPE_HP;
					break;
				case	VCF_TYPE_HP :
					SystemFlags.vcf_flags &= ~VCF_TYPE_MASK;
					SystemFlags.vcf_flags |= VCF_TYPE_LP;
					break;
				default :
					SystemFlags.vcf_flags &= ~VCF_TYPE_MASK;
					SystemFlags.vcf_flags |= VCF_TYPE_LP;
					break;
				}
				Draw_Filter_Params();
			}
			if ( SystemFlags.menu_line_counter == 3) // control
			{
				switch ( SystemFlags.vcf_flags & VCF_CONTROL_MASK)
				{
				case	VCF_CONTROL_POT :
					SystemFlags.vcf_flags &= ~VCF_CONTROL_MASK;
					SystemFlags.vcf_flags |= VCF_CONTROL_MIDI;
					break;
				case	VCF_CONTROL_MIDI :
					SystemFlags.vcf_flags &= ~VCF_CONTROL_MASK;
					SystemFlags.vcf_flags |= VCF_CONTROL_CV;
					break;
				case	VCF_CONTROL_CV :
					SystemFlags.vcf_flags &= ~VCF_CONTROL_MASK;
					SystemFlags.vcf_flags |= VCF_CONTROL_POT;
					break;
				default :
					SystemFlags.vcf_flags &= ~VCF_CONTROL_MASK;
					SystemFlags.vcf_flags |= VCF_CONTROL_MIDI;
					break;
				}
				Draw_Filter_Params();
			}
			if ( SystemFlags.menu_line_counter == 4) // enable
			{
				if (( SystemFlags.vcf_flags & VCF_ENABLED) == VCF_ENABLED)
				{
					SystemFlags.vcf_flags &= ~VCF_ENABLED;
				}
				else
				{
					SystemFlags.vcf_flags |= VCF_ENABLED;
				}
				Draw_Filter_Params();
				Draw_Effects();
				View_Sequence();
			}
			if ( SystemFlags.menu_line_counter == MENU_MAX_EFF_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_TOP;
				change_menu((ScreenTypeDef *)&Main_Menu_Screen,current_screen);
			}
		}
		break;


	case	MENU_STATE_DELAY:
		current_screen = (ScreenTypeDef *)&Delay_Menu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_DLY_LINE);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == 1)
			{
				if (SystemFlags.delay_value_from_prog < DELAY_MAX_VALUE )
					SystemFlags.delay_value_from_prog += DELAY_DELTA_VALUE;
				View_Delay_Val();
			}
			if ( SystemFlags.menu_line_counter == 2)
			{
				if (SystemFlags.delay_value_from_prog != 0 )
					SystemFlags.delay_value_from_prog -= DELAY_DELTA_VALUE;
				View_Delay_Val();
			}
			if ( SystemFlags.menu_line_counter == 3)
			{
				switch(SystemFlags.delay_flags & DLY_TYPE_MASK )
				{
				case	DLY_MIXER_FLANGER_POT :
					SystemFlags.delay_flags &= ~DLY_TYPE_MASK;
					SystemFlags.delay_flags |= DLY_MIXER_REVERB_POT;
					break;
				case	DLY_MIXER_REVERB_POT :
					SystemFlags.delay_flags &= ~DLY_TYPE_MASK;
					SystemFlags.delay_flags |= DLY_MIXER_FLANGER_MIDI;
					break;
				case	DLY_MIXER_FLANGER_MIDI :
					SystemFlags.delay_flags &= ~DLY_TYPE_MASK;
					SystemFlags.delay_flags |= DLY_MIXER_REVERB_MIDI;
					break;
				case	DLY_MIXER_REVERB_MIDI :
					SystemFlags.delay_flags &= ~DLY_TYPE_MASK;
					SystemFlags.delay_flags |= DLY_MIXER_FLANGER_POT;
					break;
				default :
					SystemFlags.delay_flags &= ~DLY_TYPE_MASK;
					SystemFlags.delay_flags |= DLY_MIXER_FLANGER_POT;
					break;
				}
				View_Delay();
				View_Delay_Val();
			}
			if ( SystemFlags.menu_line_counter == 4)
			{
				if (( SystemFlags.delay_flags & DLY_ENABLED) == DLY_ENABLED)
					SystemFlags.delay_flags &= ~DLY_ENABLED;
				else
					SystemFlags.delay_flags |= DLY_ENABLED;
				View_Delay();
				View_Sequence();
			}
			if ( SystemFlags.menu_line_counter == MENU_MAX_DLY_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_TOP;
				change_menu((ScreenTypeDef *)&Main_Menu_Screen,current_screen);
			}
		}
		break;
	case	MENU_STATE_SYSTEM:
		current_screen = (ScreenTypeDef *)&System_Menu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_SYSTEM_LINE);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == 1)
			{
				if (( SystemFlags.oscillator_flags & OSC_SRCFLAG) == OSC_SRCFLAG)
				{
					SystemFlags.oscillator_flags &= ~OSC_SRCFLAG;
					ST7735_WriteString(LINE_X_INPUT_MIDI, LINE_Y_INPUT, "CV  ", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
				}
				else
				{
					SystemFlags.oscillator_flags |= OSC_SRCFLAG;
					ST7735_WriteString(LINE_X_INPUT_MIDI, LINE_Y_INPUT, "MIDI", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
				}
			}
			if ( SystemFlags.menu_line_counter == 2)
			{
				if (( SystemFlags.control_flags & CONTROL_VCA) == CONTROL_VCA)
					SystemFlags.control_flags &= ~CONTROL_VCA;
				else
					SystemFlags.control_flags |= CONTROL_VCA;
				view_am();
			}
			if ( SystemFlags.menu_line_counter == 3)
			{
				if (( SystemFlags.control_flags & CONTROL_FM) == CONTROL_FM)
					SystemFlags.control_flags &= ~CONTROL_FM;
				else
					SystemFlags.control_flags |= CONTROL_FM;
				view_fm();
			}

			if ( SystemFlags.menu_line_counter == 4)
			{
				if (( SystemFlags.control_flags & CONTROL_OSC_VCF_DLY) == CONTROL_OSC_VCF_DLY)
				{
					SystemFlags.control_flags &= ~CONTROL_OSC_VCF_DLY;
				}
				else
				{
					SystemFlags.control_flags |= CONTROL_OSC_VCF_DLY;
				}
				View_Sequence();
			}
			if ( SystemFlags.menu_line_counter == 5)
			{
				SystemFlags.menu_state = MENU_STATE_PGM;
				SystemFlags.program_number = 0;
				change_menu((ScreenTypeDef *)&System_PGM_Menu_Screen,current_screen);
				char tmp_buf[4];
				sprintf(tmp_buf,"%d",SystemFlags.program_number);
				ST7735_WriteString(LINE2_X+4, LINE2_Y, tmp_buf, Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
			}
			if ( SystemFlags.menu_line_counter == MENU_MAX_SYSTEM_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_TOP;
				change_menu((ScreenTypeDef *)&Main_Menu_Screen,current_screen);
			}
		}
		break;
	case	MENU_STATE_PGM:
		current_screen = (ScreenTypeDef *)&System_PGM_Menu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_PGM_LINE);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == 1)
			{
				SystemFlags.program_number++;
				SystemFlags.program_number &= (NUM_PROGRAMS_MAX-1);
				char tmp_buf[4];
				sprintf(tmp_buf,"%d",SystemFlags.program_number);
				ST7735_WriteString(LINE2_X+4, LINE2_Y, tmp_buf, Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
			}
			if ( SystemFlags.menu_line_counter == 2)
			{
				if ( ee24_load_program(SystemFlags.program_number) != 0)
					ST7735_WriteString(LINE2_X, LINE2_Y, "FAIL", Font_7x10, ST7735_RED, ST7735_BLACK);
				else
				{
					SystemFlags.menu_state = MENU_STATE_SYSTEM;
					MenusDrawStatus();
					Draw_Waveform(0 , SystemFlags.osc_waves[0] );
					Draw_Waveform(1 , SystemFlags.osc_waves[1] );
					Draw_Waveform(2 , SystemFlags.osc_waves[2] );
					Draw_Waveform(3 , SystemFlags.osc_waves[3] );
					View_Sequence();
					SystemFlags.menu_state = MENU_STATE_TOP;
					change_menu((ScreenTypeDef *)&Main_Menu_Screen,current_screen);
				}
			}
			if ( SystemFlags.menu_line_counter == 3)
			{
				ee24_store_program(SystemFlags.program_number);
				SystemFlags.menu_state = MENU_STATE_TOP;
				change_menu((ScreenTypeDef *)&Main_Menu_Screen,current_screen);
			}

			if ( SystemFlags.menu_line_counter == MENU_MAX_PGM_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_SYSTEM;
				change_menu((ScreenTypeDef *)&System_Menu_Screen,current_screen);
			}
		}
		break;
		/*
	case	MENU_STATE_STOREPGM:
		current_screen = (ScreenTypeDef *)&System_StorePGM_Menu_Screen;
		if ((( SystemFlags.buttons_flag & BUTTON_TACT0) == BUTTON_TACT0) || (( SystemFlags.buttons_flag & BUTTON_TACT1) == BUTTON_TACT1))
		{
			old_line_number = SystemFlags.menu_line_counter;
			move_menu(current_screen,old_line_number,SystemFlags.menu_line_counter,MENU_MAX_STOREPGM_LINE);
		}
		if (( SystemFlags.buttons_flag & BUTTON_TACT2) == BUTTON_TACT2)
		{
			if ( SystemFlags.menu_line_counter == 1)
			{
				SystemFlags.program_number++;
				SystemFlags.program_number &= (NUM_PROGRAMS_MAX-1);
				char tmp_buf[4];
				sprintf(tmp_buf,"%d",SystemFlags.program_number);
				ST7735_WriteString(LINE2_X+4, LINE2_Y, tmp_buf, Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
			}
			if ( SystemFlags.menu_line_counter == 2)
			{
				ee24_store_program(SystemFlags.program_number);
				SystemFlags.menu_state = MENU_STATE_SYSTEM;
				change_menu((ScreenTypeDef *)&System_Menu_Screen,current_screen);
			}
			if ( SystemFlags.menu_line_counter == MENU_MAX_STOREPGM_LINE)
			{
				SystemFlags.menu_state = MENU_STATE_SYSTEM;
				change_menu((ScreenTypeDef *)&System_Menu_Screen,current_screen);
			}
		}
		break;
		*/
	}
	SystemFlags.buttons_flag &= ~(BUTTON_TACT0 | BUTTON_TACT1 | BUTTON_TACT2);

}

void bB101_Print_Tuner(void)
{
char 	tmp_buf[5];
int		value;
	value = (int ) ((TUNER_CONTROL  - 2048 )/ 20);
	if ( value > 98 )
		value = 98;
	if ( value < -98 )
		value = -98;
	sprintf(tmp_buf,"%d%% ",value);
	ST7735_WriteString(LINE_X_TUNER, LINE_Y_TUNER, tmp_buf, Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
}

void MenusDrawEmptyStatus(void)
{
uint8_t	i;

	for(i=0;i<4;i++)
		Draw_Waveform(i,SystemFlags.osc_waves[i]);
	ST7735_WriteString(LINE_X_FILTER,LINE_Y_FILTER,"LP", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
	if (( SystemFlags.oscillator_flags & OSC_SRCFLAG) == OSC_SRCFLAG)
	{
		ST7735_WriteString(LINE_X_INPUT_MIDI, LINE_Y_INPUT, "MIDI", Font_7x10, ST7735_DARKGREEN, ST7735_BLACK);
	}

	View_Sequence();
	View_Delay();
	View_Delay_Val();
	Draw_Filter_Params();
	Draw_Effects();
	view_am();
	view_fm();
}

void MenusDrawStatus(void)
{
	bB101_Print_Tuner();
}

