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

#ifndef _AABB_H_
#define _AABB_H_

#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SWAabb {
	SWVec3 min, max;
	SWVec3 corners[8];
} SWAabb;

SWAabb * swAabbSet(SWAabb * aabb, const SWVec3 * min, const SWVec3 * max);
SWAabb * swAabbRecomputeCorners(SWAabb * aabb);
int swAabbSphereIntersection(const SWAabb * aabb, const SWVec3 * aabb_offset, const SWVec3 * position, float radius);
int swAabbIsContainsPoint(const SWAabb * aabb, const SWVec3 * point);
int swAabbAabbIntersection(const SWAabb * aabb, const SWAabb * other);
int swAabbTriangleIntersection(const SWAabb * aabb, const SWVec3 * a, const SWVec3 * b, const SWVec3 * c);

#ifdef __cplusplus
}
#endif

#endif
