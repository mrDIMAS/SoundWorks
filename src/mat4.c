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

#include "mat4.h"
#include "vec3.h"
#include "quat.h"
#include <math.h>
#include <stdlib.h>
#include <memory.h>

/*=========================================*/
SWMat4 * swMat4Identity(SWMat4 * out) {
	memset(out->f, 0, 16 * sizeof(float));
	out->f[0] = 1.0f;
	out->f[5] = 1.0f;
	out->f[10] = 1.0f;
	out->f[15] = 1.0f;
	return out;
}

/*=========================================*/
SWMat4 * swMat4Transpose(SWMat4 * out, const SWMat4 * m) {
	int i, j;
	for(j = 0; j < 4; ++j) {
		for(i = 0; i < 4; ++i) {
			out->f[(j * 4) + i] = m->f[(i * 4) + j];
		}
	}
	return out;
}

/*=========================================*/
SWMat4 * swMat4Mul(SWMat4 * out, const SWMat4 * a, const SWMat4 * b) {
	SWMat4 temp;
	temp.f[0] = a->f[0] * b->f[0] + a->f[1] * b->f[4] + a->f[2] * b->f[8] + a->f[3] * b->f[12];
	temp.f[1] = a->f[0] * b->f[1] + a->f[1] * b->f[5] + a->f[2] * b->f[9] + a->f[3] * b->f[13];
	temp.f[2] = a->f[0] * b->f[2] + a->f[1] * b->f[6] + a->f[2] * b->f[10] + a->f[3] * b->f[14];
	temp.f[3] = a->f[0] * b->f[3] + a->f[1] * b->f[7] + a->f[2] * b->f[11] + a->f[3] * b->f[15];
	temp.f[4] = a->f[4] * b->f[0] + a->f[5] * b->f[4] + a->f[6] * b->f[8] + a->f[7] * b->f[12];
	temp.f[5] = a->f[4] * b->f[1] + a->f[5] * b->f[5] + a->f[6] * b->f[9] + a->f[7] * b->f[13];
	temp.f[6] = a->f[4] * b->f[2] + a->f[5] * b->f[6] + a->f[6] * b->f[10] + a->f[7] * b->f[14];
	temp.f[7] = a->f[4] * b->f[3] + a->f[5] * b->f[7] + a->f[6] * b->f[11] + a->f[7] * b->f[15];
	temp.f[8] = a->f[8] * b->f[0] + a->f[9] * b->f[4] + a->f[10] * b->f[8] + a->f[11] * b->f[12];
	temp.f[9] = a->f[8] * b->f[1] + a->f[9] * b->f[5] + a->f[10] * b->f[9] + a->f[11] * b->f[13];
	temp.f[10] = a->f[8] * b->f[2] + a->f[9] * b->f[6] + a->f[10] * b->f[10] + a->f[11] * b->f[14];
	temp.f[11] = a->f[8] * b->f[3] + a->f[9] * b->f[7] + a->f[10] * b->f[11] + a->f[11] * b->f[15];
	temp.f[12] = a->f[12] * b->f[0] + a->f[13] * b->f[4] + a->f[14] * b->f[8] + a->f[15] * b->f[12];
	temp.f[13] = a->f[12] * b->f[1] + a->f[13] * b->f[5] + a->f[14] * b->f[9] + a->f[15] * b->f[13];
	temp.f[14] = a->f[12] * b->f[2] + a->f[13] * b->f[6] + a->f[14] * b->f[10] + a->f[15] * b->f[14];
	temp.f[15] = a->f[12] * b->f[3] + a->f[13] * b->f[7] + a->f[14] * b->f[11] + a->f[15] * b->f[15];
	*out = temp;
	return out;
}

/*=========================================*/
SWMat4 * swMat4Scale(SWMat4 * out, const SWVec3 * v) {
	memset(out->f, 0, 16 * sizeof(float));
	out->f[0] = v->x;
	out->f[5] = v->y;
	out->f[10] = v->z;
	out->f[15] = 1.0f;
	return out;
}

