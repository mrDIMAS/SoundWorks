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

#include "zone.h"
#include "list.h"

SWZone * zones;

/*=========================================*/
SWZone * swZoneCreate(SWZoneType zoneType, SWEffect * effect) {
	SWZone * z = SW_NEW(SWZone);
	z->effect = effect;
	swVec3Set(&z->halfSize, 0.5f, 0.5f, 0.5f);
	z->radius = 0.5;	
	LIST_ADD(zones, z);	
	return z;
}

/*=========================================*/
void swZoneFree(SWZone * zone) {
	LIST_ERASE(zones, zone);
	swFree(zone);
}

/*=========================================*/
void swZoneUpdate(SWZone * zone) {
	
}

/*=========================================*/
void swZoneSetPosition(SWZone * zone, const SWVec3 * position) {
	zone->position = *position;
}

/*=========================================*/
SWVec3 * swZoneGetPosition(SWZone * zone, SWVec3 * pos) {
	*pos = zone->position;
	return pos;
}

/*=========================================*/
void swZoneSetRadius(SWZone * zone, float radius) {
	zone->radius = radius;
}

/*=========================================*/
float swZoneGetRadius(SWZone * zone) {
	return zone->radius;
}

/*=========================================*/
SWZoneType swZoneGetType(SWZone * zone) {
	return zone->type;
}

/*=========================================*/
void swZoneSetEffect(SWZone * zone, SWEffect * effect) {
	zone->effect = effect;
}

/*=========================================*/
SWEffect * swZoneGetEffect(SWZone * zone) {
	return zone->effect;
}

/*=========================================*/
void swZoneSetHalfSize(SWZone * zone, const SWVec3 * halfSize) {
	zone->halfSize = *halfSize;
}

/*=========================================*/
SWVec3 * swZoneGetHalfSize(SWZone * zone, SWVec3 * half_size) {
	*half_size = zone->halfSize;
	return half_size;
}