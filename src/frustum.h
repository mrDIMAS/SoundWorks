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

#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "plane.h"
#include "mat4.h"
#include "aabb.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frustum {
	plane_t planes[6];
} frustum;

frustum * frustum_from_matrix(frustum * f, const SWMat4 * m);
int frustum_box_intersection(const frustum * f, const SWAabb * aabb, const SWVec3 * aabb_offset);
int frustum_box_intersection_transform(const frustum * f, const SWAabb * aabb, const SWMat4 * obj_matrix);
int frustum_contains_point(const frustum * f, const SWVec3 * p);

#ifdef __cplusplus
}
#endif

#endif