/*=========================================*/
SWMat4 * swMat4Frustum(SWMat4 * out, float left, float right, float bottom, float top, float zNear, float zFar) {
	float temp = 2.0f * zNear;
	float temp2 = right - left;
	float temp3 = top - bottom;
	float temp4 = zFar - zNear;
	memset(out->f, 0, 16 * sizeof(float));
	out->f[0] = temp / temp2;
	out->f[5] = temp / temp3;
	out->f[8] = (right + left) / temp2;
	out->f[9] = (top + bottom) / temp3;
	out->f[10] = (-zFar - zNear) / temp4;
	out->f[11] = -1.0f;
	out->f[14] = (-temp * zFar) / temp4;
	return out;
}

/*=========================================*/
SWMat4 * swMat4Perspective(SWMat4 * out, float fov_radians, float aspect, float zNear, float zFar) {
	float yMax = zNear * tan(fov_radians);
	float xMax = yMax * aspect;
	return swMat4Frustum(out, -xMax, xMax, -yMax, yMax, zNear, zFar);
}

/*=========================================*/
SWMat4 * swMat4Ortho(SWMat4 * out, float left, float right, float bottom, float top, float zNear, float zFar) {
	memset(out->f, 0, 16 * sizeof(float));
	out->f[0] = 2.0f / (right - left);
	out->f[5] = 2.0f / (top - bottom);
	out->f[10] = 1.0f / (zFar - zNear);
	out->f[12] = (left + right) / (left - right);
	out->f[13] = (top + bottom) / (bottom - top);
	out->f[14] = zNear / (zNear - zFar);
	out->f[15] = 1.0f;
	return out;
}

/*=========================================*/
SWMat4 * swMat4Translation(SWMat4 * out, const SWVec3 * v) {
	memset(out->f, 0, 16 * sizeof(float));
	out->f[0] = 1.0f;
	out->f[5] = 1.0f;
	out->f[10] = 1.0f;
	out->f[12] = v->x;
	out->f[13] = v->y;
	out->f[14] = v->z;
	out->f[15] = 1.0f;
	return out;
}

/*=========================================*/
SWMat4 * swMat4LookAt(SWMat4 * out, const SWVec3 * eye, const SWVec3 * look, const SWVec3 * up) {
	SWVec3 f, r, u;

	swVec3Normalize(&f, swVec3Sub(&f, look, eye));
	swVec3Normalize(&r, swVec3Cross(&r, &f, up));
	swVec3Normalize(&u, swVec3Cross(&u, &r, &f));

	out->f[0] = r.x;
	out->f[1] = u.x;
	out->f[2] = -f.x;
	out->f[3] = 0.0;

	out->f[4] = r.y;
	out->f[5] = u.y;
	out->f[6] = -f.y;
	out->f[7] = 0.0;

	out->f[8] = r.z;
	out->f[9] = u.z;
	out->f[10] = -f.z;
	out->f[11] = 0.0;

	out->f[12] = -swVec3Dot(&r, eye);
	out->f[13] = -swVec3Dot(&u, eye);
	out->f[14] = swVec3Dot(&f, eye);
	out->f[15] = 1.0;

	return out;
}

