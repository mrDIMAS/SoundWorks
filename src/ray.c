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

#include "ray.h"
#include <math.h>

/*=========================================*/
SWRay * swRaySet(SWRay * out, const SWVec3 * origin, const SWVec3 * dir) {
	out->origin = *origin;
	out->dir = *dir;
	return out;
}

/*=========================================*/
SWRay * swRayByTwoPoints(SWRay * out, const SWVec3 * start, const SWVec3 * end) {
	out->origin = *start;
	swVec3Sub(&out->dir, end, start);
	return out;
}

/*=========================================*/
int swRayAabbIntersection(const SWRay * ray, const SWAabb * aabb, float * out_tmin, float * out_tmax) {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	if(ray->dir.x >= 0) {
		tmin = (aabb->min.x - ray->origin.x) / ray->dir.x;
		tmax = (aabb->max.x - ray->origin.x) / ray->dir.x;
	} else {
		tmin = (aabb->max.x - ray->origin.x) / ray->dir.x;
		tmax = (aabb->min.x - ray->origin.x) / ray->dir.x;
	}
	if(ray->dir.y >= 0) {
		tymin = (aabb->min.y - ray->origin.y) / ray->dir.y;
		tymax = (aabb->max.y - ray->origin.y) / ray->dir.y;
	} else {
		tymin = (aabb->max.y - ray->origin.y) / ray->dir.y;
		tymax = (aabb->min.y - ray->origin.y) / ray->dir.y;
	}
	if((tmin > tymax) || (tymin > tmax)) return 0; 
	if(tymin > tmin) tmin = tymin;
	if(tymax < tmax) tmax = tymax; 
	if(ray->dir.z >= 0) {
		tzmin = (aabb->min.z - ray->origin.z) / ray->dir.z;
		tzmax = (aabb->max.z - ray->origin.z) / ray->dir.z;
	} else {
		tzmin = (aabb->max.z - ray->origin.z) / ray->dir.z;
		tzmax = (aabb->min.z - ray->origin.z) / ray->dir.z;
	}
	if((tmin > tzmax) || (tzmin > tmax)) return 0;
	if(tzmin > tmin) tmin = tzmin;
	if(tzmax < tmax) tmax = tzmax;
	if(out_tmin) *out_tmin = tmin;
	if(out_tmax) *out_tmax = tmax;
	return ((tmin < 1.0f) && (tmax > 0.0f));
}

/*=========================================*/
int swRayPlaneIntersection(const SWRay * ray, const plane_t * plane, SWVec3 * out_intersection_point) {
	/* solve plane equation */
	float u = -(swVec3Dot(&ray->origin, &plane->n) + plane->d);
	float v = swVec3Dot(&ray->dir, &plane->n);
	float t = u / v;
	/* ray miss */
	if (t < 0.0f) return 0; 
	/* find intersection point */
	if(out_intersection_point) {
		SWVec3 v;
		swVec3Scale(&v, &ray->dir, t);
		swVec3Add(out_intersection_point, &ray->origin, &v);
	}
    return 1;
}

/*=========================================*/
int swRayTriangleIntersection(const SWRay * ray, const SWVec3 * a, const SWVec3 * b, const SWVec3 * c, SWVec3 * out_intersection_point) {
	float baDotba, caDotba, caDotca, invDenom, dot02, dot12, u, v;
	SWVec3 vp, point, ca, ba;
	plane_t plane;
		
	swVec3Sub(&ba, b, a);
	swVec3Sub(&ca, c, a);	

	
	swVec3Normalize(&plane.n, swVec3Cross(&plane.n, &ba, &ca));
	plane.d = -swVec3Dot(a, &plane.n);
	
	if(swRayPlaneIntersection(ray, &plane, &point)) {		
		if(out_intersection_point) *out_intersection_point = point;
		
			swVec3Sub(&vp, &point, a);
			
		/* check, if intersection point lies in triangle using barycentric coordinates */
		baDotba = swVec3Dot(&ba, &ba);
		caDotba = swVec3Dot(&ca, &ba);
		caDotca = swVec3Dot(&ca, &ca);
		
		dot02 = swVec3Dot(&ca, &vp);
		dot12 = swVec3Dot(&ba, &vp);
		
		invDenom = 1.0f / (caDotca * baDotba - caDotba * caDotba);
		
		u = (baDotba * dot02 - caDotba * dot12) * invDenom;
		v = (caDotca * dot12 - caDotba * dot02) * invDenom;
		
		return (u >= 0.0f) && (v >= 0.0f) && (u + v < 1.0f);	
	}
	
	return 0;
}

/*=========================================*/
int swRaySphereIntersection(const SWRay * ray, const SWVec3 * position, float radius, SWVec3 * out_int_point_a, SWVec3 * out_int_point_b) {
	SWVec3 d, rv;
	float a, b, c, discriminant, discrRoot, r1, r2;
	swVec3Sub(&d, &ray->origin, position);
	a = swVec3Dot(&ray->dir, &ray->dir);
	b = 2.0f * swVec3Dot(&ray->dir, &d);
	c = swVec3Dot(&d, &d) - radius * radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0.0f) return 0;
	discrRoot = sqrt(discriminant);
	/* find roots of quadratic equation */
	r1 = (-b + discrRoot) / 2.0f;
	r2 = (-b - discrRoot) / 2.0f;
	/* write intersection points out if needed */
	if (out_int_point_a) swVec3Add(out_int_point_a, &ray->origin, swVec3Scale(&rv, &ray->dir, r1));
	if (out_int_point_b) swVec3Add(out_int_point_b, &ray->origin, swVec3Scale(&rv, &ray->dir, r2));
    return 1;
}