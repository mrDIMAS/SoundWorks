#include "common.h"
#include "hrtf.h"

/*=========================================*/
SWHead * swHeadLoad(const char * hrtfFile) {
	SWHead * head = SW_NEW(SWHead);
	FILE * f = fopen(hrtfFile, "r");

	while(!feof(f)) {
		int phi;
		int theta;
		float leftShift;
		float rightShift;
		float leftDamping;
		float rightDamping;
		float leftGain;
		float rightGain;

		fscanf(f, "%d %d %f %f %f %f %f %f", &phi, &theta,
		       &leftShift, &rightShift, &leftDamping, &rightDamping,
		       &leftGain, &rightGain);

		if(phi >= 180 || phi < 0 || theta >= 360 || theta < 0) {
			swHeadFree(head);
			return NULL;
		}

		head->leftEarHrtf.phaseShift[phi][theta] = leftShift;
		head->rightEarHrtf.phaseShift[phi][theta] = rightShift;
		
		head->leftEarHrtf.hfDamping[phi][theta] = leftDamping;
		head->rightEarHrtf.hfDamping[phi][theta] = rightDamping;
		
		head->leftEarHrtf.gain[phi][theta] = leftGain;
		head->rightEarHrtf.gain[phi][theta] = rightGain;
	}

	return head;
}

/*=========================================*/
void swHeadFree(SWHead * head) {
	swFree(head);
}
