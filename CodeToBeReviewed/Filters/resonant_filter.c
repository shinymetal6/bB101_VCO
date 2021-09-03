/*
 * resonant_filter.c
 *
 *  Created on: Sep 1, 2021
 *      Author: fil
 */


#include "main.h"

//-----------------------------------------------------------------------------------

#define FILTER_TYPES	5 /* Number of filter types */

//-----------------------------------------------------------------------------------

ResonantFilter SVFilter;
ResonantFilter SVFilter2;

float filterFreq;
float filterFreq2;

float fastTanh(float var)
{
	if(var < -1.95f)     return -1.0f;
	else if(var > 1.95f) return  1.0f;
	else return  4.15f*var/(4.29f+var*var);
}

float tanhXdX(float x)
{
	float a = x*x;
	// IIRC I got this as Pade-approx for tanh(sqrt(x))/sqrt(x)
	x = ((a + 105)*a + 945) / ((15*a + 420)*a + 945);
	return x;
}

float 	softClipTwo(float in)
{
	return in * tanhXdX(0.5*in);
}

float 	fastTan(float x)
{
	float A = -15*x+x*x*x;
	float B = 3*(-5+2*x*x);
	return A/B;
}

float	Lin2Exp(uint8_t val, float min, float max)
{
	return min * powf(max / min, val / MIDI_MAX);
}

void SVF_setReso(ResonantFilter* filter, float feedback)
{
	filter->q = 1-feedback;
	if(filter->q < 0.01f) filter->q = 0.01f;
}
//------------------------------------------------------------------------------------
void SVF_initialize(ResonantFilter* filter)
{
	filter->type = 1;

	filter->s1 = 0;
	filter->s2 = 0;

	filter->f = 0.20f;
	filter->q = 0.9f;

	filter->drive = 0.4f;

	SVF_directSetFilterValue(filter,0.25f);

#if ENABLE_NONLINEAR_INTEGRATORS
	filter->zi = 0;	//input z^(-1)
#endif

#if USE_SHAPER_NONLINEARITY
	setDistortionShape(&filter->shaper, 0);
#endif
}

//------------------------------------------------------------------------------------
void SVF_recalcFreq(ResonantFilter* filter)
{
	filter->g  = fastTan(_PI * filter->f );
#if USE_SHAPER_NONLINEARITY
	setDistortionShape(&filter->shaper, filter->drive);
#endif
}
//------------------------------------------------------------------------------------
void SVF_setDrive(ResonantFilter* filter,uint8_t drive) /* drive in 0 ... 127 */
{
#if USE_SHAPER_NONLINEARITY
	filter->drive = drive;
	setDistortionShape(&filter->shaper, filter->drive);
#else
	filter->drive =  /*  0.4f +  */ (drive/MIDI_MAX)*(drive/MIDI_MAX)*6; // the output can be muted (at 0)
#endif

}
//------------------------------------------------------------------------------------
void SVF_directSetFilterValue(ResonantFilter* filter, float val) // 0 < val < 1
{
	if (val > 0.95) val = 0.95f;
	filter->f = val*0.5f;
	filter->g  = fastTan(_PI * filter->f );

}
//=====================================================================================================
void SVF_init(void)
{
	SVF_initialize(&SVFilter);
	SVF_initialize(&SVFilter2);
}

//------------------------------------------------------------------------------------
void 	Filter1Freq_set(uint8_t val)
{
	filterFreq = Lin2Exp(val, MIN_FREQ, MAX_FREQ) / SAMPLE_FREQUENCY ;
	SVF_directSetFilterValue(&SVFilter, filterFreq);
}
//------------------------------------------------------------------------------------
void 	Filter1Res_set(uint8_t val)
{
	SVF_setReso(&SVFilter, val / MIDI_MAX);
}
//------------------------------------------------------------------------------------
void	Filter1Drive_set(uint8_t val)
{
	SVF_setDrive(&SVFilter, val);
}
//------------------------------------------------------------------------------------
/*
void 	Filter1Type_set(uint8_t val)
{
	SVFilter.type = (uint8_t)lrintf(FILTER_TYPES * val / MIDI_MAX);
}
*/

//------------------------------------------------------------------------------------
void 	Filter2Freq_set(uint8_t val)
{
	filterFreq2 = Lin2Exp(val, MIN_FREQ, MAX_FREQ) / SAMPLE_FREQUENCY ;
	SVF_directSetFilterValue(&SVFilter2, filterFreq2);
}
//------------------------------------------------------------------------------------
void 	Filter2Res_set(uint8_t val)
{
	SVF_setReso(&SVFilter2, val / MIDI_MAX);
}
//------------------------------------------------------------------------------------
void	Filter2Drive_set(uint8_t val)
{
	SVF_setDrive(&SVFilter2, val);
}
//------------------------------------------------------------------------------------
/*
void 	Filter2Type_set(uint8_t val)
{
	SVFilter2.type = (uint8_t)lrintf(FILTER_TYPES * val / MIDI_MAX);
}
*/


