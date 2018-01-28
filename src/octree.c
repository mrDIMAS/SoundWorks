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

#include "octree.h"

/*=========================================*/
static void swOctreeCountLeafsRecursive(SWOctreeNode * node, int * counter) {
	int i;
	if (node->split) {
		for (i = 0; i < 8; ++i) {
			swOctreeCountLeafsRecursive(node->children[i], counter);
		}
	} else {
		(*counter)++;
	}
}

/*=========================================*/
int swOctreeCountLeafs(SWOctree * octree) {
	int counter = 0;
	swOctreeCountLeafsRecursive(octree->root, &counter);
	return counter;
}

/*=========================================*/
static void swOctreeAllocTraceBuffers(SWOctree * octree) {
	int i;
	int leafCount = swOctreeCountLeafs(octree);
	for (i = 0; i < SW_OCTREE_SIMULTANEOUS_TRACES; ++i) {
		octree->trace_buffer[i].nodes = calloc(leafCount, sizeof(*octree->trace_buffer[i].nodes));
		octree->trace_buffer[i].size = 0;
	}
}

/*=========================================*/
static void swOctreeNodeFree(SWOctreeNode * node) {
	int i;
	if (node->split) {
		for (i = 0; i < 8; ++i) {
			swOctreeNodeFree(node->children[i]);
		}
	}
	free(node->indices);
	free(node);
}

/*=========================================*/
void swOctreeFree(SWOctree * octree) {
	int i;
	swOctreeNodeFree(octree->root);
	for (i = 0; i < SW_OCTREE_SIMULTANEOUS_TRACES; ++i) {
		free(octree->trace_buffer[i].nodes);
	}
}

/*=========================================*/
static void swOctreeRayTraceRecursive(SWOctree * octree, SWOctreeNode * node, const SWRay * ray, int threadNum) {
	int i;
	if(swRayAabbIntersection(ray, &node->aabb, NULL, NULL)) {
		if (node->split) {
			for (i = 0; i < 8; i++) {
				swOctreeRayTraceRecursive(octree, node->children[i], ray, threadNum);
			}
		} else {
			octree->trace_buffer[threadNum].nodes[octree->trace_buffer[threadNum].size++] = node;
		}
	}
}

/*=========================================*/
void swOctreeRayTrace(SWOctree * octree, const SWRay * ray, int threadNum) {
	octree->trace_buffer[threadNum].size = 0;
	swOctreeRayTraceRecursive(octree, octree->root, ray, threadNum);
}

/*=========================================*/
void swOctreeSphereTraceRecursive(SWOctree * octree, SWOctreeNode * node, const SWVec3 * position, float radius) {
	int i;
	if (swAabbSphereIntersection(&node->aabb, NULL, position, radius)) {
		if (node->split) {
			for (i = 0; i < 8; i++) {
				swOctreeSphereTraceRecursive(octree, node->children[i], position, radius);
			}
		} else {
			octree->trace_buffer[0].nodes[octree->trace_buffer[0].size++] = node;
		}
	}
}

/*=========================================*/
void swOctreeSphereTrace(SWOctree * octree, const SWVec3 * position, float radius) {
	octree->trace_buffer[0].size = 0;
	swOctreeSphereTraceRecursive(octree, octree->root, position, radius);
}

