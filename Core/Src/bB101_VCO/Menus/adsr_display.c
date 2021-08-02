/*
 * adsr_display.c
 *
 *  Created on: Jul 26, 2021
 *      Author: fil
 */

#include "main.h"

/*
 *
 min val = 0
 max val = 0xfff
 */

void DisplayClearADSR(void)
{
	ST7735_FillRectangle(ADSR_X, ADSR_Y, ADSR_W, ADSR_H+2, ST7735_BLACK);
}

#define	ADSR_COLOR	ST7735_WHITE
/*
void DisplayADSR(void)
{
uint16_t	startx,starty,endx,endy;

	ST7735_FillRectangle(ADSR_X, ADSR_Y, ADSR_W, ADSR_H, ST7735_BLACK);

	startx = ADSR_X_ORIGIN;
	starty = ADSR_Y_ORIGIN;
	endx = ADSR_X_ORIGIN+SystemFlags.Atime;
	endy = ADSR_Y_ORIGIN-ADSR_H;

	if ( endx > ADSR_X+ADSR_W)
		endx = ADSR_X+ADSR_W-1;
	if (endy > ADSR_Y+ADSR_H)
		endy = ADSR_Y+ADSR_H-1;

	ST7735_DrawLine(startx,starty,endx,endy,ADSR_COLOR);

	startx = endx;
	starty = endy;
	endx = ADSR_X_ORIGIN+SystemFlags.Dtime;
	endy = ADSR_Y_ORIGIN-SystemFlags.Sval;

	ST7735_DrawLine(startx,starty,endx,endy,ADSR_COLOR);

	startx = endx;
	starty = endy;
	endx = (ADSR_X_ORIGIN+SystemFlags.Rtime);
	if ( endx > ADSR_X+ADSR_W)
		endx = ADSR_X+ADSR_W-1;
	endy = ADSR_Y_ORIGIN-ADSR_H;
	if (endy > ADSR_Y+ADSR_H)
		endy = ADSR_Y+ADSR_H-1;
	ST7735_DrawLine(startx,starty,endx,endy,ADSR_COLOR);

	startx = endx;
	starty = endy;
	endx = ADSR_X_ORIGIN + ADSR_W;
	endy = ADSR_Y_ORIGIN;
	if ( endx > ADSR_X+ADSR_W)
		endx = ADSR_X+ADSR_W-1;
	endy = ADSR_Y_ORIGIN-ADSR_H;
	if (endy > ADSR_Y+ADSR_H)
		endy = ADSR_Y+ADSR_H-1;
	ST7735_DrawLine(startx,starty,endx,endy,ADSR_COLOR);

}
*/

void DisplayADSR(void)
{
uint16_t	startx,starty,endx,endy;

	ST7735_FillRectangle(ADSR_X, ADSR_Y, ADSR_W, ADSR_H+1, ST7735_BLACK);

	startx = ADSR_X_ORIGIN;
	starty = ADSR_Y_ORIGIN;
	endx = ADSR_X_ORIGIN+SystemFlags.Atime;
	endy = ADSR_Y_ORIGIN-ADSR_H;

	if ( endx > ADSR_X+ADSR_W)
		endx = ADSR_X+ADSR_W-1;
	if (endy > ADSR_Y+ADSR_H)
		endy = ADSR_Y+ADSR_H-1;

	ST7735_DrawLine(startx,starty,endx,endy,ADSR_COLOR);

	startx = endx;
	starty = endy;
	endx = ADSR_X_ORIGIN+SystemFlags.Dtime;
	endy = ADSR_Y_ORIGIN-SystemFlags.Sval;

	if ( endx > ADSR_X+ADSR_W)
		endx = ADSR_X+ADSR_W-1;
	if (endy > ADSR_Y+ADSR_H)
		endy = ADSR_Y+ADSR_H-1;

	ST7735_DrawLine(startx,starty,endx,endy,ADSR_COLOR);

	startx = endx;
	starty = endy;
	endx = (ADSR_X_ORIGIN+SystemFlags.Rtime);

	if ( endx > ADSR_X+ADSR_W)
		endx = ADSR_X+ADSR_W-1;
	if (endy > ADSR_Y+ADSR_H)
		endy = ADSR_Y+ADSR_H-1;

	ST7735_DrawLine(startx,starty,endx,endy,ADSR_COLOR);

	startx = endx;
	starty = endy;
	endx = ADSR_X_ORIGIN + ADSR_W;
	endy = ADSR_Y_ORIGIN;

	if ( endx > ADSR_X+ADSR_W)
		endx = ADSR_X+ADSR_W-1;
	if (endy > ADSR_Y+ADSR_H)
		endy = ADSR_Y+ADSR_H-1;

	ST7735_DrawLine(startx,starty,endx,endy,ADSR_COLOR);

}
