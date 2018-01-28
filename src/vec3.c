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

#include "vec3.h"
#include "mat4.h"
#include <math.h>

/*=========================================*/
SWVec3 * swVec3Set(SWVec3 * out, float x, float y, float z) {
	out->x = x;
	out->y = y;
	out->z = z;
	return out;
}

/*=========================================*/
SWVec3 * swVec3Zero(SWVec3 * out) {
	out->x = 0.0f;
	out->y = 0.0f;
	out->z = 0.0f;
	return out;
}

/*=========================================*/
SWVec3 * swVec3Add(SWVec3 * out, const SWVec3 * a, const SWVec3 * b) {
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	return out;
}

/*=========================================*/
SWVec3 * swVec3Sub(SWVec3 * out, const SWVec3 * a, const SWVec3 * b) {
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
	return out;
}

/*=========================================*/
SWVec3 * swVec3Div(SWVec3 * out, const SWVec3 * a, const SWVec3 * b) {
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
	return out;
}

/*=========================================*/
SWVec3 * swVec3Mul(SWVec3 * out, const SWVec3 * a, const SWVec3 * b) {
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
	return out;
}

/*=========================================*/
SWVec3 * swVec3Scale(SWVec3 * out, const SWVec3 * a, float s) {
	out->x = a->x * s;
	out->y = a->y * s;
	out->z = a->z * s;
	return out;
}

/*=========================================*/
SWVec3 * swVec3Negate(SWVec3 * out, const SWVec3 * a) {
	out->x = -a->x;
	out->y = -a->y;
	out->z = -a->z;
	return out;
}

/*=========================================*/
float swVec3Dot(const SWVec3 * a, const SWVec3 * b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

/*=========================================*/
SWVec3 * swVec3Cross(SWVec3 * out, const SWVec3 * a, const SWVec3 * b) {
	SWVec3 v;
	v.x = a->y * b->z - a->z * b->y;
	v.y = a->z * b->x - a->x * b->z;
	v.z = a->x * b->y - a->y * b->x;
	out->x = v.x;
	out->y = v.y;
	out->z = v.z;
	return out;
}

/*=========================================*/
float swVec3Len(const SWVec3 * a) {
	return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

/*=========================================*/
float swVec3SqrLen(const SWVec3 * a) {
	return a->x * a->x + a->y * a->y + a->z * a->z;
}

/*=========================================*/
SWVec3 * swVec3Normalize(SWVec3 * out, const SWVec3 * a) {
	float len = swVec3Len(a);
	if(len > 0.0001f) {
		float k = 1.0f / len;
		out->x = a->x * k;
		out->y = a->y * k;
		out->z = a->z * k;
	}
	return out;
}

/*=========================================*/
SWVec3 * swVec3Lerp(SWVec3 * out, const SWVec3 * a, const SWVec3 * b, float t) {
	out->x = a->x + (b->x - a->x) * t;
	out->y = a->y + (b->y - a->y) * t;
	out->z = a->z + (b->z - a->z) * t;
	return out;
}

/*=========================================*/
float swVec3Distance(const SWVec3 * a, const SWVec3 * b) {
	SWVec3 d;
	return swVec3Len(swVec3Sub(&d, a, b));
}

/*=========================================*/
float swVec3SqrDistance(const SWVec3 * a, const SWVec3 * b) {
	SWVec3 d;
	return swVec3SqrLen(swVec3Sub(&d, a, b));
}

/*=========================================*/
float swVec3Angle(const SWVec3 * a, const SWVec3 * b) {
	return acos(swVec3Dot(a, b) / (swVec3Len(a) * swVec3Len(b)));
}

/*=========================================*/
SWVec3 * swVec3ProjectOnToPlane(SWVec3 * out, const SWVec3 * point, const SWVec3 * normal) {
	SWVec3 n = {0};
	if(swVec3SqrLen(normal) < 0.00001f) return out; /* degenerated normal vector */
	swVec3Normalize(&n, normal);
	swVec3Scale(&n, &n, -swVec3Dot(&n, point));
	return swVec3Add(out, point, &n);
}

/*=========================================*/
SWVec3 * swVec3Reflect(SWVec3 * out, const SWVec3 * a, const SWVec3 * normal) {
	SWVec3 v;
	swVec3Scale(&v, normal, 2.0f * swVec3Dot(a, normal));
	return swVec3Sub(out, a, &v);
}

/*=========================================*/
SWVec3 * swVec3Transform(SWVec3 * out, const SWVec3 * a, const SWMat4 * mat) {
	SWVec3 v;
	v.x = a->x * mat->f[0] + a->y * mat->f[4] + a->z * mat->f[8] + mat->f[12];
	v.y = a->x * mat->f[1] + a->y * mat->f[5] + a->z * mat->f[9] + mat->f[13];
	v.z = a->x * mat->f[2] + a->y * mat->f[6] + a->z * mat->f[10] + mat->f[14];
	out->x = v.x;
	out->y = v.y;
	out->z = v.z;
	return out;
}

/*=========================================*/
SWVec3 * swVec3TransformNormal(SWVec3 * out, const SWVec3 * a, const SWMat4 * mat) {
	SWVec3 v;
	v.x = a->x * mat->f[0] + a->y * mat->f[4] + a->z * mat->f[8];
	v.y = a->x * mat->f[1] + a->y * mat->f[5] + a->z * mat->f[9];
	v.z = a->x * mat->f[2] + a->y * mat->f[6] + a->z * mat->f[10];
	out->x = v.x;
	out->y = v.y;
	out->z = v.z;
	return out;
}

/*=========================================*/
SWVec3 * swVec3Middle(SWVec3 * out, const SWVec3 * a, const SWVec3 * b) {
	SWVec3 v;
	swVec3Add(&v, a, b);
	swVec3Scale(out, &v, 0.5f);
	return out;
}

/*=========================================*/
void swVec3MinMax(const SWVec3 * a, SWVec3 * vMin, SWVec3 * vMax) {
	if(a->x < vMin->x) vMin->x = a->x;
	if(a->y < vMin->y) vMin->y = a->y;
	if(a->z < vMin->z) vMin->z = a->z;

	if(a->x > vMax->x) vMax->x = a->x;
	if(a->y > vMax->y) vMax->y = a->y;
	if(a->z > vMax->z) vMax->z = a->z;
}

/*=========================================*/
int swVec3ApproxEquals(const SWVec3 * a, const SWVec3 * b) {
	return fabs(a->x - b->x) < 0.0001f && fabs(a->y - b->y) < 0.0001f && fabs(a->z - b->z) < 0.0001f;
}

/*=========================================*/
int swVec3Equals(const SWVec3 * a, const SWVec3 * b) {
	return a->x == b->x && a->y == b->y && a->z == b->z;
}
