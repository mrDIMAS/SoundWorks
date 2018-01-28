#ifndef _HRTF_H_
#define _HRTF_H_


typedef struct SWHrtf {
	float hfDamping[180][360];
	float phaseShift[180][360];
	float gain[180][360];
} SWHrtf;

struct SWHead {
	SWHrtf leftEarHrtf;
	SWHrtf rightEarHrtf;
};

#endif