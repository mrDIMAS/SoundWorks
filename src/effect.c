/* The MIT License (MIT)

Copyright (c) 2015-2017 Stepanov Dmitriy aka mrDIMAS

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "list.h"
#include "memory.h"
#include "effect.h"

SWEffect * g_effects;

/*
const float SW_EARLY_LINE_LENGTH[4] = { 0.0015f, 0.0045f, 0.0135f, 0.0405f };
const float SW_ALLPASS_LINE_LENGTH[4] = { 0.0151f, 0.0167f, 0.0183f, 0.0200f };
const float SW_LATE_LINE_LENGTH[4] = { 0.0211f, 0.0311f, 0.0461f, 0.0680f };*/
#define SW_TIME(t) 3 * t
const float SW_EARLY_LINE_LENGTH[4] = { SW_TIME(0.0015f), SW_TIME(0.0045f), SW_TIME(0.0135f), SW_TIME(0.0405f) };
const float SW_ALLPASS_LINE_LENGTH[4] = { SW_TIME(0.0151f), SW_TIME(0.0167f), SW_TIME(0.0183f), SW_TIME(0.0200f) };
const float SW_LATE_LINE_LENGTH[4] = { SW_TIME(0.0211f), SW_TIME(0.0311f), SW_TIME(0.0461f), SW_TIME(0.0680f) };
#undef SW_TIME

SWEffect * swEffectGetList() {
	return g_effects;
}

/*=========================================*/
SWEffect * swEffectNext(SWEffect * effect) {
	return effect->next;
}

/*=========================================*/
SWBool swEffectIsValid(SWEffect * effect) {
	SWEffect * e;

	for(e = g_effects; e; e = e->next) {
		if(e == effect) return SW_TRUE;
	}

	return SW_FALSE;
}

/*=========================================*/
float swEffectCalculateDecay(float timeLength, float decayTime) {
	return pow(0.001, timeLength / decayTime);
}

/*=========================================*/
void swReverbSetDecayTime(SWEffect * effect, float time) {
	int i;

	if(time < 0.01f) {
		time = 0.01f;
	}

	effect->reverb.decayTime = time;

	for(i = 0; i < 4; i++) {
		effect->reverb.late.decay[i] = swEffectCalculateDecay(SW_LATE_LINE_LENGTH[i], time);
		effect->reverb.early.decay[i] = swEffectCalculateDecay(SW_EARLY_LINE_LENGTH[i], 0.03);
		effect->reverb.late.allpass[i].gain = swEffectCalculateDecay(SW_ALLPASS_LINE_LENGTH[i], time);
	}

}

/*=========================================*/
float swReverbGetDecayTime(SWEffect * effect) {
	return effect->reverb.decayTime;
}

/*=========================================*/
float SW_Effect_CalcLateMixCoeff(float diffusion) {
	float n = 1.73205; /* sqrt( 3 ) */
	float t = diffusion * 1.04719f; /* diffusion * atan( sqrt( 3 ) ) */

	return (sin(t) / n) / cos(t);
}

