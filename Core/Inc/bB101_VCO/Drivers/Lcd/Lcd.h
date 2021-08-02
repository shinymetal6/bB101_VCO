/*
 * Lcd.h
 *
 *  Created on: Sep 9, 2019
 *      Author: fil
 */

#ifndef LCD_H_
#define LCD_H_

#include "main.h"

#include <stdio.h>

typedef struct {
    uint8_t xpos;
	uint8_t	ypos;
    char line[32];
    uint16_t fore_color;
	uint16_t bkg_color;
} Video;

#define	ZERO_BRIGHTNESS	0
#define	LOW_BRIGHTNESS	30
#define	HALF_BRIGHTNESS	500
#define	FULL_BRIGHTNESS	1000

#define CHAR_W 		11
#define CHAR_H 		18
#define MIN_TIME	10
#define MAX_TIME	180
#define TIME_STEP	10
#define START_TIME	30
#define STOPPED		0
#define RUNNING		1
#define DEBOUNCE_TIME 10
#define TIM14_COUNTS 10
#define TIM17_COUNTS 1000
#define	LIMIT		500
#define	STEP		10
#define	RAMP_STEP	20
#define	FINISHED_TIME_BANNER	200


extern	void LcdInit(void);
extern void LcdWrite11x18(Video *data);
extern void LcdWrite16x26(Video *wr_struct);
extern void LcdClearScreen(Video *data);
extern	void LcdSetBrightness(uint16_t brightness);

#endif /* LCD_H_ */
