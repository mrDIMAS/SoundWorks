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

#include "buffer.h"
#include "list.h"
#include "listener.h"
#include "dsp.h"
#include "effect.h"
#include "memory.h"
#include "mixer.h"
#include "sound.h"
#include <math.h>
#include "hrtf.h"

SWSound * g_sounds;

/* built-in tables to make simple hrtf by applying hf damping, when user not specify 'head' for listener */
static float hfDampingTableLeft[360] = {
	0.35f,  0.34f,  0.33f,   0.32f,   0.31f,   0.3f,    0.29f,   0.28f,   0.27f,   0.26f,   /* 10  deg */
	0.22f,  0.18f,  0.14f,   0.11f,   0.07f,   0.06f,   0.04f,   0.03f,   0.02f,   0.0f,    /* 20  deg */
	0.0f,   0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    /* 30  deg */
	0.0f,   0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    /* 40  deg */
	0.0f,   0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    /* 50  deg */
	0.1f,   0.11,   0.12f,   0.13f,   0.14f,   0.15f,   0.16f,   0.17f,   0.18f,   0.19f,   /* 60  deg */
	0.2f,   0.21,   0.22f,   0.23f,   0.24f,   0.25f,   0.26f,   0.27f,   0.28f,   0.29f,   /* 70  deg */
	0.3f,   0.31,   0.32f,   0.33f,   0.34f,   0.35f,   0.36f,   0.37f,   0.38f,   0.39f,   /* 80  deg */
	0.4f,   0.41,   0.42f,   0.43f,   0.44f,   0.45f,   0.46f,   0.47f,   0.48f,   0.49f,   /* 90  deg */
	0.5f,   0.51,   0.52f,   0.53f,   0.54f,   0.55f,   0.56f,   0.57f,   0.58f,   0.59f,   /* 100 deg */
	0.6f,   0.61,   0.62f,   0.63f,   0.64f,   0.65f,   0.66f,   0.67f,   0.68f,   0.69f,   /* 110 deg */
	0.7f,   0.71,   0.711f,  0.712f,  0.713f,  0.714f,  0.715f,  0.716f,  0.717f,  0.718f,  /* 120 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 130 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 140 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 150 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 160 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 170 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 180 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 190 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 200 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 210 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 220 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 230 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 240 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 250 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 260 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 270 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 280 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 290 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 300 deg */
	0.718f, 0.717f, 0.716f,  0.715f,  0.714f,  0.713f,  0.712f,  0.711f,  0.71f,   0.71f,   /* 310 deg */
	0.7f,   0.69f,  0.68f,   0.67f,   0.66f,   0.65f,   0.64f,   0.63f,   0.62f,   0.61f,   /* 320 deg */
	0.5f,   0.49f,  0.46f,   0.42f,   0.40f,   0.39f,   0.38f,   0.37f,   0.36f,   0.35f,   /* 330 deg */
	0.35f,  0.35f,  0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   /* 340 deg */
	0.35f,  0.35f,  0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   /* 350 deg */
	0.35f,  0.35f,  0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   /* 360 deg */
};