/*=========================================*/
void SW_Reverb_Setup(SWEffect * effect) {
	memset(&effect->reverb, 0, sizeof(effect->reverb));

	/* decorellator */
	swDelayLineCreate(&effect->reverb.decorellator.delay, 0.1f);
	effect->reverb.decorellator.tap[0] = -swConvertTimeToSampleNum(0.02f);
	effect->reverb.decorellator.tap[1] = swConvertTimeToSampleNum(0.07);
	effect->reverb.decorellator.tap[2] = swConvertTimeToSampleNum(0.04);
	effect->reverb.decorellator.tap[3] = -swConvertTimeToSampleNum(0.1);

	/* early */
	effect->reverb.early.gain = 0.15f;

	swDelayLineCreate(&effect->reverb.early.inputDelay, 0.05f);
	swOnepoleCreate(&effect->reverb.early.inputLowpass, 0.98f);
	swDelayLineCreate(&effect->reverb.early.delay[0], SW_EARLY_LINE_LENGTH[0]);
	swDelayLineCreate(&effect->reverb.early.delay[1], SW_EARLY_LINE_LENGTH[1]);
	swDelayLineCreate(&effect->reverb.early.delay[2], SW_EARLY_LINE_LENGTH[2]);
	swDelayLineCreate(&effect->reverb.early.delay[3], SW_EARLY_LINE_LENGTH[3]);

	/* late */
	effect->reverb.late.mix = SW_Effect_CalcLateMixCoeff(0.2);
	effect->reverb.late.gain = 0.5f;

	swDelayLineCreate(&effect->reverb.late.inputDelay, 0.075f);
	swOnepoleCreate(&effect->reverb.late.inputLowpass, 0.9f);

	swAllpassCreate(&effect->reverb.late.allpass[0], SW_ALLPASS_LINE_LENGTH[0]);
	swAllpassCreate(&effect->reverb.late.allpass[1], SW_ALLPASS_LINE_LENGTH[1]);
	swAllpassCreate(&effect->reverb.late.allpass[2], SW_ALLPASS_LINE_LENGTH[2]);
	swAllpassCreate(&effect->reverb.late.allpass[3], SW_ALLPASS_LINE_LENGTH[3]);

	swDelayLineCreate(&effect->reverb.late.delay[0], SW_LATE_LINE_LENGTH[0]);
	swDelayLineCreate(&effect->reverb.late.delay[1], SW_LATE_LINE_LENGTH[1]);
	swDelayLineCreate(&effect->reverb.late.delay[2], SW_LATE_LINE_LENGTH[2]);
	swDelayLineCreate(&effect->reverb.late.delay[3], SW_LATE_LINE_LENGTH[3]);

	swOnepoleCreate(&effect->reverb.late.lowpass[0], 0.9f);
	swOnepoleCreate(&effect->reverb.late.lowpass[1], 0.9f);
	swOnepoleCreate(&effect->reverb.late.lowpass[2], 0.9f);
	swOnepoleCreate(&effect->reverb.late.lowpass[3], 0.9f);

	swReverbSetDecayTime(effect, 0.8f);
}

/*=========================================*/
void SW_Reverb_Free(SWEffect * effect) {
	/* decorellator */
	swDelayLineFree(&effect->reverb.decorellator.delay);

	/* early */
	swDelayLineFree(&effect->reverb.early.inputDelay);
	swDelayLineFree(&effect->reverb.early.delay[0]);
	swDelayLineFree(&effect->reverb.early.delay[1]);
	swDelayLineFree(&effect->reverb.early.delay[2]);
	swDelayLineFree(&effect->reverb.early.delay[3]);

	/* late */
	swDelayLineFree(&effect->reverb.late.inputDelay);
	swDelayLineFree(&effect->reverb.late.delay[0]);
	swDelayLineFree(&effect->reverb.late.delay[1]);
	swDelayLineFree(&effect->reverb.late.delay[2]);
	swDelayLineFree(&effect->reverb.late.delay[3]);

	swAllpassFree(&effect->reverb.late.allpass[0]);
	swAllpassFree(&effect->reverb.late.allpass[1]);
	swAllpassFree(&effect->reverb.late.allpass[2]);
	swAllpassFree(&effect->reverb.late.allpass[3]);
}

/*=========================================*/
float swEffectExtractAverageSample(SWEffect * effect) {
	float average = 0.0f;
	if(effect->inputSampleCount > 0) {
		average = effect->accumulationSample / effect->inputSampleCount;
	}
	effect->accumulationSample = 0.0f;
	effect->inputSampleCount = 0.0f;
	return average;
}

/*=========================================*/
void swEffectAddDrySample(SWEffect * effect, float sample) {
	effect->accumulationSample += sample;
	effect->inputSampleCount += 1.0f;
}

