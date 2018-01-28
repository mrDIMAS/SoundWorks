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

#include "plane.h"
#include <math.h>

/*=========================================*/
plane_t * swPlaneSetCoeff(plane_t * p, float a, float b, float c, float d) {
	p->n.x = a;
	p->n.y = b;
	p->n.z = c;
	swVec3Normalize(&p->n, &p->n);
	p->d = d;
	return p;
}

/*=========================================*/
plane_t * swPlaneSet(plane_t * p, const SWVec3 * n, float d) {
	swVec3Normalize(&p->n, n);
	p->d = d;
	return p;
}

/*=========================================*/
float swPlaneDistance(const plane_t * p, const SWVec3 * point) {
	return fabs(swVec3Dot(&p->n, point) + p->d);
}

/*=========================================*/
float swPlaneDot(const plane_t * p, const SWVec3 * point) {
    return swVec3Dot(&p->n, point) + p->d;
}

/*=========================================*/
plane_t * swPlaneNormalize(plane_t * p) {
    float d = 1.0f / swVec3Len(&p->n);
	swVec3Scale(&p->n, &p->n, d);
    p->d *= d;
    return p;
}