//------------------------------------------------------------------------------------

float SVF_calcSample(ResonantFilter* filter, float in)
{
	const float f 	= filter->g;
	//fix unstable filter for high f and r settings
	const float R 	= filter->f >= 0.4499f ? 1 : filter->q;
	const float ff 	= f*f;


#if USE_SHAPER_NONLINEARITY
	const float x = (buf[i]/((float)0x7fff));
#else
	const float x = softClipTwo( in * filter->drive);
#endif

#if ENABLE_NONLINEAR_INTEGRATORS
	// input with half sample delay, for non-linearities
	float ih = 0.5f * (x + filter->zi);
	filter->zi = x;
#endif

	// evaluate the non-linear gains
	/*
		You can trivially remove any saturator by setting the corresponding gain t0,...,t1 to 1. Also, you can simply scale any saturator (i.e. change clipping threshold) to 1/a*tanh(a*x) by writing
		double t1 = tanhXdX(a*s[0]);
	 */
#if ENABLE_NONLINEAR_INTEGRATORS
	const float scale = 0.5f;
	const float t0 = tanhXdX(scale* (ih - 2*R*filter->s1 - filter->s2 ) );
	const float t1 = tanhXdX(scale* (filter->s1 ) );
#else
	const float t0 = 1;
	const float t1 = 1;
#endif

	// g# the denominators for solutions of individual stages
	const float g0 = 1.f / (1.f + f*t0*2*R);

	const float s1 = filter->s1;
	const float s2 = filter->s2;

	// solve feedback
	const float f1 = ff*g0*t0*t1;
	float y1=(f1*x+s2+f*g0*t1*s1)/(f1+1);


	// solve the remaining stages with nonlinear gain
	const float xx = t0*(x - y1);
	const float y0 = (softClipTwo(s1) + f*xx)*g0;

	filter->s1   = softClipTwo(filter->s1) + 2*f*(xx - t0*2*R*y0);
	filter->s2   = (filter->s2)    + 2*f* t1*y0;

	float out;


	//switch(filter->type)
	switch(SystemFlags.vcf_flags & VCF_TYPE_MASK)
	{

//	case FILTER_LP:
	case VCF_TYPE_LP:
#if USE_SHAPER_NONLINEARITY

		buf[i] = FILTER_GAIN * fastTanh( distortion_calcSampleFloat(&filter->shaper, y1));
#else
		out = fastTanh(y1) ;
		//buf[i] = __SSAT(tmp,16);
#endif
		break;

	//case FILTER_HP:
	case VCF_TYPE_HP:
	{
		const float ugb = 2*R*y0;
		const float h = x - ugb - y1;
#if USE_SHAPER_NONLINEARITY

		buf[i] = FILTER_GAIN * distortion_calcSampleFloat(&filter->shaper, h);
#else
		out = h * FILTER_GAIN_F;
		//buf[i] = __SSAT(tmp,16);
#endif
	}
	break;

//	case FILTER_BP:
	case VCF_TYPE_BP:
#if USE_SHAPER_NONLINEARITY

		buf[i] = FILTER_GAIN * distortion_calcSampleFloat(&filter->shaper, y0);
#else
		out = y0 * FILTER_GAIN_F;
		//buf[i] = __SSAT(tmp,16);
#endif
		break;

#ifdef TO_BE_ADDED
	case FILTER_NOTCH:
	{
		const float ugb = 2*R*y0;
#if USE_SHAPER_NONLINEARITY

		buf[i] = FILTER_GAIN * distortion_calcSampleFloat(&filter->shaper, (x-ugb));
#else
		out = (x-ugb) * FILTER_GAIN_F;
		//buf[i] = __SSAT(tmp,16);
#endif
	}
	break;

	case FILTER_PEAK:
	{
		const float ugb = 2*R*y0;
		const float h = x - ugb - y1;
#if USE_SHAPER_NONLINEARITY

		buf[i] = FILTER_GAIN * distortion_calcSampleFloat(&filter->shaper, (y1-h));
#else
		out = (y1-h) * FILTER_GAIN_F;
		//buf[i] = __SSAT(tmp,16);
#endif
	}
	break;
#endif // TO_BE_ADDED
	default:
		out = in ;
		break;
	}

	out = (out > 1.0f) ? 1.0f : out; //clip too loud samples
	out = (out < -1.0f) ? -1.0f : out;

	return out;

}

float SVF_FilterSample(uint32_t type, float in)
{
	if ( type == 0 )
		return SVF_calcSample(&SVFilter, in);
	return SVF_calcSample(&SVFilter2, in);
}

//------------------------------------------------------------------------------------
