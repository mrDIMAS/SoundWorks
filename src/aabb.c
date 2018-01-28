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

#include "aabb.h"
#include "util.h"
#include <math.h>
#include "float.h"

/*=========================================*/
SWAabb * swAabbSet(SWAabb * aabb, const SWVec3 * min, const SWVec3 * max) {
	aabb->min = *min;
	aabb->max = *max;
	return swAabbRecomputeCorners(aabb);
}

/*=========================================*/
SWAabb * swAabbRecomputeCorners(SWAabb * aabb) {
	swVec3Set(&aabb->corners[0], aabb->min.x, aabb->min.y, aabb->min.z);
	swVec3Set(&aabb->corners[1], aabb->min.x, aabb->min.y, aabb->max.z);
	swVec3Set(&aabb->corners[2], aabb->max.x, aabb->min.y, aabb->max.z);
	swVec3Set(&aabb->corners[3], aabb->max.x, aabb->min.y, aabb->min.z);
	swVec3Set(&aabb->corners[4], aabb->min.x, aabb->max.y, aabb->min.z);
	swVec3Set(&aabb->corners[5], aabb->min.x, aabb->max.y, aabb->max.z);
	swVec3Set(&aabb->corners[6], aabb->max.x, aabb->max.y, aabb->max.z);
	swVec3Set(&aabb->corners[7], aabb->max.x, aabb->max.y, aabb->min.z);
	return aabb;
} 

/*=========================================*/
int swAabbSphereIntersection(const SWAabb * aabb, const SWVec3 * aabb_offset, const SWVec3 * position, float radius) {
	float r2 = radius * radius;
	float dmin = 0;
	SWVec3 min = aabb->min, max = aabb->max;

	if(aabb_offset) {
		swVec3Add(&min, &min, aabb_offset);
		swVec3Add(&max, &max, aabb_offset);
	}

	if(position->x < min.x) {
		dmin += swSqr(position->x - min.x);
	} else if(position->x > max.x) {
		dmin += swSqr(position->x - max.x);
	}

	if(position->y < min.y) {
		dmin += swSqr(position->y - min.y);
	} else if(position->y > max.y) {
		dmin += swSqr(position->y - max.y);
	}

	if(position->z < min.z) {
		dmin += swSqr(position->z - min.z);
	} else if(position->z > max.z) {
		dmin += swSqr(position->z - max.z);
	}

	return dmin <= r2 ||
	       ((position->x >= min.x) && (position->x <= max.x) &&
	        (position->y >= min.y) && (position->y <= max.y) &&
	        (position->z >= min.z) && (position->z <= max.z));
}

/*=========================================*/
int swAabbIsContainsPoint(const SWAabb * aabb, const SWVec3 * point) {
	return point->x >= aabb->min.x && point->x <= aabb->max.x &&
	       point->y >= aabb->min.y && point->y <= aabb->max.y &&
	       point->z >= aabb->min.z && point->z <= aabb->max.z;
}

/*=========================================*/
int swAabbAabbIntersection(const SWAabb * aabb, const SWAabb * other) {
	int i, points_inside = 0;
	for(i = 0; i < 8; ++i) {
		if(swAabbIsContainsPoint(aabb, &other->corners[i])) {
			++points_inside;
		}
	}
	return points_inside;
}

/*=========================================*/
int swAabbTriangleIntersection(const SWAabb * aabb, const SWVec3 * a, const SWVec3 * b, const SWVec3 * c) {
	SWVec3 triangle_min, triangle_max;
	SWVec3 aabb_center, aabb_size, triangle_aabb_center, triangle_aabb_size;

	swVec3Set(&triangle_min, FLT_MAX, FLT_MAX, FLT_MAX);
	swVec3Set(&triangle_max, -FLT_MAX, -FLT_MAX, -FLT_MAX);

	/* get aabb of triangle */
	swVec3MinMax(a, &triangle_min, &triangle_max);
	swVec3MinMax(b, &triangle_min, &triangle_max);
	swVec3MinMax(c, &triangle_min, &triangle_max);

	/* get size of triangle's aabb */
	swVec3Scale(&triangle_aabb_size, swVec3Sub(&triangle_aabb_size, &triangle_max, &triangle_min), 0.5f);
	/* get center of triangle's aabb */
	swVec3Middle(&triangle_aabb_center, &triangle_min, &triangle_max);

	/* get size of aabb */
	swVec3Scale(&aabb_size, swVec3Sub(&aabb_size, &aabb->max, &aabb->min), 0.5f);
	/* get center of aabb */
	swVec3Middle(&aabb_center, &aabb->min, &aabb->max);

	/* do aabb intesection test */
	if(fabs(aabb_center.x - triangle_aabb_center.x) > (aabb_size.x + triangle_aabb_size.x)) return 0;
	if(fabs(aabb_center.y - triangle_aabb_center.y) > (aabb_size.y + triangle_aabb_size.y)) return 0;
	if(fabs(aabb_center.z - triangle_aabb_center.z) > (aabb_size.z + triangle_aabb_size.z)) return 0;

	return 1;
}
