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

#ifndef _SOUND_H_
#define _SOUND_H_

#include "buffer.h"
#include "effect.h"

SW_BEGIN_HEADER()

struct SWSound {    
    SWBuffer * buffer;
    float * bufferSamples;
    uint32_t bufferSampleCount;
    
    SWBool playing;
    SWBool looping;
    SWBool is3D;
    SWBool paused;
    float gain;
    float pitch;
    float readPtr;
    float pan;
    float leftGain;
    float rightGain;
    float sampleRateMultiplier;
    float currentSampleRate;  

    /* 3d properties */
    SWVec3 pos;
    float radius;
    float distanceGain;
    float effectDistanceGain;
    float effectRadius;
    SWVariableDelayLine vdlLeft;
    SWVariableDelayLine vdlRight;
    SWOnepole opLeft;
    SWOnepole opRight;
    
    SWEffect * effect;
    
    /* proper function automatically selected by number of channels in buffer */
    void (*GetSamples)( struct SWSound * snd, float * left, float * right );
	
	/* intrusive list */
    struct SWSound * next;
    struct SWSound * prev;
};

extern SWSound * g_sounds;

/*
========================
SW_Sound_Update
    - For internal use only
========================
*/
void swSoundUpdate( SWSound * snd );


SW_END_HEADER()

#endif