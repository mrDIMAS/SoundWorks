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

#ifndef _QUAT_H_
#define _QUAT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SWQuat {
	float x, y, z, w;
} SWQuat;

struct SWVec3;

SWQuat * swQuatSet(SWQuat * out, float x, float y, float z, float w);
SWQuat * swQuatAxisAngle(SWQuat * out, const struct SWVec3 * axis, float angle);
float swQuatDot(const SWQuat * a, const SWQuat * b);
float swQuatLen(const SWQuat * a);
float swQuatSqrLen(const SWQuat * a);
float swQuatAngle(const SWQuat * a, const SWQuat * b);
SWQuat * swQuatSlerp(SWQuat * out, const SWQuat * a, const SWQuat * b, float t);
SWQuat * swQuatEuler(SWQuat * out, float yaw, float pitch, float roll);
SWQuat * swQuatMul(SWQuat * out, const SWQuat * a, const SWQuat * b);
SWQuat * swQuatNormalize(SWQuat * out, SWQuat * a);
struct SWVec3 * swQuatGetAxis(const SWQuat * q, struct SWVec3 * out_axis);
float swQuatGetAngle(const SWQuat * q);

#ifdef __cplusplus
}
#endif

#endif