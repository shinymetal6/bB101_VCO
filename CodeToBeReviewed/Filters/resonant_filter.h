/*
 * resonant_filter.h
 *
 *  Created on: Sep 1, 2021
 *      Author: fil
 */

#ifndef INC_BB101_VCO_COMPONENTS_EFFECTS_RESONANT_FILTER_H_
#define INC_BB101_VCO_COMPONENTS_EFFECTS_RESONANT_FILTER_H_

#include <stdint.h>
#include <math.h>
//#include <errno.h>
//#include "CONSTANTS.h"
//#include "math_tools.h"

//----------------------------------------------------
//removed to free some cpu cycles
#define ENABLE_NONLINEAR_INTEGRATORS 	1
#define FILTER_GAIN_F 					0.88f
#define USE_SHAPER_NONLINEARITY 		0
#define MAX_FREQ						8000.f
#define MIN_FREQ						80.f

//----------------------------------------------------

enum filterTypeEnum
{
	FILTER_LP=VCF_TYPE_LP,
	FILTER_HP=VCF_TYPE_HP,
	FILTER_BP=VCF_TYPE_BP,
	FILTER_NOTCH,
	FILTER_PEAK
};
/*
#define	FILTER_LP		VCF_TYPE_LP
#define	FILTER_BP		VCF_TYPE_BP
#define	FILTER_HP		VCF_TYPE_HP
#define	FILTER_NOTCH	1
#define	FILTER_PEAK		2
*/

//----------------------------------------------------
typedef struct ResoFilterStruct
{
	uint8_t 	type; // filter type : LP, HP, BP...
	float 		f;	/**< cutoff */
	float 		g;  /**< embedded integrator gain (Fig 3.11), wc == wa*/
	float 		q;	/**< q value calculated from setReso()*/
	float 		s1;
	float 		s2;
	float 		drive;
#if ENABLE_NONLINEAR_INTEGRATORS
	float 		zi;	//input z^(-1)
#endif

#if USE_SHAPER_NONLINEARITY
	Distortion shaper;
#endif

} ResonantFilter;


//------------------------------------------------------------------------------------
void 	SVF_setReso(ResonantFilter* filter, float feedback);
//------------------------------------------------------------------------------------
void 	SVF_setDrive(ResonantFilter* filter, uint8_t drive);
//------------------------------------------------------------------------------------
void 	SVF_directSetFilterValue(ResonantFilter* filter, float val);
//------------------------------------------------------------------------------------
//void SVF_calcBlockZDF(ResonantFilter* filter, const uint8_t type, int16_t* buf, const uint8_t size);
//------------------------------------------------------------------------------------
void 	SVF_recalcFreq(ResonantFilter* filter);

float 	SVF_calcSample(ResonantFilter* filter, float in);
//------------------------------------------------------------------------------------
void 	SVF_init(void);

void 	Filter1Freq_set(uint8_t val);

void 	Filter1Res_set(uint8_t val);

void	Filter1Drive_set(uint8_t val);

void 	Filter1Type_set(uint8_t val);
/*-----------------------------------------------------------------------------------*/
void 	Filter2Freq_set(uint8_t val);

void 	Filter2Res_set(uint8_t val);

void	Filter2Drive_set(uint8_t val);

void 	Filter2Type_set(uint8_t val);

float SVF_FilterSample(uint32_t type, float in);


#endif /* INC_BB101_VCO_COMPONENTS_EFFECTS_RESONANT_FILTER_H_ */