static float hfDampingTableRight[360] = {
	0.35f,  0.35f,  0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   /* 10 deg */
	0.35f,  0.35f,  0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   0.35f,   /* 20 deg */
	0.42f,  0.44f,  0.46f,   0.48f,   0.50f,   0.51f,   0.52f,   0.53f,   0.54f,   0.56f,   /* 30 deg */
	0.58f,  0.59f,  0.6f,    0.61f,   0.62f,   0.63f,   0.64f,   0.65f,   0.67f,   0.68f,   /* 40 deg */
	0.69f,  0.7f,   0.71f,   0.711f,  0.712f,  0.713f,  0.714f,  0.715f,  0.716f,  0.717f,  /* 50 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 60 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 70 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 80 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 90 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 100 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 110 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 120 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 130 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 140 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 150 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 160 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 170 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 180 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 190 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 200 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 210 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 220 deg */
	0.718f, 0.718f, 0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  0.718f,  /* 230 deg */
	0.718f, 0.717f, 0.716f,  0.715f,  0.714f,  0.713f,  0.712f,  0.711f,  0.71f,   0.7f,    /* 240  deg */
	0.69f,  0.68f,  0.67f,   0.66f,   0.65f,   0.64f,   0.63f,   0.62f,   0.61f,   0.6f,    /* 250  deg */
	0.59f,  0.58f,  0.57f,   0.56f,   0.55f,   0.54f,   0.53f,   0.52f,   0.51f,   0.5f,    /* 260  deg */
	0.49f,  0.48f,  0.47f,   0.46f,   0.45f,   0.44f,   0.43f,   0.42f,   0.41f,   0.4f,    /* 270  deg */
	0.39f,  0.38f,  0.37f,   0.36f,   0.35f,   0.34f,   0.33f,   0.32f,   0.31f,   0.3f,    /* 280  deg */
	0.29f,  0.28f,  0.27f,   0.26f,   0.25f,   0.24f,   0.23f,   0.22f,   0.21f,   0.2f,    /* 290  deg */
	0.19f,  0.18f,  0.17f,   0.16f,   0.15f,   0.14f,   0.13f,   0.12f,   0.11f,   0.1f,    /* 300  deg */
	0.0f,   0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    /* 310  deg */
	0.0f,   0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    /* 320  deg */
	0.0f,   0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    /* 330  deg */
	0.0f,   0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    /* 340  deg */
	0.0f,   0.01f,  0.02f,   0.04f,   0.06f,   0.08f,   0.11f,   0.14f,   0.18f,   0.22f,   /* 350  deg */
	0.26f,  0.27f,  0.28f,   0.29f,   0.3f,    0.31f,   0.32f,   0.33f,   0.34f,   0.35f,   /* 360  deg */
};

void swSoundGetSamplesMono(SWSound * snd, float * left, float * right);
void swSoundGetSamplesStereo(SWSound * snd, float * left, float * right);
void swSoundGetSamplesMonoConvert(SWSound * snd, float * left, float * right);

/*=========================================*/
float swCartesianToSphericalThetaDegrees(const SWVec3 * v) {
	return swRadToDeg(atan2(v->z, v->x));
}

/*=========================================*/
float swCartesianToSphericalPhiDegrees(const SWVec3 * v) {
	float vl = swVec3Len(v);
	if(vl > 0.0001f) {
		return swRadToDeg(acos(v->y / vl));
	}
	return 0.0f;
}

/*=========================================*/
void swSoundGetSphericalAngles(const SWVec3 * d, float * hAngle, float * vAngle) {
	*hAngle = 180 - (swCartesianToSphericalThetaDegrees(d) - swCartesianToSphericalThetaDegrees(&g_listener.look));
	if(*hAngle < 0) *hAngle += 360;
	*hAngle = fmod(*hAngle, 360);

	*vAngle = 90 - (swCartesianToSphericalPhiDegrees(d) + swCartesianToSphericalPhiDegrees(&g_listener.look));
	if(*vAngle < 0) *vAngle += 360;
	*vAngle = fmod(*vAngle, 180);
}

