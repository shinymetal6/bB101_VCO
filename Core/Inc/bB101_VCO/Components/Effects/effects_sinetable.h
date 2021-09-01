/*
 * effects_sinetable.h
 *
 *  Created on: Sep 1, 2021
 *      Author: fil
 */

#ifndef INC_BB101_VCO_COMPONENTS_EFFECTS_EFFECTS_SINETABLE_H_
#define INC_BB101_VCO_COMPONENTS_EFFECTS_EFFECTS_SINETABLE_H_

#define SINETABLE_SIZE 	1024
#define _2PI    		6.283185307F
#define ALPHA			(SINETABLE_SIZE/_2PI)

extern	const float effects_sinetable[SINETABLE_SIZE+1];

#endif /* INC_BB101_VCO_COMPONENTS_EFFECTS_EFFECTS_SINETABLE_H_ */
