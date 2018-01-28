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

#include "quat.h"
#include "vec3.h"
#include <math.h>

/*=========================================*/
SWQuat * swQuatSet(SWQuat * out, float x, float y, float z, float w) {
	out->x = x;
	out->y = y;
	out->z = z;
	out->w = w;
	return out;
}

/*=========================================*/
SWQuat * swQuatAxisAngle(SWQuat * out, const struct SWVec3 * axis, float angle) {
	float half_angle = angle * 0.5f;
	float d = swVec3Len(axis);
	float s = sin(half_angle) / d;
	out->x = axis->x * s;
	out->y = axis->y * s;
	out->z = axis->z * s;
	out->w = cos(half_angle);
	return out;
}

/*=========================================*/
float swQuatDot(const SWQuat * a, const SWQuat * b) {
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

/*=========================================*/
float swQuatLen(const SWQuat * a) {
	return sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

/*=========================================*/
float swQuatSqrLen(const SWQuat * a) {
	return a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w;
}

/*=========================================*/
float swQuatAngle(const SWQuat * a, const SWQuat * b) {
	float s = sqrt(swQuatSqrLen(a) * swQuatSqrLen(b));
	return acos(swQuatDot(a, b) / s);
}

/*=========================================*/
SWQuat * swQuatSlerp(SWQuat * out, const SWQuat * a, const SWQuat * b, float t) {
	SWQuat q;
	float theta = swQuatAngle(a, b);
	if (fabs(theta) > 0.00001f) {
		float d = 1.0f / sin(theta);
		float s0 = sin((1.0f - t) * theta);
		float s1 = sin(t * theta);
		if (swQuatDot(a, b) < 0) {
			q.x = (a->x * s0 + -b->x * s1) * d;
			q.y = (a->y * s0 + -b->y * s1) * d;
			q.z = (a->z * s0 + -b->z * s1) * d;
			q.w = (a->w * s0 + -b->w * s1) * d;			
		} else {
			q.x = (a->x * s0 + b->x * s1) * d;
			q.y = (a->y * s0 + b->y * s1) * d;
			q.z = (a->z * s0 + b->z * s1) * d;
			q.w = (a->w * s0 + b->w * s1) * d;		
		}
	}
	*out = q;
	return out;
}

/*=========================================*/
SWQuat * swQuatEuler(SWQuat * out, float yaw, float pitch, float roll) {
	float halfYaw = yaw * 0.5f;
	float halfPitch = pitch * 0.5f;
	float halfRoll = roll * 0.5f;
	float cosYaw = cos(halfYaw);
	float sinYaw = sin(halfYaw);
	float cosPitch = cos(halfPitch);
	float sinPitch = sin(halfPitch);
	float cosRoll = cos(halfRoll);
	float sinRoll = sin(halfRoll);
	out->x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	out->y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	out->z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
	out->w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;	
	return out;
}

/*=========================================*/
SWQuat * swQuatMul(SWQuat * out, const SWQuat * a, const SWQuat * b) {
	SWQuat t;
	t.x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
	t.y = a->w * b->y + a->y * b->w + a->z * b->x - a->x * b->z;
	t.z = a->w * b->z + a->z * b->w + a->x * b->y - a->y * b->x;
	t.w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
	*out = t;
	return out;
}

/*=========================================*/
SWQuat * swQuatNormalize(SWQuat * out, SWQuat * a) {
	float k = 1.0 / swQuatLen(a);
	out->x *= k;
	out->y *= k;
	out->z *= k;
	out->w *= k;
	return out;
}

/*=========================================*/
SWVec3 * swQuatGetAxis(const SWQuat * q, struct SWVec3 * out_axis) {
	float s_squared = 1.0f - q->w * q->w;	
	if (s_squared < 0.0001f) {
		swVec3Set(out_axis, 1.0f, 0.0f, 0.0f);
	} else {
		float s = 1.0f / sqrt(s_squared);
		swVec3Set(out_axis, q->x * s, q->y * s, q->z * s);	
	}	
	return out_axis;
}

/*=========================================*/
float swQuatGetAngle(const SWQuat * q) {
	return 2.0f * acos(q->w);
}