/*=========================================*/
void swSoundUpdate(SWSound * snd) {
	float leftGain, rightGain;
	float k, commonGain;

	snd->distanceGain = 1.0f;
	snd->effectDistanceGain = 1.0f;

	/* addition computations for spatial sounds */
	if(snd->is3D) {
		/* compute gain for the sound according to distance to the listener */
		SWVec3 d;
		float sqrD;
		float hAngle, vAngle;

		swVec3Sub(&d, &g_listener.pos, &snd->pos);

		sqrD = swVec3SqrLen(&d);
		snd->distanceGain = 1.0f / (1.0f + (sqrD / (snd->radius * snd->radius)));
		snd->effectDistanceGain = 1.0f / (1.0f + (sqrD / (snd->effectRadius * snd->effectRadius)));

		swSoundGetSphericalAngles(&d, &hAngle, &vAngle);

		if(g_listener.head) {
			int tblVAngle = (int)vAngle;
			int tblHAngle = (int)hAngle;

			float leftPhaseShift = g_listener.head->leftEarHrtf.phaseShift[tblVAngle][tblHAngle];
			float rightPhaseShift = g_listener.head->rightEarHrtf.phaseShift[tblVAngle][tblHAngle];

			float leftDamping = g_listener.head->leftEarHrtf.hfDamping[tblVAngle][tblHAngle];
			float rightDamping = g_listener.head->rightEarHrtf.hfDamping[tblVAngle][tblHAngle];

			leftGain = g_listener.head->leftEarHrtf.gain[tblVAngle][tblHAngle];
			rightGain = g_listener.head->rightEarHrtf.gain[tblVAngle][tblHAngle];

			swVariableDelayLineSetDelay(&snd->vdlLeft, rightPhaseShift);
			swVariableDelayLineSetDelay(&snd->vdlRight, leftPhaseShift);

			swOnepoleSetPole(&snd->opLeft, rightDamping);
			swOnepoleSetPole(&snd->opRight,leftDamping);
		} else {
			/* compute panning for 3d sound */
			if(sqrD < 0.0001f) {
				snd->pan = 0.0f;
			} else {
				d.y = 0.0f;
				swVec3Normalize(&d, &d);
				snd->pan = swVec3Dot(&g_listener.right, &d);
				if(snd->pan < -1.0f) {
					snd->pan = -1.0f;
				}
				if(snd->pan > 1.0f) {
					snd->pan = 1.0f;
				}
			}
			
			/* calculate pan for each channel separately */
			leftGain = 1.0f + snd->pan;
			rightGain = 1.0f - snd->pan;
			
			/* get damping from built-in table */
			float leftDamping = hfDampingTableLeft[(int)hAngle];
			float rightDamping = hfDampingTableRight[(int)hAngle];

			swVariableDelayLineSetDelay(&snd->vdlLeft, snd->vdlLeft.timeLength * (1.0f - snd->pan) * 0.5f);
			swVariableDelayLineSetDelay(&snd->vdlRight, snd->vdlRight.timeLength * (1.0f + snd->pan) * 0.5f);

			swOnepoleSetPole(&snd->opLeft, rightDamping);
			swOnepoleSetPole(&snd->opRight, leftDamping);
		}
	} else {		
		/* calculate panning */
		leftGain = 1.0f + snd->pan;
		rightGain = 1.0f - snd->pan;
	}

	commonGain = snd->gain * snd->distanceGain;

	/* calculate gain for each channel separately */
	snd->leftGain = commonGain * leftGain;
	snd->rightGain = commonGain * rightGain;
}

/*=========================================*/
void swSoundSelectSamplingFunc(SWSound * snd) {
	if(swBufferGetChannelCount(snd->buffer) == 2) {
		if(snd->is3D) {
			snd->GetSamples = swSoundGetSamplesMonoConvert;
		} else {
			snd->GetSamples = swSoundGetSamplesStereo;
		}
	} else {
		snd->GetSamples = swSoundGetSamplesMono;
	}
}

/*=========================================*/
SWBool swSoundIsValid(SWSound * snd) {
	SWSound * s;
	/* TODO: replace linear search with binary */
	for(s = g_sounds; s; s = s->next) {
		if(s == snd) return SW_TRUE;
	}
	return SW_FALSE;
}

/*=========================================*/
SWSound * swSoundGetList() {
	return g_sounds;
}

/*=========================================*/
SWSound * swSoundNext(SWSound * snd) {
	return snd->next;
}

/*=========================================*/
void swSoundSetPan(SWSound * snd, float pan) {
	if(swSoundIsValid(snd)) {
		if(pan < -1.0f) pan = -1.0f;
		if(pan > 1.0f) pan = 1.0f;
		snd->pan = pan;
	}
}

/*=========================================*/
float swSoundGetPan(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->pan;
	} else {
		return 0.0f;
	}
}

/*=========================================*/
/*
 Inreases readptr and wrap it if needed
 Also performs streaming - this is wrong! - streaming must not be done inside function that called inside of mixer
*/
static void swSoundIncreaseReadPtr(SWSound * snd) {
	snd->readPtr += snd->currentSampleRate;
	if(snd->readPtr >= snd->bufferSampleCount) {
		snd->readPtr = 0;
		if(swBufferGetType(snd->buffer) == SW_BUFFER_STREAM) {
			snd->bufferSamples = swBufferGetNextBlock(snd->buffer);
		} else {
			snd->playing = snd->looping;
		}
	}
}

