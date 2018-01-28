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

#include "util.h"
#include <stdlib.h>
#include <math.h>

/*=========================================*/
float swSqr(float a) {
	return a * a;
}

/*=========================================*/
float swRadToDeg(float a) {
	return a * 180.0f / 3.14159f;
}

/*=========================================*/
float swDegToRad(float a) {
	return a * 3.14159 / 180.0f;
}

/*=========================================*/
float swLerp(float a, float b, float t) {
	return a + (b - a) * t;
}

/*=========================================*/
float swMin( float a, float b ) {
    return a < b ? a : b;
}

/*=========================================*/
float swMax( float a, float b ) {
    return a > b ? a : b;
}

/*=========================================*/
float swRandom( float min, float max ) {
    return ((float)rand() / (float)RAND_MAX) * ( max - min );
}

/*=========================================*/
SWVec3 * swSphericalToCartesian(SWVec3 * cartesian, float hor, float ver, float radius) {
	cartesian->x = radius * sin(hor) * cos(ver);
	cartesian->y = radius * sin(hor) * sin(ver);
	cartesian->z = radius * cos(hor);
	return cartesian;
}

/*=========================================*/
void swCartesianToSpherical(const SWVec3 * v, float * phi, float * theta, float * r) {
	*r = swVec3Len(v);
	*theta = atan2(v->z, v->x);
	*phi = acos(v->y / *r);
}