/*=========================================*/
static void swOctreeNodeSplit(SWOctreeNode * node) {
	int i;
	SWVec3 center, vmin, vmax;	
	swVec3Middle(&center, &node->aabb.min, &node->aabb.max);

	for (i = 0; i < 8; i++) {
		node->children[i] = calloc(1, sizeof(SWOctreeNode));
	}

	swAabbSet(&node->children[0]->aabb, swVec3Set(&vmin, node->aabb.min.x, node->aabb.min.y, node->aabb.min.z), swVec3Set(&vmax, center.x, center.y, center.z));
	swAabbSet(&node->children[1]->aabb, swVec3Set(&vmin, center.x, node->aabb.min.y, node->aabb.min.z), swVec3Set(&vmax, node->aabb.max.x, center.y, center.z));
	swAabbSet(&node->children[2]->aabb, swVec3Set(&vmin, node->aabb.min.x, node->aabb.min.y, center.z), swVec3Set(&vmax, center.x, center.y, node->aabb.max.z));
	swAabbSet(&node->children[3]->aabb, swVec3Set(&vmin, center.x, node->aabb.min.y, center.z), swVec3Set(&vmax, node->aabb.max.x, center.y, node->aabb.max.z));
	swAabbSet(&node->children[4]->aabb, swVec3Set(&vmin, node->aabb.min.x, center.y, node->aabb.min.z), swVec3Set(&vmax, center.x, node->aabb.max.y, center.z));
	swAabbSet(&node->children[5]->aabb, swVec3Set(&vmin, center.x, center.y, node->aabb.min.z), swVec3Set(&vmax, node->aabb.max.x, node->aabb.max.y, center.z));
	swAabbSet(&node->children[6]->aabb, swVec3Set(&vmin, node->aabb.min.x, center.y, center.z), swVec3Set(&vmax, center.x, node->aabb.max.y, node->aabb.max.z));
	swAabbSet(&node->children[7]->aabb, swVec3Set(&vmin, center.x, center.y, center.z), swVec3Set(&vmax, node->aabb.max.x, node->aabb.max.y, node->aabb.max.z));

	node->split = 1;
}

/*=========================================*/
static void swOctreeBuildRecursive(SWOctreeNode * node, const SWVec3 * vertices, int * indices, int indexCount, int maxTrianglesPerNode) {
	int childNum, i;
	if (indexCount < maxTrianglesPerNode) {
		int sizeBytes = sizeof(int) * indexCount;
		node->indexCount = indexCount;
		node->indices = malloc(sizeBytes);
		memcpy(node->indices, indices, sizeBytes);
	} else {	
		swOctreeNodeSplit(node);		
		for (childNum = 0; childNum < 8; childNum++) {
			SWOctreeNode * child = node->children[childNum];
			int * leafIndices = NULL;
			int leafIndexCount = 0;		
			for (i = 0; i < indexCount; i += 3) {
				const SWVec3 * a = &vertices[indices[i]];
				const SWVec3 * b = &vertices[indices[i + 1]];
				const SWVec3 * c = &vertices[indices[i + 2]];
				if(swAabbTriangleIntersection(&child->aabb, a, b, c)) {
					leafIndexCount += 3;
					leafIndices = realloc( leafIndices, leafIndexCount * sizeof( *leafIndices ));
					leafIndices[ leafIndexCount - 3 ] = indices[i];	
					leafIndices[ leafIndexCount - 2 ] = indices[i + 1];
					leafIndices[ leafIndexCount - 1 ] = indices[i + 2];
				}
			}
			swOctreeBuildRecursive(child, vertices, leafIndices, leafIndexCount, maxTrianglesPerNode);
			free(leafIndices);
		}		
	}
}

/*=========================================*/
SWOctree * swOctreeBuild(SWOctree * octree, const SWVec3 * vertices, int * indices, int indexCount, int maxTrianglesPerNode) {
	int i;
	memset(octree, 0, sizeof(*octree));
	octree->root = calloc(1, sizeof(SWOctreeNode));
	swVec3Set(&octree->root->aabb.min, FLT_MAX, FLT_MAX, FLT_MAX);
	swVec3Set(&octree->root->aabb.max, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (i = 0; i < indexCount; ++i) {
		swVec3MinMax(&vertices[indices[i]], &octree->root->aabb.min, &octree->root->aabb.max);
	}
	swAabbRecomputeCorners(&octree->root->aabb);
	swOctreeBuildRecursive(octree->root, vertices, indices, indexCount, maxTrianglesPerNode);
	swOctreeAllocTraceBuffers(octree);
	return octree;
}