/*=========================================
 There is only one input sample, because
 spatial effects can only be applied to
 the 3d sounds
=========================================*/
void swSoundAddSpatialEffects(SWSound * snd, float sample, float * left, float * right) {
	/* high quality 3d positioning effect is done using phase shift between channels */
	*left = swOnepoleFeed(&snd->opLeft, swVariableDelayLineFeed(&snd->vdlLeft, sample * snd->leftGain));
	*right = swOnepoleFeed(&snd->opRight, swVariableDelayLineFeed(&snd->vdlRight, sample * snd->rightGain));
	/* feed effect if present */
	if(snd->effect) {
		swEffectAddDrySample(snd->effect, sample * snd->effectDistanceGain);
	}
}

/*=========================================*/
void swSoundGetSamplesMono(SWSound * snd, float * left, float * right) {
	float sample = snd->bufferSamples[(int)snd->readPtr ];
	if(snd->is3D) {
		swSoundAddSpatialEffects(snd, sample, left, right);
	} else {
		*left = sample * snd->leftGain;
		*right = sample * snd->rightGain;
	}
	swSoundIncreaseReadPtr(snd);
}

/*=========================================*/
void swSoundGetSamplesStereo(SWSound * snd, float * left, float * right) {
	/* read_ptr must be always even when stereo buffer */
	uint32_t read_ptr = ((int)snd->readPtr) & ~1;
	*left = snd->bufferSamples[ read_ptr++ ] * snd->leftGain;
	*right = snd->bufferSamples[ read_ptr ] * snd->rightGain;
	swSoundIncreaseReadPtr(snd);
}

/*=========================================
 Used with stereo buffers for 3d sounds
 (performs conversion from stereo to mono
 on the fly)
=========================================*/
void swSoundGetSamplesMonoConvert(SWSound * snd, float * left, float * right) {
	/* read_ptr must be always even when stereo buffer */
	uint32_t read_ptr = ((int)snd->readPtr) & ~1;
	float sample = (snd->bufferSamples[ read_ptr ] + snd->bufferSamples[ read_ptr + 1 ]) * 0.5f;
	swSoundAddSpatialEffects(snd, sample, left, right);
	swSoundIncreaseReadPtr(snd);
}

/*=========================================*/
void swSoundSetPitch(SWSound * snd, float pitch) {
	if(swSoundIsValid(snd)) {
		if(pitch < 0.0f) pitch = 0.0f;
		snd->pitch = pitch;
		/* recompute sample rate */
		snd->currentSampleRate = snd->pitch * snd->sampleRateMultiplier * swBufferGetChannelCount(snd->buffer);
	}
}

/*=========================================*/
float swSoundGetPitch(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->pitch;
	} else {
		return 0.0f;
	}
}

/*=========================================*/
SWSound * swSoundCreate(SWBuffer * buf) {
	SWSound * snd = NULL;
	if(swBufferIsValid(buf)) {
		snd = SW_NEW(SWSound);
		LIST_ADD(g_sounds, snd);
		snd->buffer = buf;
		snd->bufferSamples = swBufferGetSamples(buf);
		snd->bufferSampleCount = swBufferGetSampleCount(buf);
		snd->gain = 1.0f;
		snd->is3D = SW_FALSE;
		snd->playing = SW_FALSE;
		snd->looping = SW_FALSE;
		snd->pan = 0;
		snd->paused = SW_FALSE;
		snd->distanceGain = 1.0f;
		snd->effectDistanceGain = 1.0f;
		snd->radius = 10.0f;
		snd->effectRadius = 10.0f;
		snd->leftGain = 1.0f;
		snd->rightGain = 1.0f;
		snd->sampleRateMultiplier = (float)swBufferGetFrequency(buf) / (float)SW_OUTPUT_DEVICE_SAMPLE_RATE;
		snd->currentSampleRate = 1.0f;
		swSoundSelectSamplingFunc(snd);
		swSoundSetPitch(snd, 1.0f);
		swVariableDelayLineCreate(&snd->vdlLeft,  0.0009666f);
		swVariableDelayLineCreate(&snd->vdlRight, 0.0009666f);
		swOnepoleCreate(&snd->opLeft, 0.09f);
		swOnepoleCreate(&snd->opRight, 0.09f);
	}
	return snd;
}

