/*
 * phaser.h
 *
 *  Created on: Sep 1, 2021
 *      Author: fil
 */

#ifndef INC_BB101_VCO_COMPONENTS_EFFECTS_PHASER_H_
#define INC_BB101_VCO_COMPONENTS_EFFECTS_PHASER_H_

#include <stdint.h>
//#include "CONSTANTS.h"
#include "effects_sinetable.h"

#define MIDI_MAX				127.f 	// floating max value
#define MIDI_MAXi				127		// integer max value
#define MIDI_MID_i				64		// integer mid value


extern	void PhaserInit(void);
extern	void Phaser( uint16_t *buffer_out,uint16_t *buffer_in);
extern	float process_Phaser(float xin);
extern	void Phaser_Rate_set(uint8_t val);
extern	void Phaser_Feedback_set(uint8_t val);
extern	void Phaser_Wet_set(uint8_t val);

#endif /* INC_BB101_VCO_COMPONENTS_EFFECTS_PHASER_H_ */
