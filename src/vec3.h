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

#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SWVec3 {
	float x, y, z;
} SWVec3;

struct SWMat4;

SWVec3 * swVec3Set(SWVec3 * out, float x, float y, float z);
SWVec3 * swVec3Zero(SWVec3 * out);
SWVec3 * swVec3Add(SWVec3 * out, const SWVec3 * a, const SWVec3 * b);
SWVec3 * swVec3Sub(SWVec3 * out, const SWVec3 * a, const SWVec3 * b);
SWVec3 * swVec3Div(SWVec3 * out, const SWVec3 * a, const SWVec3 * b);
SWVec3 * swVec3Mul(SWVec3 * out, const SWVec3 * a, const SWVec3 * b);
SWVec3 * swVec3Scale(SWVec3 * out, const SWVec3 * a, float s);
SWVec3 * swVec3Negate(SWVec3 * out, const SWVec3 * a);
float swVec3Dot(const SWVec3 * a, const SWVec3 * b);
SWVec3 * swVec3Cross(SWVec3 * out, const SWVec3 * a, const SWVec3 * b);
float swVec3Len(const SWVec3 * a);
float swVec3SqrLen(const SWVec3 * a);
SWVec3 * swVec3Normalize(SWVec3 * out, const SWVec3 * a);
SWVec3 * swVec3Lerp(SWVec3 * out, const SWVec3 * a, const SWVec3 * b, float t);
float swVec3Distance(const SWVec3 * a, const SWVec3 * b);
float swVec3SqrDistance(const SWVec3 * a, const SWVec3 * b);
float swVec3Angle(const SWVec3 * a, const SWVec3 * b);
SWVec3 * swVec3ProjectOnToPlane(SWVec3 * out, const SWVec3 * point, const SWVec3 * normal);
SWVec3 * swVec3Reflect(SWVec3 * out, const SWVec3 * a, const SWVec3 * normal);
SWVec3 * swVec3Transform(SWVec3 * out, const SWVec3 * a, const struct SWMat4 * mat);
SWVec3 * swVec3TransformNormal(SWVec3 * out, const SWVec3 * a, const struct SWMat4 * mat);
SWVec3 * swVec3Middle(SWVec3 * out, const SWVec3 * a, const SWVec3 * b);
void swVec3MinMax( const SWVec3 * a, SWVec3 * vMin, SWVec3 * vMax );
int swVec3ApproxEquals(const SWVec3 * a, const SWVec3 * b);
int swVec3Equals(const SWVec3 * a, const SWVec3 * b);

#ifdef __cplusplus
}
#endif


#endif