/*=========================================*/
void swReverbSetLateDiffusion(SWEffect * effect, float diffusion)  {
	float feed;
	if(diffusion > 0.45) {
		diffusion = 0.45f;
	}
	if(diffusion < 0.005f) {
		diffusion = 0.005f;
	}
	feed = 0.5f * diffusion * diffusion;
	effect->reverb.late.allpass[0].feed = feed;
	effect->reverb.late.allpass[1].feed = feed;
	effect->reverb.late.allpass[2].feed = feed;
	effect->reverb.late.allpass[3].feed = feed;

	effect->reverb.diffusion = diffusion;
	effect->reverb.late.mix = SW_Effect_CalcLateMixCoeff(diffusion);
}

/*=========================================*/
float swReverbGetLateDiffusion(SWEffect * effect) {
	return effect->reverb.diffusion;
}

/*=========================================*/
void swReverbProcess(SWEffect * effect, float * outLeft, float * outRight) {
	float vol;
	float feed[4];
	float earlyOut[4] = { 0 };
	float lateOut[4], f[4], d[4];
	float edOut[4];
	float h;

	float sample = swEffectExtractAverageSample(effect);

	float earlyFiltered = swOnepoleFeed(&effect->reverb.early.inputLowpass, sample);
	float lateFiltered = swOnepoleFeed(&effect->reverb.late.inputLowpass, sample);

	float earlyInput = swDelayLineFeed(&effect->reverb.early.inputDelay,earlyFiltered);
	float lateInput = swDelayLineFeed(&effect->reverb.late.inputDelay, lateFiltered);

	/* early */
	edOut[0] = effect->reverb.early.delay[0].last * effect->reverb.early.decay[0];
	edOut[1] = effect->reverb.early.delay[1].last * effect->reverb.early.decay[1];
	edOut[2] = effect->reverb.early.delay[2].last * effect->reverb.early.decay[2];
	edOut[3] = effect->reverb.early.delay[3].last * effect->reverb.early.decay[3];

	vol = (edOut[0] + edOut[1] + edOut[2] + edOut[3]) * 0.5f + earlyInput;

	feed[0] = vol - edOut[0];
	feed[1] = vol - edOut[1];
	feed[2] = vol - edOut[2];
	feed[3] = vol - edOut[3];

	swDelayLineFeed(&effect->reverb.early.delay[0], feed[0]);
	swDelayLineFeed(&effect->reverb.early.delay[1], feed[1]);
	swDelayLineFeed(&effect->reverb.early.delay[2], feed[2]);
	swDelayLineFeed(&effect->reverb.early.delay[3], feed[3]);

	earlyOut[0] = effect->reverb.early.gain * feed[0];
	earlyOut[1] = effect->reverb.early.gain * feed[1];
	earlyOut[2] = effect->reverb.early.gain * feed[2];
	earlyOut[3] = effect->reverb.early.gain * feed[3];

	/* late */
	swDelayLineFeed(&effect->reverb.decorellator.delay, lateInput);

	f[0] = swDelayLineTapOut(&effect->reverb.decorellator.delay, effect->reverb.decorellator.tap[0]);
	f[1] = swDelayLineTapOut(&effect->reverb.decorellator.delay, effect->reverb.decorellator.tap[1]);
	f[2] = swDelayLineTapOut(&effect->reverb.decorellator.delay, effect->reverb.decorellator.tap[2]);
	f[3] = swDelayLineTapOut(&effect->reverb.decorellator.delay, effect->reverb.decorellator.tap[3]);

	f[0] += effect->reverb.late.delay[0].last * effect->reverb.late.decay[0];
	f[1] += effect->reverb.late.delay[1].last * effect->reverb.late.decay[1];
	f[2] += effect->reverb.late.delay[2].last * effect->reverb.late.decay[2];
	f[3] += effect->reverb.late.delay[3].last * effect->reverb.late.decay[3];

	d[0] = swOnepoleFeed(&effect->reverb.late.lowpass[2], f[2]);
	d[1] = swOnepoleFeed(&effect->reverb.late.lowpass[0], f[0]);
	d[2] = swOnepoleFeed(&effect->reverb.late.lowpass[3], f[3]);
	d[3] = swOnepoleFeed(&effect->reverb.late.lowpass[1], f[1]);

	d[0] = swAllpassFeed(&effect->reverb.late.allpass[0], d[0]);
	d[1] = swAllpassFeed(&effect->reverb.late.allpass[1], d[1]);
	d[2] = swAllpassFeed(&effect->reverb.late.allpass[2], d[2]);
	d[3] = swAllpassFeed(&effect->reverb.late.allpass[3], d[3]);

	f[0] = d[0] + effect->reverb.late.mix * (d[1] - d[2] + d[3]);
	f[1] = d[1] + effect->reverb.late.mix * (-d[0] + d[2] + d[3]);
	f[2] = d[2] + effect->reverb.late.mix * (d[0] - d[1] + d[3]);
	f[3] = d[3] + effect->reverb.late.mix * (-d[0] - d[1] - d[2]);

	lateOut[0] = effect->reverb.late.gain * f[0];
	lateOut[1] = effect->reverb.late.gain * f[1];
	lateOut[2] = effect->reverb.late.gain * f[2];
	lateOut[3] = effect->reverb.late.gain * f[3];

	swDelayLineFeed(&effect->reverb.late.delay[0], f[0]);
	swDelayLineFeed(&effect->reverb.late.delay[1], f[1]);
	swDelayLineFeed(&effect->reverb.late.delay[2], f[2]);
	swDelayLineFeed(&effect->reverb.late.delay[3], f[3]);

	/* mix */
	h = lateOut[0] + lateOut[1] + lateOut[2] + lateOut[3] + earlyOut[0] + earlyOut[1] + earlyOut[2] + earlyOut[3];

	*outLeft = h * effect->gain;
	*outRight = h * effect->gain;
}

