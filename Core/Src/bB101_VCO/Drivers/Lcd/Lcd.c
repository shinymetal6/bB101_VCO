/*
 * LcdTask.c
 *
 *  Created on: Sep 9, 2019
 *      Author: fil
 */
#include "main.h"


#define VMARGIN 1
#define HMARGIN (12*7)-1
uint8_t	horizontal_line_space;
uint8_t	vertical_line_space;
uint8_t	current_highlight_line;
#define	NUMLINES	7


void LcdSetBrightness(uint16_t brightness)
{
	if ( brightness <= FULL_BRIGHTNESS)
	{
		BACKLIGHT_TIMER.Instance->CCR1 = brightness;
		SystemFlags.current_brightness = brightness;
	}
}

void LcdInit(void)
{
    LcdSetBrightness(ZERO_BRIGHTNESS);
	HAL_TIMEx_PWMN_Start(&BACKLIGHT_TIMER,TIM_CHANNEL_1);
    ST7735_Unselect();
	ST7735_Init();
    ST7735_FillScreen(ST7735_BLACK);
}

void LcdWrite11x18(Video *wr_struct)
{
	ST7735_WriteString(wr_struct->xpos, wr_struct->ypos, wr_struct->line, Font_11x18, wr_struct->fore_color, wr_struct->bkg_color);
}

void LcdWrite16x26(Video *wr_struct)
{
	ST7735_WriteString(wr_struct->xpos, wr_struct->ypos, wr_struct->line, Font_16x26, wr_struct->fore_color, wr_struct->bkg_color);
}

void LcdClearScreen(Video *wr_struct)
{
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_FillRectangle(wr_struct->xpos, wr_struct->ypos, strlen(wr_struct->line)*CHAR_W, CHAR_H, ST7735_BLACK);
}
