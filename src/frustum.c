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

#include "frustum.h"

/*=========================================*/
frustum * frustum_from_matrix(frustum * f, const SWMat4 * m) {	
	swPlaneSetCoeff(&f->planes[0], m->f[3] + m->f[0], m->f[7] + m->f[4], m->f[11] + m->f[8], m->f[15] + m->f[12]); /* left plane */
	swPlaneSetCoeff(&f->planes[1], m->f[3] - m->f[0], m->f[7] - m->f[4], m->f[11] - m->f[8], m->f[15] - m->f[12]); /* right plane */
	swPlaneSetCoeff(&f->planes[2], m->f[3] - m->f[1], m->f[7] - m->f[5], m->f[11] - m->f[9], m->f[15] - m->f[13]); /* top plane */
	swPlaneSetCoeff(&f->planes[3], m->f[3] + m->f[1], m->f[7] + m->f[5], m->f[11] + m->f[9], m->f[15] + m->f[13]); /* bottom plane */	
	swPlaneSetCoeff(&f->planes[4], m->f[3] - m->f[2], m->f[7] - m->f[6], m->f[11] - m->f[10], m->f[15] - m->f[14]); /* far plane */
	swPlaneSetCoeff(&f->planes[5], m->f[2], m->f[6], m->f[10], m->f[14]); /* near plane */
	return f;
}

/*=========================================*/
int frustum_box_intersection(const frustum * f, const SWAabb * aabb, const SWVec3 * aabb_offset) {
	int i, k;
	SWVec3 aabb_points[8];
	for(i = 0; i < 8; ++i) {
		aabb_points[i] = aabb->corners[i];
		if(aabb_offset) {
			swVec3Add(&aabb_points[i], &aabb_points[i], aabb_offset);
		}
	}
	for(i = 0; i < 6; ++i ) {
		int back_points = 0;
		for(k = 0; k < 8; ++k) {
			if(swPlaneDot(&f->planes[i], &aabb_points[k]) <= 0) {
				if ( ++back_points >= 8 ) {
					return 0;
				}
			}
		}
	}
	return 1;
}

/*=========================================*/
int frustum_box_intersection_transform(const frustum * f, const SWAabb * aabb, const SWMat4 * obj_matrix) {
	int i, k;
	SWVec3 aabb_points[8];
	for(i = 0; i < 8; ++i) {
		aabb_points[i] = aabb->corners[i];
		if(obj_matrix) {
			swVec3Transform(&aabb_points[i], &aabb_points[i], obj_matrix);
		}
	}
	for(i = 0; i < 6; ++i ) {
		int back_points = 0;
		for(k = 0; k < 8; ++k) {
			if(swPlaneDot(&f->planes[i], &aabb_points[k]) <= 0) {
				if ( ++back_points >= 8 ) {
					return 0;
				}
			}
		}
	}
	return 1;
}

/*=========================================*/
int frustum_contains_point(const frustum * f, const SWVec3 * p) {
	int i;
	for(i = 0; i < 6; i++) {
		if(swPlaneDot(&f->planes[i], p) <= 0.0f) {
			return 0;
		}
	}
	return 1;
}