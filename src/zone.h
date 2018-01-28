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

#ifndef _ZONE_H_
#define _ZONE_H_

#include "common.h"

typedef enum SWZoneType {
	SW_ZT_BOX,
	SW_ZT_SPHERE
} SWZoneType;

struct SWZone {
	SWZoneType type;
	SWVec3 halfSize; /* half size of a box */
	float radius; /* radius of a sphere */
	SWVec3 position;
	SWEffect * effect;
	/* intrusive list */
	struct SWZone * next; 
    struct SWZone * prev;
};

SWZone * swZoneCreate(SWZoneType zoneType, SWEffect * effect);
void swZoneFree(SWZone * zone);
void swZoneUpdate(SWZone * zone);
void swZoneSetPosition(SWZone * zone, const SWVec3 * position);
SWVec3 * swZoneGetPosition(SWZone * zone, SWVec3 * pos);
void swZoneSetRadius(SWZone * zone, float radius);
float swZoneGetRadius(SWZone * zone);
SWZoneType swZoneGetType(SWZone * zone);
void swZoneSetEffect(SWZone * zone, SWEffect * effect);
SWEffect * swZoneGetEffect(SWZone * zone);
void swZoneSetHalfSize(SWZone * zone, const SWVec3 * halfSize);
SWVec3 * swZoneGetHalfSize(SWZone * zone, SWVec3 * half_size);

#endif