/*=========================================*/
void swEffectProcessDummy(SWEffect * effect, float * outLeft, float * outRight) {
	float sample = swEffectExtractAverageSample(effect);
	*outLeft = sample * effect->gain;
	*outRight = sample * effect->gain;
}

/*=========================================*/
SWEffect * swEffectCreate(EEffectType type) {
	SWEffect * effect = SW_NEW(SWEffect);
	LIST_ADD(g_effects, effect);

	effect->type = type;
	effect->gain = 0.3162f;
	effect->enabled = SW_TRUE;

	if(type == SW_EFFECT_REVERB) {
		effect->Process = swReverbProcess;
		SW_Reverb_Setup(effect);
	} else {
		effect->Process = swEffectProcessDummy;
		swLogWrite("WARNING: Effect created with undefined type! Using dummy processing function!");
	}

	return effect;
}

/*=========================================*/
void swEffectSetGain(SWEffect * effect, float gain) {
	if(swEffectIsValid(effect)) {
		effect->gain = gain;
	}
}

/*=========================================*/
float swEffectGetGain(SWEffect * effect) {
	if(swEffectIsValid(effect)) {
		return effect->gain;
	}
	return 0.0f;
}

/*=========================================*/
void swEffectFree(SWEffect * effect) {
	if(effect->type == SW_EFFECT_REVERB) {
		SW_Reverb_Free(effect);
	}
	LIST_ERASE(g_effects, effect);
	swFree(effect);
}

/*=========================================*/
void swEffectFreeAll() {
	if(g_effects) {
		SWEffect * effect = g_effects;
		while(effect) {
			SWEffect * next = effect->next;
			swEffectFree(effect);
			effect = next;
		}
	}
}

/*=========================================*/
void swEffectSetEnabled(SWEffect * effect, SWBool state) {
	effect->enabled = state;
}

/*=========================================*/
SWBool swEffectIsEnabled(SWEffect * effect) {
	return effect->enabled;
}
