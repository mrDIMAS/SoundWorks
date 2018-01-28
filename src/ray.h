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

#ifndef _RAY_H_
#define _RAY_H_

#include "vec3.h"
#include "plane.h"
#include "aabb.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SWRay {
	SWVec3 origin;
	SWVec3 dir;
} SWRay;

SWRay * swRaySet(SWRay * out, const SWVec3 * origin, const SWVec3 * dir);
SWRay * swRayByTwoPoints(SWRay * out, const SWVec3 * start, const SWVec3 * end);
int swRayAabbIntersection(const SWRay * ray, const SWAabb * aabb, float * out_tmin, float * out_tmax);
int swRayPlaneIntersection(const SWRay * ray, const plane_t * plane, SWVec3 * out_intersection_point);
int swRayTriangleIntersection(const SWRay * ray, const SWVec3 * a, const SWVec3 * b, const SWVec3 * c, SWVec3 * out_intersection_point);
int swRaySphereIntersection(const SWRay * ray, const SWVec3 * position, float radius, SWVec3 * out_int_point_a, SWVec3 * out_int_point_b);

#ifdef __cplusplus
}
#endif

#endif