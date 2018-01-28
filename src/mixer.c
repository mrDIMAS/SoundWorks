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

#include "device.h"
#include "sound.h"
#include <limits.h>
#include "memory.h"
#include "mixer.h"
#include "listener.h"
#include "hiresclock.h"

#ifdef _WIN32
#	include <windows.h>
#endif

static SWMixer * mixer;

/*=========================================*/
void * swMixerThread(void * param) {
	int n;
	static const float sampleMinLimit = SHRT_MIN;
	static const float sampleMaxLimit = SHRT_MAX;
	SWMixer * mixer = param;
	int sampleCount = mixer->dev->bufferHalfSize / sizeof(int16_t);
	mixer->threadActive = SW_TRUE;
	swLogWrite("Mixer thread started!");
	while(mixer->active) {
		if(!mixer->suspended) {
			double lastTime;

			/* thread-safety */
			pthread_mutex_lock(&mixer->mutex);
			
			/* begin performance measurements */
			lastTime = swHiResClock();
			
			/* begin mixing */
			for(n = 0; n < sampleCount;) {
				SWEffect * effect;
				SWSound * snd;
				float leftMixed = 0.0f;
				float rightMixed = 0.0f;
				float left, right;
				
				/* mix all playing sounds */
				snd = g_sounds;
				while(snd) {
					if(snd->playing && !snd->paused) {
						snd->GetSamples(snd, &left, &right);
						leftMixed += left;
						rightMixed += right;
					}
					snd = snd->next;
				}
				
				/* process effects */
				effect = g_effects;
				while(effect) {
					if(effect->enabled) {
						effect->Process(effect, &left, &right);
						leftMixed += left;
						rightMixed += right;
					}
					effect = effect->next;
				}
				
				/* clamp to [-32768;32767] */
				if(leftMixed > sampleMaxLimit) leftMixed = sampleMaxLimit;
				if(leftMixed < sampleMinLimit) leftMixed = sampleMinLimit;
				if(rightMixed > sampleMaxLimit) rightMixed = sampleMaxLimit;
				if(rightMixed < sampleMinLimit) rightMixed = sampleMinLimit;
				
				/* write to buffer */
				mixer->data[n++] = leftMixed;
				mixer->data[n++] = rightMixed;
			}
			
			/* end performance measurement */
			mixer->frameTimeSeconds = swHiResClock() - lastTime;
			
			pthread_mutex_unlock(&mixer->mutex);
			
			/* send mixed data to the output device */
			swDeviceSendData(mixer->dev, mixer->data);
		}
	}
	swLogWrite("Mixer thread exited successfully!");
	mixer->threadActive = SW_FALSE;	
	return NULL;
}

/*=========================================*/
SWBool swMixerCreate(SWOutputDevice * dev) {	
	if(!dev) {
		return SW_FALSE;
	}
	if(mixer) {
		return SW_FALSE;
	}
	
	mixer = SW_NEW(SWMixer);
	mixer->dev = dev;
	mixer->data = swAlloc(swGetHalfBufferSize(dev));
	mixer->suspended = SW_FALSE;
	mixer->locked = SW_FALSE;
	
	swListenerInit();

	return SW_TRUE;
}

/*=========================================*/
SWBool swMixerStart(void) {	
	pthread_t thread;
	
	if(mixer->active) {
		return SW_FALSE;
	}	
	
	mixer->active = SW_TRUE;
	pthread_mutex_init(&mixer->mutex, NULL);		
	pthread_create(&thread, NULL, swMixerThread, mixer);
	while(!mixer->threadActive) {
		printf("Avaiting mixer thread to start...\r");
	}
	
	return SW_TRUE;
}

/*=========================================*/
SWBool swMixerStop(void) {
	if(!mixer) {
		return SW_FALSE;
	}
	
	pthread_mutex_lock(&mixer->mutex);
	mixer->active = SW_FALSE;
	pthread_mutex_unlock(&mixer->mutex);
	while(mixer->threadActive) {
		printf("Avaiting mixer thread to stop...\r");
	}
	
	return SW_TRUE;
}

/*=========================================*/
SWBool swMixerSuspend(void) {
	if(!mixer) {
		return SW_FALSE;
	}
	mixer->suspended = SW_TRUE;
	return SW_TRUE;
}

/*=========================================*/
SWBool swMixerProcess(void) {
	if(!mixer) {
		return SW_FALSE;
	}
	mixer->suspended = SW_FALSE;
	return SW_TRUE;
}

/*=========================================*/
SWBool swMixerFree(void) {
	if(!mixer) {
		return SW_FALSE;
	}
	swMixerStop();
	swFree(mixer->data);
	swFree(mixer);
	mixer = NULL;
	return SW_TRUE;
}

/*=========================================*/
SWBool swMixerIsActive(void) {
	return mixer ? mixer->active : SW_FALSE;
}

/*=========================================*/
SWBool swUpdate(void) {
	SWSound * snd;

	if(!mixer) {
		return SW_FALSE;
	}
	
	snd = g_sounds;
	pthread_mutex_lock(&mixer->mutex);
	while(snd) {
		swSoundUpdate(snd);
		snd = snd->next;
	}
	pthread_mutex_unlock(&mixer->mutex);
	
	return SW_TRUE;
}

/*=========================================*/
float swMixerGetFrameTimeSeconds(void) {
	return mixer ? mixer->frameTimeSeconds : 0.0f;
}
