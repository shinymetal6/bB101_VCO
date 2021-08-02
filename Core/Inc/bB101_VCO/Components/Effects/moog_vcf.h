/*
 * moog_vcf.h
 *
 *  Created on: Jun 30, 2021
 *      Author: fil
 */

#ifndef INC_BB101_VCO_COMPONENTS_EFFECTS_MOOG_VCF_H_
#define INC_BB101_VCO_COMPONENTS_EFFECTS_MOOG_VCF_H_

typedef struct _VCFParametersTypeDef
{
	/* Used for moog1 filter */
	float 	f, p, q;
	float 	b0, b1, b2, b3, b4;
	/* Used for moog2 filter */
	float 	out1,out2,out3,out4;
	float 	in1,in2,in3,in4;
	float	filterCutoff;
	float	filterResonance;
	float	filter2_constant01;

}VCFParametersTypeDef;

extern	VCFParametersTypeDef	VCFParameters;

extern	void Moog_VCF( uint16_t *buffer_in,uint16_t *buffer_out );
extern	void Set_Filter1_Coefficients(void);
extern	void Set_Filter2_Coefficients(void);
extern	void Clear_VCF_data(void);

#endif /* INC_BB101_VCO_COMPONENTS_EFFECTS_MOOG_VCF_H_ */