/*=========================================*/
SWMat4 * swMat4Inverse(SWMat4 * out, const SWMat4 * mat) {
	int i;
	float det;
	SWMat4 temp;
	const float * a = mat->f;
	temp.f[0] = a[5] * a[10] * a[15] - a[5] * a[14] * a[11] - a[6] * a[9] * a[15] + a[6] * a[13] * a[11] + a[7] * a[9] * a[14] - a[7] * a[13] * a[10];
	temp.f[1] = -a[1] * a[10] * a[15] + a[1] * a[14] * a[11] + a[2] * a[9] * a[15] - a[2] * a[13] * a[11] - a[3] * a[9] * a[14] + a[3] * a[13] * a[10];
	temp.f[2] = a[1] * a[6] * a[15] - a[1] * a[14] * a[7] - a[2] * a[5] * a[15] + a[2] * a[13] * a[7] + a[3] * a[5] * a[14] - a[3] * a[13] * a[6];
	temp.f[3] = -a[1] * a[6] * a[11] + a[1] * a[10] * a[7] + a[2] * a[5] * a[11] - a[2] * a[9] * a[7] - a[3] * a[5] * a[10] + a[3] * a[9] * a[6];
	temp.f[4] = -a[4] * a[10] * a[15] + a[4] * a[14] * a[11] + a[6] * a[8] * a[15] - a[6] * a[12] * a[11] - a[7] * a[8] * a[14] + a[7] * a[12] * a[10];
	temp.f[5] = a[0] * a[10] * a[15] - a[0] * a[14] * a[11] - a[2] * a[8] * a[15] + a[2] * a[12] * a[11] + a[3] * a[8] * a[14] - a[3] * a[12] * a[10];
	temp.f[6] = -a[0] * a[6] * a[15] + a[0] * a[14] * a[7] + a[2] * a[4] * a[15] - a[2] * a[12] * a[7] - a[3] * a[4] * a[14] + a[3] * a[12] * a[6];
	temp.f[7] = a[0] * a[6] * a[11] - a[0] * a[10] * a[7] - a[2] * a[4] * a[11] + a[2] * a[8] * a[7] + a[3] * a[4] * a[10] - a[3] * a[8] * a[6];
	temp.f[8] = a[4] * a[9] * a[15] - a[4] * a[13] * a[11] - a[5] * a[8] * a[15] + a[5] * a[12] * a[11] + a[7] * a[8] * a[13] - a[7] * a[12] * a[9];
	temp.f[9] = -a[0] * a[9] * a[15] + a[0] * a[13] * a[11] + a[1] * a[8] * a[15] - a[1] * a[12] * a[11] - a[3] * a[8] * a[13] + a[3] * a[12] * a[9];
	temp.f[10] = a[0] * a[5] * a[15] - a[0] * a[13] * a[7] - a[1] * a[4] * a[15] + a[1] * a[12] * a[7] + a[3] * a[4] * a[13] - a[3] * a[12] * a[5];
	temp.f[11] = -a[0] * a[5] * a[11] + a[0] * a[9] * a[7] + a[1] * a[4] * a[11] - a[1] * a[8] * a[7] - a[3] * a[4] * a[9] + a[3] * a[8] * a[5];
	temp.f[12] = -a[4] * a[9] * a[14] + a[4] * a[13] * a[10] + a[5] * a[8] * a[14] - a[5] * a[12] * a[10] - a[6] * a[8] * a[13] + a[6] * a[12] * a[9];
	temp.f[13] = a[0] * a[9] * a[14] - a[0] * a[13] * a[10] - a[1] * a[8] * a[14] + a[1] * a[12] * a[10] + a[2] * a[8] * a[13] - a[2] * a[12] * a[9];
	temp.f[14] = -a[0] * a[5] * a[14] + a[0] * a[13] * a[6] + a[1] * a[4] * a[14] - a[1] * a[12] * a[6] - a[2] * a[4] * a[13] + a[2] * a[12] * a[5];
	temp.f[15] = a[0] * a[5] * a[10] - a[0] * a[9] * a[6] - a[1] * a[4] * a[10] + a[1] * a[8] * a[6] + a[2] * a[4] * a[9] - a[2] * a[8] * a[5];
	det = a[0] * temp.f[0] + a[4] * temp.f[1] + a[8] * temp.f[2] + a[12] * temp.f[3];
	if(fabs(det) > 0.00001f) det = 1.0f / det;
	for(i = 0; i < 16; i++) out->f[i] = temp.f[i] * det;
	return out;
}

/*=========================================*/
SWMat4 * swMat4RotationTranslation(SWMat4 * out, const SWQuat * q, const SWVec3 * v) {
	float s = 2.0f / swQuatSqrLen(q);
	float xs = q->x * s, ys = q->y * s, zs = q->z * s;
	float wx = q->w * xs, wy = q->w * ys, wz = q->w * zs;
	float xx = q->x * xs, xy = q->x * ys, xz = q->x * zs;
	float yy = q->y * ys, yz = q->y * zs, zz = q->z * zs;
	out->f[0] = 1.0f - (yy + zz);
	out->f[1] = xy + wz;
	out->f[2] = xz - wy;
	out->f[3] = 0.0f;
	out->f[4] = xy - wz;
	out->f[5] = 1.0f - (xx + zz);
	out->f[6] = yz + wx;
	out->f[7] = 0.0f;
	out->f[8] = xz + wy;
	out->f[9] = yz - wx;
	out->f[10] = 1.0f - (xx + yy);
	out->f[11] = 0.0f;
	out->f[12] = v->x;
	out->f[13] = v->y;
	out->f[14] = v->z;
	out->f[15] = 1.0f;
	return out;
}

