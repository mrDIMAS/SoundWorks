#pragma once

#include "../vec3.h"

typedef struct SWGraphEdge {
    struct SWGraphVertex * destVertex;
    float distToDestVertex;
} SWGraphEdge;

typedef struct SWGraphVertex {
    int used;
    float distanceFromBegin;
    struct SWGraphVertex * ancestor;    
    SWGraphEdge * edges;
    int edgeCount;
    SWVec3 position;
} SWGraphVertex;

typedef struct SWPathfinder {
    SWGraphVertex * graphVertices;
    int graphVertexCount;
} SWPathfinder;

typedef struct SWPath {
    SWVec3 * points;
    int pointCount;
} SWPath;

SWPathfinder * swPathfinderInit(SWPathfinder * pf);
void swPathfinderClear(SWPathfinder * pf);
SWGraphVertex * swPathfinderGetNearestVertex(SWPathfinder * pf, const SWVec3 * position );
void swPathfinderBuildPath(SWPathfinder * pf, const SWVec3 * beginPoint, const SWVec3 * endPoint, SWPath * outPath);
void swPathClear(SWPath * path);
void swPathAddPoint(SWPath * path, const SWVec3 * point);
void swGraphVertexInit(SWGraphVertex * v, const SWVec3 * position);
void swGraphVertexClearState(SWGraphVertex * v);
void swGraphVertexAddEgde(SWGraphVertex * v, SWGraphVertex * vertex);