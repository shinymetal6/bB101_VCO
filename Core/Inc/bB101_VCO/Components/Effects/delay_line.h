/*
 * delay_line.h
 *
 *  Created on: Jul 1, 2021
 *      Author: fil
 */

#ifndef INC_BB101_VCO_COMPONENTS_EFFECTS_DELAY_LINE_H_
#define INC_BB101_VCO_COMPONENTS_EFFECTS_DELAY_LINE_H_

#define	DELAY_LINE_SIZE		(16384)
#define	DELAY_STEP 			(SAMPLE_FREQUENCY/DELAY_LINE_SIZE)
#define	DELAY_MAX_VALUE		370
#define	DELAY_DELTA_VALUE	10
extern	void DelayLine(uint16_t *buffer_out , uint16_t *buffer_in);
extern	void SetDelay(void);

#endif /* INC_BB101_VCO_COMPONENTS_EFFECTS_DELAY_LINE_H_ */
