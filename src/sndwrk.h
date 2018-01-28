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

#ifndef _SOUNDWORKS_H_
#define _SOUNDWORKS_H_

#include <math.h>
#include <stdint.h>
#include "gcl.h"

typedef int SWBool;

#define SW_FALSE 0
#define SW_TRUE 1

typedef struct SWOutputDevice SWOutputDevice;
typedef struct SWSound SWSound;
typedef struct SWBuffer SWBuffer;
typedef struct SWEffect SWEffect;
typedef struct SWZone SWZone;
typedef struct SWHead SWHead;

typedef enum EEffectType {
    SW_EFFECT_REVERB = 0 /* Reverb is the basic effect */
    /* here can be added more effects in the future */
} EEffectType;

typedef enum SWBufferType {
    SW_BUFFER_FULL, /* load entire file in the memory */
    SW_BUFFER_STREAM /* load small piece of data in beginning, another and another when playing */
} SWBufferType;

#ifdef __cplusplus
extern "C" {
#endif

/*
#########################################
#      DEVICE MANAGEMENT FUNCTIONS      #
#########################################
*/
/*
========================
swDeviceCreate
    - Creates a new output device
    - 'bufferHalfSize' defines half of size of the buffer used by the output device
    - 'bufferHalfSize' - typical values is 8000 - 32000, smaller values can cause clicks in sound
========================
*/
SWOutputDevice * swDeviceCreate(uint32_t bufferHalfSize);

/*
========================
swDeviceDestroy
    - Destroys output device
    - Releases all allocated resources (buffers, sounds, effects, mixers and etc.)
========================
*/
void swDeviceDestroy(SWOutputDevice * dev);

/*
========================
swGetHalfBufferSize
    - Returns size of half of the buffer
========================
*/
uint32_t swGetHalfBufferSize(SWOutputDevice * dev);


/*
========================
swUpdate
    - Call this function to update state of sound engine
	- This function applies positions of sounds, listener properties and
	  etc. by locking mixer and updating gain coefficients of each channel
	- Call this function 12 to 16 times per second, no need to do it more
	  frequently because of performance reasons
========================
*/
SWBool swUpdate(void);

/*
#########################################
#     HEAD (HRTF-pair) FUNCTIONS        #
#########################################
*/
/* HRTF - head-related transfer function */

/*
========================
swHeadLoad
	- Head is pair of HRTF for each ear
    - Loads head from file generated using hrtfgen tool
	- Use returned pointer with swListenerSetHead to enable HRTF in your app
========================
*/
SWHead * swHeadLoad(const char * hrtfFile);

/*
========================
swHeadFree
========================
*/
void swHeadFree(SWHead * head);

/*
#########################################
#      LISTENER MANAGEMENT FUNCTIONS    #
#########################################
*/
/*
========================
swListenerSetPosition
    - Sets listener position in 3D world
========================
*/
void swListenerSetPosition(const SWVec3 * position);

/*
========================
swListenerGetPosition
    - Gets listener position in 3D world
========================
*/
SWVec3 * swListenerGetPosition(SWVec3 * position);

/*
========================
swListenerSetOrientation
    - This function sets orientation basis for lister, which allows to do spatialization
    - upVector is vector, that defines 'up' axis
    - lookVector is vector, that defines 'look' axis
    - By default these vectors are up = (0,1,0); look = (0,0,1)
	- Remember, that SoundWorks assumes that you use coordinate system with z looking towards
	  y - up, x - right
========================
*/
void swListenerSetOrientation(const SWVec3 * upVector, const SWVec3 * lookVector);

/*
========================
swListenerGetOrientaionUp
========================
*/
SWVec3 * swListenerGetOrientationUp(SWVec3 * up);

/*
========================
swListenerGetOrientationLook
========================
*/
SWVec3 * swListenerGetOrientationLook(SWVec3 * look);

/*
========================
swListenerGetOrientationRight
========================
*/
SWVec3 * swListenerGetOrientationRight(SWVec3 * right);

/*
========================
swListenerSetGain
    - Sets 'gain' of the listener, meaning of this value is the same as of volume of a sound
========================
*/
void swListenerSetGain(float gain);

/*
========================
swListenerGetGain
========================
*/
float swListenerGetGain(void);

void swListenerSetHead(SWHead * head);
SWHead * swListenerGetHead(void);

/*
#########################################
#       MIXER MANAGEMENT FUNCTIONS      #
#########################################
*/
/*
========================
swMixerCreate
========================
*/
SWBool swMixerCreate(SWOutputDevice * dev);

/*
========================
swMixerStart
	- Creates new thread for the mixing purposes
========================
*/
SWBool swMixerStart(void);

/*
========================
swMixerStop
	- Stops mixing thread, after calling this no data is sended to output device
========================
*/
SWBool swMixerStop(void);

/*
========================
swMixerSuspend
	- Pauses mixing thread
========================
*/
SWBool swMixerSuspend(void);

/*
========================
swMixerProcess
	- Continues mixing thread
========================
*/
SWBool swMixerProcess(void);

/*
========================
swMixerFree
	- Frees memory allocated for mixer
	- Also calls swMixerStop
========================
*/
SWBool swMixerFree(void);

/*
========================
swMixerIsActive
========================
*/
SWBool swMixerIsActive(void);

/*
========================
swMixerGetTime
	- Returns time in seconds, spent on mixing sounds into specified output device buffer size
========================
*/
float swMixerGetFrameTimeSeconds(void);

/*
#########################################
#       BUFFER MANAGEMENT FUNCTIONS     #
#########################################
*/
/*
========================
swBufferIsValid
    - Checks pointer for validity
    - Widely used in other functions SW_Buffer_XXX, so there is no need to check pointer manually
========================
*/
SWBool swBufferIsValid(SWBuffer * buf);

/*
========================
swBufferGetList
    - Gets first buffer in list, next element can be accessed through SW_Buffer_GetNext
========================
*/
SWBuffer * swBufferGetList(void);

/*
========================
swBufferGetNext
========================
*/
SWBuffer * swBufferGetNext(SWBuffer * buf);

/*
========================
swBufferCreateFromFile
    - Creates new buffer
	- Reference count is set to zero
	- Reference count increases by one when buffer is capturing by sound
	- Fills it with decoded data from 'fileName' file
	- 'type' == SW_BUFFER_STREAM useful for large files (music, ambient and etc.)
	- When 'type' == SW_BUFFER_STREAM, fills it only with small piece of decoded data
	- Supported formats: Vorbis Ogg (*.ogg)
========================
*/
SWBuffer * swBufferCreateFromFile(const char * fileName, SWBufferType type);

/*
========================
swBufferCreateFromMemory
    - Creates new buffer
	- 'ptr' should point to raw interleaved PCM data
	- Fills it with data from 'ptr'
	- 'channels' in current version should be one(mono), or two(stereo)
	- Only 16-bit samples supported
	- Only interleaved order supported (left-right, left-right, ...)
	- 'frequency' - sampling frequency
	- Streaming is not supported
========================
*/
SWBuffer * swBufferCreateFromMemory(char * ptr, int sizeBytes, int channels, int frequency);

/*
========================
swBufferFreeAll
    - Frees buffers, that are do not belong to any sound (refCount <= 0)
    - To be complete sure that all buffers are freed, just free all sounds before calling this function
========================
*/
void swBufferFreeAll();

/*
========================
swBufferFree
    - Buffer can only be freed, if no one owns it (refCount <= 0)
========================
*/
void swBufferFree(SWBuffer * buf);

/*
========================
swBufferGetSamples
    - As a side effect, increases refCounter of the buffer
    - This function is rather slow to use it in loop, to speed up things, save returned pointer and use it
    - To be sure, that returned pointer is valid, use SW_Buffer_IsBuffer to check validity
========================
*/
float * swBufferGetSamples(SWBuffer * buf);

/*
========================
swBufferGetSampleCount
    - This function is rather slow to use it in loop, to speed up things, save returned value and use it
========================
*/
uint32_t swBufferGetSampleCount(SWBuffer * buf);

/*
========================
swBufferGetChannelCount
    - 1 - mono
    - 2 - stereo
    - more channels can be supported in future
========================
*/
uint32_t swBufferGetChannelCount(SWBuffer * buf);

/*
========================
swBufferGetFrequency
    - Possible frequencies 11025 Hz, 22050 Hz, 44100 Hz, 48000 Hz, 96000 Hz
========================
*/
uint32_t swBufferGetFrequency(SWBuffer * buf);

/*
========================
swBufferGetType
========================
*/
SWBufferType swBufferGetType(SWBuffer * buf);



/*
#########################################
#       SOUND MANAGEMENT FUNCTIONS      #
#########################################
*/

/*
========================
swSoundIsValid
    - Checks pointer for validity
    - Widely used in other functions SW_Sound_XXX, so there is no need to check pointer manually
========================
*/
SWBool swSoundIsValid(SWSound * snd);

/*
========================
swSoundGetList
    - Gets first sound in list, next element can be accessed through Sound_Next
========================
*/
SWSound * swSoundGetList(void);

/*
========================
swSoundNext
    - Note that this function do not check 'snd' for validity
========================
*/
SWSound * swSoundNext(SWSound * snd);

/*
========================
swSoundCreate
    - Creates new sound from buffer
    - Buffer must be valid, otherwise NULL is returned
========================
*/
SWSound * swSoundCreate(SWBuffer * buf);

/*
========================
swSoundFree
    - Frees sound, pointer becomes invalid
========================
*/
void swSoundFree(SWSound * snd);

/*
========================
swSoundFreeAll
    - Frees all sounds, all user pointers become invalid
========================
*/
void swSoundFreeAll();

/*
========================
swSoundSetLooping
========================
*/
void swSoundSetLooping(SWSound * snd, SWBool state);

/*
========================
swSoundIsLooping
========================
*/
SWBool swSoundIsLooping(SWSound * snd);

/*
========================
swSoundGetPan
========================
*/
float swSoundGetPan(SWSound * snd);

/*
========================
swSoundSetPan
    - Pan must be in [-1.0; +1.0]
========================
*/
void swSoundSetPan(SWSound * snd, float pan);

/*
========================
swSoundGetPitch
========================
*/
float swSoundGetPitch(SWSound * snd);

/*
========================
swSoundSetPitch
    - Pitch must be > 0.0
    - Works only for 2D sounds
========================
*/
void swSoundSetPitch(SWSound * snd, float pitch);

/*
========================
swSoundPlay
    - No check for playing, immediate play
========================
*/
void swSoundPlay(SWSound * snd);

/*
========================
swSoundIsPlaying
========================
*/
SWBool swSoundIsPlaying(SWSound * snd);

/*
========================
swSoundPause
========================
*/
void swSoundPause(SWSound * snd);

/*
========================
swSoundIsPaused
========================
*/
SWBool swSoundIsPaused(SWSound * snd);

/*
========================
swSoundStop
========================
*/
void swSoundStop(SWSound * snd);

/*
========================
swSoundSetPosition
    - Sets position in 3D
========================
*/
void swSoundSetPosition(SWSound * snd, const SWVec3 * position);

/*
========================
swSoundGetPosition
    - Gets current sound position in 3D
========================
*/
SWVec3 * swSoundGetPosition(SWSound * snd, SWVec3 * pos);

/*
========================
swSoundSet3D
    - If 'SW_TRUE' passed, sound becomes '3D sound' with proper spatial effects
    - In this case, you can't control panning of the sound
========================
*/
void swSoundSet3D(SWSound * snd, SWBool state);

/*
========================
swSoundIs3D
========================
*/
SWBool swSoundIs3D(SWSound * snd);

/*
========================
swSoundSetVolume
========================
*/
void swSoundSetVolume(SWSound * snd, float vol);

/*
========================
swSoundGetVolume
========================
*/
float swSoundGetVolume(SWSound * snd);

/*
========================
swSoundSetRadius
    - Sets radius of imaginable sphere around the sound, which determines volume in which attenuation of the sound falling to zero after the border
    - Default law of attenuation is quadratic attenuation: 1 / ( 1 + ( d^2 / r^2 ))
    - Default radius is 10.0
========================
*/
void swSoundSetRadius(SWSound * snd, float radius);

/*
========================
swSoundGetRadius
========================
*/
float swSoundGetRadius(SWSound * snd);

/*
========================
swSoundSetEffectRadius
    - Controls decay of the sound intensity, which passed to effect
    - For more detailed description see SW_Sound_SetRadius
========================
*/
void swSoundSetEffectRadius(SWSound * snd, float radius);

/*
========================
swSoundGetEffectRadius
========================
*/
float swSoundGetEffectRadius(SWSound * snd);

/*
========================
swSoundSetEffect
    - Sets effect for the sound, effect defines additional path of processing for the signal
    - If effect is set, result of SW_Sound_GetSamples contains mixed 'wet' signals from spatial processing and effect processing
      'wet' - is the processed signal
========================
*/
void swSoundSetEffect(SWSound * snd, SWEffect * effect);

/*
========================
swSoundGetEffect
========================
*/
SWEffect * swSoundGetEffect(SWSound * snd);

/*
========================
swSoundGetPlaybackPositionSamples
	- Returns playback position in samples
========================
*/
unsigned int swSoundGetPlaybackPositionSamples(SWSound * snd);

/*
========================
swSoundGetPlaybackPositionSeconds
	- Returns playback position in seconds
========================
*/
float swSoundGetPlaybackPositionSeconds(SWSound * snd);

/*
========================
swSoundGetLengthSamples
	- Returns total length in samples
========================
*/
unsigned int swSoundGetLengthSamples(SWSound * snd);

/*
========================
swSoundGetLengthSeconds
	- Returns total length in seconds
========================
*/
float swSoundGetLengthSeconds(SWSound * snd);

/*
#########################################
#       EFFECT MANAGEMENT FUNCTIONS     #
#########################################
*/
/*
========================
swEffectGetList
========================
*/
SWEffect * swEffectGetList();

/*
========================
swEffectNext
========================
*/
SWEffect * swEffectNext(SWEffect * effect);

/*
========================
swEffectIsValid
========================
*/
SWBool swEffectIsValid(SWEffect * effect);

/*
========================
swEffectCreate
========================
*/
SWEffect * swEffectCreate(EEffectType type);

/*
========================
SW_Effect_AddDrySample
    - Adds dry sample to accumulation sample
    - When mixer calls SW_Effect_Process this accumulation sample is divided by count of input sources, thus it becomes average sample of all of the inputs
========================
*/
void swEffectAddDrySample(SWEffect * effect, float sample);

/*
========================
swEffectSetGain
========================
*/
void swEffectSetGain(SWEffect * effect, float gain);

/*
========================
swEffectGetGain
========================
*/
float swEffectGetGain(SWEffect * effect);

/*
========================
swEffectSetEnabled
========================
*/
void swEffectSetEnabled(SWEffect * effect, SWBool state);

/*
========================
swEffectIsEnabled
========================
*/
SWBool swEffectIsEnabled(SWEffect * effect);

/*
========================
swEffectFree
========================
*/
void swEffectFree(SWEffect * effect);

/*
========================
swEffectFreeAll
========================
*/
void swEffectFreeAll(void);

/*
========================
swReverbSetDecayTime
========================
*/
void swReverbSetDecayTime(SWEffect * effect, float time);

/*
========================
swReverbGetDecayTime
========================
*/
float swReverbGetDecayTime(SWEffect * effect);

/*
========================
swReverbSetLateDiffusion
========================
*/
void swReverbSetLateDiffusion(SWEffect * effect, float diffusion);

/*
========================
swReverbGetLateDiffusion
========================
*/
float swReverbGetLateDiffusion(SWEffect * effect);

#ifdef __cplusplus
}
#endif

#endif
