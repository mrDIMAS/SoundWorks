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

#ifndef _DSP_H_
#define _DSP_H_

#include "common.h"

SW_BEGIN_HEADER()

typedef struct SWOnepole {
	float a1;
	float b0;
	float last;
} SWOnepole;

typedef struct SWDelayLine {
	float * line;
	float last;
	float timeLength;
	int32_t ptr;
	uint32_t wrapMask;
} SWDelayLine;

typedef struct SWVariableDelayLine {
	float * line;
	
	uint32_t readPtr;
	uint32_t writePtr;
	uint32_t wrapMask;
	
	/*
	int32_t readPtr;
	int32_t writePtr;*/
	
	int32_t sampleLength;
	float timeLength;
} SWVariableDelayLine;

typedef struct SWAllpass {
	SWDelayLine dl;
	float gain;
	float feed;
} SWAllpass;

uint32_t swNearestPowOfTwo(uint32_t num);
uint32_t swConvertTimeToSampleNum(float time);
void swOnepoleSetPole(SWOnepole * o, float pole);
void swOnepoleCreate(SWOnepole * o, float pole);
float swOnepoleFeed(SWOnepole * o, float input);
void swDelayLineCreate(SWDelayLine * dl, float length_sec);
float swDelayLineFeed(SWDelayLine * dl, float input);
float swDelayLineTapOut(SWDelayLine * dl, int offset);
void swDelayLineFree(SWDelayLine * dl);
void swAllpassCreate(SWAllpass * ap, float length_sec);
void swAllpassFree(SWAllpass * ap);
float swAllpassFeed(SWAllpass * ap, float input);
void swVariableDelayLineCreate(SWVariableDelayLine * vdl, float fullLength);
void swVariableDelayLineSetDelay(SWVariableDelayLine * vdl, float timeSec);
float swVariableDelayLineFeed(SWVariableDelayLine * vdl, float sample);
void swVariableDelayLineFree(SWVariableDelayLine * vdl);

SW_END_HEADER()

#endif