/*=========================================*/
void swSoundFree(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		swVariableDelayLineFree(&snd->vdlLeft);
		swVariableDelayLineFree(&snd->vdlRight);
		swBufferFree(snd->buffer);
		LIST_ERASE(g_sounds, snd);
		swFree(snd);
	}
}

/*=========================================*/
void swSoundFreeAll() {
	SWSound * snd = g_sounds;
	while(snd) {
		SWSound * next = snd->next;
		swSoundFree(snd);
		snd = next;
	}
}

/*=========================================*/
void swSoundSetLooping(SWSound * snd, SWBool state) {
	if(swSoundIsValid(snd)) {
		snd->looping = state;
	}
}

/*=========================================*/
SWBool swSoundIsLooping(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->looping;
	} else {
		return SW_FALSE;
	}
}

/*=========================================*/
void swSoundPause(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		snd->paused = SW_TRUE;
	}
}

/*=========================================*/
SWBool swSoundIsPaused(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->paused;
	}
	return SW_FALSE;
}

/*=========================================*/
void swSoundStop(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		snd->readPtr = 0;
		snd->playing = SW_FALSE;
	}
}

/*=========================================*/
void swSoundPlay(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		snd->paused = SW_FALSE;
		snd->playing = SW_TRUE;
	}
}

/*=========================================*/
SWBool swSoundIsPlaying(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->playing;
	} else {
		return SW_FALSE;
	}
}

/*=========================================*/
void swSoundSetPosition(SWSound * snd, const SWVec3 * position) {
	if(swSoundIsValid(snd)) {
		snd->pos = *position;
	}
}

/*=========================================*/
SWVec3 * swSoundGetPosition(SWSound * snd, SWVec3 * pos) {
	if(swSoundIsValid(snd)) {
		*pos = snd->pos;
	}
	return pos;
}

/*=========================================*/
void swSoundSet3D(SWSound * snd, SWBool state) {
	if(swSoundIsValid(snd)) {
		snd->is3D = state;
		swSoundSelectSamplingFunc(snd);
	}
}

/*=========================================*/
SWBool swSoundIs3D(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->is3D;
	}
	return SW_FALSE;
}

/*=========================================*/
void swSoundSetVolume(SWSound * snd, float vol) {
	if(swSoundIsValid(snd)) {
		if(vol < 0.0f) {
			vol = 0.0f;
		}
		snd->gain = vol;
	}
}

/*=========================================*/
float swSoundGetVolume(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->gain;
	}
	return 0.0f;
}

/*=========================================*/
void swSoundSetRadius(SWSound * snd, float radius) {
	if(swSoundIsValid(snd)) {
		snd->radius = radius;
	}
}

/*=========================================*/
float swSoundGetRadius(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->radius;
	}
	return 0.0f;
}

/*=========================================*/
void swSoundSetEffectRadius(SWSound * snd, float radius) {
	if(swSoundIsValid(snd)) {
		snd->effectRadius = radius;
	}
}

/*=========================================*/
float swSoundGetEffectRadius(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->effectRadius;
	}
	return 0.0f;
}

/*=========================================*/
void swSoundSetEffect(SWSound * snd, SWEffect * effect) {
	if(swSoundIsValid(snd)) {
		snd->effect = effect;
	}
}

/*=========================================*/
SWEffect * swSoundGetEffect(SWSound * snd) {
	if(swSoundIsValid(snd)) {
		return snd->effect;
	}
	return NULL;
}

/*=========================================*/
unsigned int swSoundGetPlaybackPositionSamples(SWSound * snd) {
	return snd->readPtr;
}

/*=========================================*/
float swSoundGetPlaybackPositionSeconds(SWSound * snd) {
	return snd->readPtr / (float)SW_OUTPUT_DEVICE_SAMPLE_RATE;
}

/*=========================================*/
unsigned int swSoundGetLengthSamples(SWSound * snd) {
	if(snd->buffer) {
		return snd->buffer->sampleCount;
	}
	return 0;
}

/*=========================================*/
float swSoundGetLengthSeconds(SWSound * snd) {
	if(snd->buffer) {
		return snd->buffer->sampleCount / (float)SW_OUTPUT_DEVICE_SAMPLE_RATE; /* could be wrong! */
	}
	return 0;
}
