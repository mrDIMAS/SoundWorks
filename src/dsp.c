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

#include "dsp.h"

/*=========================================*/
uint32_t swNearestPowOfTwo(uint32_t num) {
	uint32_t power = 1;
	while(power < num) {
		power *= 2;
	}
	return power;
}

/*=========================================*/
uint32_t swConvertTimeToSampleNum(float time) {
	return time * SW_OUTPUT_DEVICE_SAMPLE_RATE;
}

/*=========================================*/
void swOnepoleSetPole(SWOnepole * o, float pole) {
	if(pole > 1.0f) {
		pole = 1.0f;
	}
	if(pole < -1.0f) {
		pole = -1.0f;
	}
	if(pole > 0.0f) {
		o->b0 = 1.0f - pole;
	} else {
		o->b0 = 1.0f + pole;
	}
	o->a1 = -pole;
}

/*=========================================*/
void swOnepoleCreate(SWOnepole * o, float pole) {
	o->last = 0.0f;
	swOnepoleSetPole(o, pole);
}

/*=========================================*/
float swOnepoleFeed(SWOnepole * o, float input) {
	o->last = o->b0 * input - o->a1 * o->last;
	return o->last;
}

/*=========================================*/
void swDelayLineCreate(SWDelayLine * dl, float length_sec) {
	dl->wrapMask = swNearestPowOfTwo(swConvertTimeToSampleNum(length_sec) + 1) - 1;
	dl->line = swCAlloc((dl->wrapMask + 1) * sizeof(float));
	dl->ptr = 0;
	dl->timeLength = length_sec;
	dl->last = 0.0f;
}

/*=========================================*/
float swDelayLineFeed(SWDelayLine * dl, float input) {
	uint32_t p = dl->ptr & dl->wrapMask;
	dl->last = dl->line[p];
	dl->line[p] = input;
	++dl->ptr;
	return dl->last;
}

/*=========================================*/
float swDelayLineTapOut(SWDelayLine * dl, int offset) {
	return dl->line[(dl->ptr + offset) & dl->wrapMask ];
}

/*=========================================*/
void swDelayLineFree(SWDelayLine * dl) {
	swFree(dl->line);
}

/*=========================================*/
void swAllpassCreate(SWAllpass * ap, float length_sec) {
	swDelayLineCreate(&ap->dl, length_sec);
	ap->gain = 1.0f;
	ap->feed = 0.5f;
}

/*=========================================*/
void swAllpassFree(SWAllpass * ap) {
	swDelayLineFree(&ap->dl);
}

/*=========================================*/
float swAllpassFeed(SWAllpass * ap, float input) {
	float out, feed;
	out = ap->dl.last;
	feed = ap->feed * input;
	swDelayLineFeed(&ap->dl, ap->feed * (out - feed) + input);
	return (ap->gain * out) - feed;
}

/*=========================================*/
void swVariableDelayLineCreate(SWVariableDelayLine * vdl, float fullLength) {
	vdl->wrapMask = swNearestPowOfTwo(swConvertTimeToSampleNum(fullLength) + 1) - 1;
	vdl->line = swCAlloc((vdl->wrapMask + 1) * sizeof(float));
	vdl->timeLength = fullLength;
	vdl->readPtr = 0;
	vdl->writePtr = 0;
}

/*=========================================*/
void swVariableDelayLineSetDelay(SWVariableDelayLine * vdl, float timeSec) {
	vdl->readPtr = (vdl->writePtr - swConvertTimeToSampleNum(timeSec)) & vdl->wrapMask;
}

/*=========================================*/
float swVariableDelayLineFeed(SWVariableDelayLine * vdl, float sample) {
	float out;
	vdl->line[ vdl->writePtr & vdl->wrapMask ] = sample;
	out = vdl->line[ vdl->readPtr & vdl->wrapMask ];
	++vdl->writePtr;
	++vdl->readPtr;
	return out;
}

/*=========================================*/
void swVariableDelayLineFree(SWVariableDelayLine * vdl) {
	swFree(vdl->line);
}