/*=========================================*/
SWMat4 * swMat4FromQuat(SWMat4 * out, const SWQuat * q) {
	SWVec3 z;
	return swMat4RotationTranslation(out, q, swVec3Zero(&z));
}

/*=========================================*/
SWMat4 * swMat4RotationX(SWMat4 * out, float angle_radians) {
	out->f[0] = 1.0f;
	out->f[1] = 0.0f;
	out->f[2] = 0.0f;
	out->f[3] = 0.0f;

	out->f[4] = 0.0f;
	out->f[5] = cos(angle_radians);
	out->f[6] = sin(angle_radians);
	out->f[7] = 0.0f;

	out->f[8] = 0.0f;
	out->f[9] = -sin(angle_radians);
	out->f[10] = cos(angle_radians);
	out->f[11] = 0.0f;

	out->f[12] = 0.0f;
	out->f[13] = 0.0f;
	out->f[14] = 0.0f;
	out->f[15] = 1.0f;

	return out;
}

/*=========================================*/
SWMat4 * swMat4RotationY(SWMat4 * out, float angle_radians) {
	out->f[0] = cos(angle_radians);
	out->f[1] = 0.0f;
	out->f[2] = -sin(angle_radians);
	out->f[3] = 0.0f;

	out->f[4] = 0.0f;
	out->f[5] = 1.0f;
	out->f[6] = 0.0f;
	out->f[7] = 0.0f;

	out->f[8] = sin(angle_radians);
	out->f[9] = 0.0f;
	out->f[10] = cos(angle_radians);
	out->f[11] = 0.0f;

	out->f[12] = 0.0f;
	out->f[13] = 0.0f;
	out->f[14] = 0.0f;
	out->f[15] = 1.0f;

	return out;
}

/*=========================================*/
SWMat4 * swMat4RotationZ(SWMat4 * out, float angle_radians) {
	out->f[0] = cos(angle_radians);
	out->f[1] = sin(angle_radians);
	out->f[2] = 0.0f;
	out->f[3] = 0.0f;

	out->f[4] = -sin(angle_radians);
	out->f[5] = cos(angle_radians);
	out->f[6] = 0.0f;
	out->f[7] = 0.0f;

	out->f[8] = 0.0f;
	out->f[9] = 0.0f;
	out->f[10] = 1.0f;
	out->f[11] = 0.0f;

	out->f[12] = 0.0f;
	out->f[13] = 0.0f;
	out->f[14] = 0.0f;
	out->f[15] = 1.0f;

	return out;
}

/*=========================================*/
SWMat4 * swMat4RotationYawPitchRoll(SWMat4 * out, float pitch, float yaw, float roll) {
	SWMat4 yaw_matrix;
	SWMat4 roll_matrix;
	SWMat4 pitch_matrix;
	swMat4RotationY(&yaw_matrix, yaw);
	swMat4RotationX(&pitch_matrix, pitch);
	swMat4RotationZ(&roll_matrix, roll);
	swMat4Mul(out, &pitch_matrix, &roll_matrix);
	swMat4Mul(out, &yaw_matrix, out);
	return out;
}

/*=========================================*/
SWVec3 * swMat4GetUpVector(const SWMat4 * m, SWVec3 * up) {
	up->x = m->f[4];
	up->y = m->f[5];
	up->z = m->f[6];
	return up;
}

/*=========================================*/
SWVec3 * swMat4GetSideVector(const SWMat4 * m, SWVec3 * side) {
	side->x = m->f[0];
	side->y = m->f[1];
	side->z = m->f[2];
	return side;
}

/*=========================================*/
SWVec3 * swMat4GetLookVector(const SWMat4 * m, SWVec3 * look) {
	look->x = m->f[8];
	look->y = m->f[9];
	look->z = m->f[10];
	return look;
}
