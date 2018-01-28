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

#ifndef _OUTPUT_DEVICE_H_
#define _OUTPUT_DEVICE_H_

#include "common.h"

#ifdef _WIN32
#   include <windows.h>
#   include "dsound.h"
#else
#   include <alsa/asoundlib.h>
#endif

SW_BEGIN_HEADER()

struct SWOutputDevice {
	uint32_t bufferHalfSize;
#ifdef _WIN32
	IDirectSound8 * dsound;
	IDirectSoundBuffer8 * buffer;
	IDirectSoundNotify * notify;
	HANDLE points[2];
#else
	snd_pcm_t * playbackDevice;
	int frameCount;
#endif
};

/*
========================
swDeviceSendData
    - Sends data to output device
    - Size of input data must be bufferHalfSize (defined when device is created)
    - Format of the input data is: interleaved 16-bit stereo - LL RR LL RR LL RR - where L,R - 1 byte
    - This function is for internal use only!
========================
*/
void swDeviceSendData(SWOutputDevice * dev, short * inputData);

SW_END_HEADER()

#endif
