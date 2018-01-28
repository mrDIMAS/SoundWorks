#include "path.h"
#include <memory.h>
#include <stdlib.h>
#include <float.h>

SWPathfinder * swPathfinderInit(SWPathfinder * pf) {
	memset(pf, 0, sizeof(*pf));
    return pf;
}

void swPathfinderClear(SWPathfinder * pf) {
    free(pf->graphVertices);
}

SWGraphVertex * swPathfinderGetNearestVertex(SWPathfinder * pf, const SWVec3 * position ) {
	int i;
    SWGraphVertex * nearest = &pf->graphVertices[0];
    for( i = 1; i < pf->graphVertexCount; ++i ) {
		if(swVec3SqrDistance(&pf->graphVertices[i].position, position) < swVec3SqrDistance(&nearest->position, position)) {
			nearest = &pf->graphVertices[i];
		}        
    }
    return nearest;
}

void swPathfinderBuildPath(SWPathfinder * pf, const SWVec3 * beginPoint, const SWVec3 * endPoint, SWPath * outPath) {
	int i, j;
	SWGraphVertex * v;
	SWGraphVertex * begin;
	SWGraphVertex * end;
    swPathClear(outPath);
	
    begin = swPathfinderGetNearestVertex(pf, beginPoint);
    end = swPathfinderGetNearestVertex(pf, endPoint);
    
	if(begin == end) {
        swPathAddPoint(outPath, beginPoint);
		return;
	}

    /* clear state of all vertices */
    for(i = 0; i < pf->graphVertexCount; ++i) {
        swGraphVertexClearState(&pf->graphVertices[i]);
    }

    /* set begin graph vertex */
    begin->distanceFromBegin = 0;

    for( i = 0; i < pf->graphVertexCount; i++ ) {
        /* get nearest vertex */
        SWGraphVertex * nearest = NULL;

        for( j = 0; j < pf->graphVertexCount; ++j ) {
            if( pf->graphVertices[j].used ) {
                continue;
            } else if( !nearest ) {
                nearest = &pf->graphVertices[j];
            } else if( pf->graphVertices[j].distanceFromBegin < nearest->distanceFromBegin ) {
                nearest = &pf->graphVertices[j];
            }
        }

        if( nearest->distanceFromBegin >= FLT_MAX ) {
            break;
        }

        nearest->used = 1;

        /* relaxation */
		for( j = 0; j < nearest->edgeCount; ++j ) {
			SWGraphEdge * edge = &nearest->edges[j];
            if( nearest->distanceFromBegin + edge->distToDestVertex < edge->destVertex->distanceFromBegin ) {
                edge->destVertex->distanceFromBegin = nearest->distanceFromBegin + edge->distToDestVertex;
                edge->destVertex->ancestor = nearest;
            }
        }
    }

    /* restore path to dest vertex */
    for( v = end; v != begin;  ) {
		if( v ) {
            swPathAddPoint(outPath, &v->position);
			v = v->ancestor;
		} else {
			break;
		}		
    }

    swPathAddPoint(outPath, beginPoint);

	/* reverse */
	for( i = 0; i < outPath->pointCount / 2; ++i ) {
		SWVec3 temp = outPath->points[ outPath->pointCount - i - 1 ];
		outPath->points[ outPath->pointCount - i - 1 ] = outPath->points[i];
		outPath->points[i] = temp;
	}
}

void swPathClear(SWPath * path) {
    free(path->points);
    path->points = NULL;
    path->pointCount = 0;
}

void swPathAddPoint(SWPath * path, const SWVec3 * point) {
	path->points = realloc( path->points, (path->pointCount + 1) * sizeof( *path->points ));
	path->points[path->pointCount++] = *point;
}


void swGraphVertexInit(SWGraphVertex * v, const SWVec3 * position) {
	memset(v, 0, sizeof(*v));
    v->distanceFromBegin = FLT_MAX;    
	v->position = *position;
}


void swGraphVertexClearState(SWGraphVertex * v) {
    v->ancestor = NULL;
    v->used = 0;
    v->distanceFromBegin = FLT_MAX;
}

void swGraphVertexAddEgde(SWGraphVertex * v, SWGraphVertex * vertex) {
	v->edges = realloc( v->edges, (v->edgeCount + 1) * sizeof( *v->edges ));
	v->edges[v->edgeCount].destVertex = vertex;
	v->edges[v->edgeCount].distToDestVertex = swVec3Distance(&v->position, &vertex->position);
	++v->edgeCount;
}
