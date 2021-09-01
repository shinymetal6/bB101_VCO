/*
 * moog_vcf.c
 *
 *  Created on: Jun 30, 2021
 *      Author: fil
 */

#include "main.h"

VCFParametersTypeDef	VCFParameters;

void Set_Filter1_Coefficients(void)
{
/*    Set coefficients given frequency & resonance [0.0...1.0] */
	VCFParameters.q = 1.0F - VCFParameters.filterCutoff;
	VCFParameters.p = VCFParameters.filterCutoff + 0.8F * VCFParameters.filterCutoff * VCFParameters.q;
	VCFParameters.f = VCFParameters.p + VCFParameters.p - 1.0F;
	VCFParameters.q = VCFParameters.filterResonance * (1.0F + 0.5f * VCFParameters.q * (1.0F - VCFParameters.q + 5.6F * VCFParameters.q * VCFParameters.q));
}

void Set_Filter2_Coefficients(void)
{
	VCFParameters.filterCutoff *= 1.16F;
	VCFParameters.filterResonance *= (4.0F * (1.0F - 0.15F * VCFParameters.filterCutoff * VCFParameters.filterCutoff));
	VCFParameters.filter2_constant01 = VCFParameters.filterCutoff*VCFParameters.filterCutoff*VCFParameters.filterCutoff*VCFParameters.filterCutoff*0.35013F;;
}

void Clear_VCF_data(void)
{
	VCFParameters.b0=VCFParameters.b1=VCFParameters.b2=VCFParameters.b3=VCFParameters.b4=0;
	VCFParameters.in1=VCFParameters.in2=VCFParameters.in3=VCFParameters.in4=0;
	VCFParameters.out1=VCFParameters.out2=VCFParameters.out3=VCFParameters.out4=0;
}

float process_filter1(float in)
{
float t1, t2;              //temporary buffers
	in -= VCFParameters.q * VCFParameters.b4;                          //feedback
	t1 = VCFParameters.b1;
	VCFParameters.b1 = (in + VCFParameters.b0) * VCFParameters.p - VCFParameters.b1 * VCFParameters.f;
	t2 = VCFParameters.b2;
	VCFParameters.b2 = (VCFParameters.b1 + t1) * VCFParameters.p - VCFParameters.b2 * VCFParameters.f;
	t1 = VCFParameters.b3;
	VCFParameters.b3 = (VCFParameters.b2 + t2) * VCFParameters.p - VCFParameters.b3 * VCFParameters.f;
	VCFParameters.b4 = (VCFParameters.b3 + t1) * VCFParameters.p - VCFParameters.b4 * VCFParameters.f;
	VCFParameters.b4 = VCFParameters.b4 - VCFParameters.b4 * VCFParameters.b4 * VCFParameters.b4 * 0.166667F;    //clipping
	VCFParameters.b0 = in;
	if (( SystemFlags.vcf_flags & VCF_TYPE_BP ) == VCF_TYPE_BP)
		return 3.0f * (VCFParameters.b3 - VCFParameters.b4);
	if (( SystemFlags.vcf_flags & VCF_TYPE_LP ) == VCF_TYPE_LP) // in -= q*(b4 - in)
		return VCFParameters.b4;
	if (( SystemFlags.vcf_flags & VCF_TYPE_HP ) == VCF_TYPE_HP)
		return (in - 3.0f * (VCFParameters.b3 - VCFParameters.b4))-VCFParameters.b4;
	return 0;
	// Lowpass  output:  b4
	// Highpass output:  in - b4; or (in - 3.0f * (b3 - b4))-b4;
	// Bandpass output:  3.0f * (b3 - b4);
}

float process_filter2(float input)
{
	input -= VCFParameters.out4 * VCFParameters.filterResonance;
	input *= VCFParameters.filter2_constant01; //0.35013 * (f*f)*(f*f);
	VCFParameters.out1 = input + 0.3F * VCFParameters.in1 + (1.0F - VCFParameters.filterCutoff) * VCFParameters.out1; // Pole 1
	VCFParameters.in1 = input;
	VCFParameters.out2 = VCFParameters.out1 + 0.3F * VCFParameters.in2 + (1 - VCFParameters.filterCutoff) * VCFParameters.out2; // Pole 2
	VCFParameters.in2  = VCFParameters.out1;
	VCFParameters.out3 = VCFParameters.out2 + 0.3F * VCFParameters.in3 + (1 - VCFParameters.filterCutoff) * VCFParameters.out3; // Pole 3
	VCFParameters.in3  = VCFParameters.out2;
	VCFParameters.out4 = VCFParameters.out3 + 0.3F * VCFParameters.in4 + (1 - VCFParameters.filterCutoff) * VCFParameters.out4; // Pole 4
	VCFParameters.in4 = VCFParameters.out3;
	if (( SystemFlags.vcf_flags & VCF_TYPE_BP ) == VCF_TYPE_BP)
		return 3.0f * (VCFParameters.out3 - VCFParameters.out4);
	if (( SystemFlags.vcf_flags & VCF_TYPE_LP ) == VCF_TYPE_LP)
		return VCFParameters.out4;
	if (( SystemFlags.vcf_flags & VCF_TYPE_HP ) == VCF_TYPE_HP)
		return input - VCFParameters.out4;
	return 0;
}

void Moog_VCF( uint16_t *buffer_out,uint16_t *buffer_in )
{
uint16_t	i;

	if (( SystemFlags.vcf_flags & VCF_ENABLED ) == 0)
	{
		for ( i=0;i<HALF_NUMBER_OF_AUDIO_SAMPLES;i++)
			*buffer_out++ = *buffer_in++;
	}
	else
	{
		if ((( SystemFlags.afx_flags & AFX_MOOG1) == AFX_MOOG1) || (( SystemFlags.afx_flags & AFX_MOOG2) == AFX_MOOG2))
		{
			if ( isnan(VCFParameters.b0) )
				VCFParameters.b0=VCFParameters.b1=VCFParameters.b2=VCFParameters.b3=VCFParameters.b4=0;
		}

		for ( i=0;i<HALF_NUMBER_OF_AUDIO_SAMPLES;i++)
		{
			if (( SystemFlags.afx_flags & AFX_MOOG1) == AFX_MOOG1)
				*buffer_out++ = FLOAT_2_NORMALIZED_INT(process_filter1(INT_2_NORMALIZED_FLOAT(*buffer_in++)));
			else if (( SystemFlags.afx_flags & AFX_MOOG2) == AFX_MOOG2)
				*buffer_out++ = FLOAT_2_NORMALIZED_INT(process_filter2(INT_2_NORMALIZED_FLOAT(*buffer_in++)));
			else if (( SystemFlags.afx_flags & AFX_RESF1) == AFX_RESF1)
				*buffer_out++ = (uint16_t )SVF_FilterSample(0,(float )buffer_in[i] );
			else if (( SystemFlags.afx_flags & AFX_RESF1) == AFX_RESF2)
				*buffer_out++ = (uint16_t )SVF_FilterSample(1,(float )buffer_in[i] );
			else if (( SystemFlags.afx_flags & AFX_PHASER) == AFX_PHASER)
				*buffer_out++ = (uint16_t )Phaser_compute((float )buffer_in[i] );
			else
				*buffer_out++ = *buffer_in++;
		}
	}
}
