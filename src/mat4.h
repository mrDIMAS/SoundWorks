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

#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SWMat4 {
	float f[16];
} SWMat4;

struct SWVec3;
struct SWQuat;

SWMat4 * swMat4Identity(SWMat4 * out);
SWMat4 * swMat4Transpose(SWMat4 * out, const SWMat4 * m);
SWMat4 * swMat4Mul(SWMat4 * out, const SWMat4 * a, const SWMat4 * b);
SWMat4 * swMat4Scale(SWMat4 * out, const struct SWVec3 * v);
SWMat4 * swMat4Frustum(SWMat4 * out, float left, float right, float bottom, float top, float zNear, float zFar);
SWMat4 * swMat4Perspective(SWMat4 * out, float fov_radians, float aspect, float zNear, float zFar);
SWMat4 * swMat4Ortho(SWMat4 * out, float left, float right, float bottom, float top, float zNear, float zFar);
SWMat4 * swMat4Translation(SWMat4 * out, const struct SWVec3 * v);
SWMat4 * swMat4LookAt(SWMat4 * out, const struct SWVec3 * eye, const struct SWVec3 * look, const struct SWVec3 * up);
SWMat4 * swMat4Inverse(SWMat4 * out, const SWMat4 * mat);
SWMat4 * swMat4RotationTranslation(SWMat4 * out, const struct SWQuat * q, const struct SWVec3 * v);
SWMat4 * swMat4FromQuat(SWMat4 * out, const struct SWQuat * q);
SWMat4 * swMat4RotationX(SWMat4 * out, float angle_radians);
SWMat4 * swMat4RotationY(SWMat4 * out, float angle_radians);
SWMat4 * swMat4RotationZ(SWMat4 * out, float angle_radians);
SWMat4 * swMat4RotationYawPitchRoll(SWMat4 * out, float pitch, float yaw, float roll);
struct SWVec3 * swMat4GetUpVector(const SWMat4 * m, struct SWVec3 * up);
struct SWVec3 * swMat4GetSideVector(const SWMat4 * m, struct SWVec3 * side);
struct SWVec3 * swMat4GetLookVector(const SWMat4 * m, struct SWVec3 * look);

#ifdef __cplusplus
}
#endif

#endif