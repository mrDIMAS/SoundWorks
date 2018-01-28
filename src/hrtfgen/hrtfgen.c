#include "../common.h"
#include "../memory.h"
#include <ctype.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "thirdparty/glext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "path.h"

typedef struct SWFace {
	int va, vb, vc;
} SWFace;

typedef struct SWModel {
	SWVec3 * vertices;
	int vertexCount; 
	SWFace * faces;
	int faceCount;
} SWModel;

static SDL_Window * window;
static SDL_GLContext glcontext;
static SWModel * model;
static int resW = 1024;
static int resH = 768;
static SWVec3 leftEar = { -0.061f, 0.0f, 0.0f }; /* left ear position - in meters */
static SWVec3 rightEar = { 0.059f, 0.0f, 0.0f }; /* right ear position - in meters */
static float speedOfSound = 343.0f; /* m/s */
static float skinWaveDamping = 8.0f; /* m^-1 */
static float approxDistanceBewteenEars = 0.43f; /* m */

/*=========================================*/
SWModel * swLoadModel(const char * filename) {
	int n;
	SWModel * model;
	FILE * f = fopen(filename, "r");
	if(!f) {
		return NULL;
	}
	model = SW_NEW(SWModel);
	while(SW_TRUE) {
		char header[1024];
		if(fscanf(f, "%s", header) == EOF) {
			break;
		}
		if(strcmp(header, "v") == 0) {
			model->vertexCount++;
			model->vertices = swRealloc(model->vertices, model->vertexCount * sizeof(SWVec3));
			fscanf(f, "%f %f %f\n", &model->vertices[ model->vertexCount - 1 ].x, &model->vertices[ model->vertexCount - 1 ].y, &model->vertices[ model->vertexCount - 1 ].z);
		} else if(strcmp(header, "f") == 0) {
			int dummy, v0, v1, v2, t0, t1, t2;
			if(fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v0, &t0, &dummy, &v1, &t1, &dummy, &v2, &t2, &dummy) != 9) {
				return NULL;
			}
			n = model->faceCount;
			model->faceCount++;
			model->faces = swRealloc(model->faces, model->faceCount * sizeof(SWFace));
			model->faces[n].va = v0 - 1;
			model->faces[n].vb = v1 - 1;
			model->faces[n].vc = v2 - 1;
		}
	}

	return model;
}

/*=========================================*/
void swFreeModel(SWModel * model) {
	swFree(model->vertices);
	swFree(model->faces);
	swFree(model);
}

/*=========================================*/
float swGetTime() {
	return clock() / (float)CLOCKS_PER_SEC;
}

/*=========================================*/
void DrawLine(const SWVec3 * begin, const SWVec3 * end, unsigned char r, unsigned char g, unsigned char b) {
	glColor3ub(r, g, b);
	glVertex3f(begin->x, begin->y, begin->z);
	glVertex3f(end->x, end->y, end->z);
}

/*=========================================*/
void DrawPath(const SWPath * path, unsigned char r, unsigned char g, unsigned char b) {
	int i;
	glLineWidth(2.5);
	glBegin(GL_LINES);
	for(i = 0; i < path->pointCount - 1; ++i) {
		DrawLine(&path->points[i], &path->points[i + 1], r, g, b);
	}
	glEnd();
}

/*=========================================*/
void DrawPoint(const SWVec3 * p, unsigned char r, unsigned char g, unsigned char b) {
	glPointSize(3);
	glColor3ub(r, g, b);
	glBegin(GL_POINTS);
	glVertex3f(p->x, p->y, p->z);
	glEnd();
}

/*=========================================*/
void DrawModel(SWModel * mdl) {
	int i;
	glBegin(GL_TRIANGLES);
	glColor3ub(160, 160, 160);
	for(i = 0; i < mdl->faceCount; ++i) {
		glVertex3fv(&mdl->vertices[mdl->faces[i].va].x);
		glVertex3fv(&mdl->vertices[mdl->faces[i].vb].x);
		glVertex3fv(&mdl->vertices[mdl->faces[i].vc].x);
	}
	glEnd();
}

/*=========================================*/
void InitGraphics() {
	SDL_DisplayMode mode;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GetDesktopDisplayMode(0, &mode);
	window = SDL_CreateWindow("SoundWorks Head-Related Transfer Function(HRTF) Builder", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resW, resH, SDL_WINDOW_OPENGL);
	glcontext = SDL_GL_CreateContext(window);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPointSize(10);
}

/*=========================================*/
void BeginFrame(SWVec3 * eyePos) {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
	}

	/* rendering goes here */
	glClearColor(0.4, 0.5, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (float)resW / (float)resH, 0.01, 1024);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eyePos->x, eyePos->y, eyePos->z, 0, 0, 0, 0, 1, 0);

	DrawModel(model);
}

/*=========================================*/
void EndFrame(void) {
	SDL_GL_SwapWindow(window);
}

/*=========================================*/
SWBool DoRayTrace(const SWRay * ray, SWVec3 * v) {
	int i;
	SWVec3 closestPoint = {FLT_MAX, FLT_MAX, FLT_MAX};
	SWBool gotIntersection = SW_FALSE;
	for(i = 0; i < model->faceCount; ++i) {
		SWVec3 intersection;
		SWVec3 * a = &model->vertices[model->faces[i].va];
		SWVec3 * b = &model->vertices[model->faces[i].vb];
		SWVec3 * c = &model->vertices[model->faces[i].vc];

		if(swRayTriangleIntersection(ray, a, b, c, &intersection)) {
			gotIntersection = SW_TRUE;
			if(swVec3SqrDistance(&intersection, &ray->origin) < swVec3SqrDistance(&closestPoint, &ray->origin)) {
				closestPoint = intersection;
			}
		}
	}	
	*v = closestPoint;
	DrawPoint(v, 0, 255, 0);
	return gotIntersection;
}

/*=========================================*/
float DoPathTrace(SWPathfinder * pf, const SWVec3 * from, const SWVec3 * to, unsigned char r, unsigned char g, unsigned char b) {
	SWPath path = {0};
	float pathLen = 0;
	int j;

	swPathfinderBuildPath(pf, from, to, &path);
	pathLen = 0;
	for(j = 1; j < path.pointCount; ++j) {
		pathLen += swVec3Distance(&path.points[j - 1], &path.points[j]);
	}	
	DrawPath(&path, r, g, b);
	swPathClear(&path);
	
	return pathLen;
}

/*=========================================*/
int main(int argc, char ** argv) {
	int theta;
	int phi;
	const float rad = 1.2f; /* this distance (in meters) should perform ray-casting from "far field" */
	int i;
	SWPathfinder pf;
	FILE * hrtfFile;
	char * headModel = "head.obj";
	char * hrtfOut = "head.hrtf";
	float percentDone = 0.0f;
	int iterationsDone = 0;

	swPathfinderInit(&pf);
	InitGraphics();

	hrtfFile = fopen(hrtfOut, "w");

	printf("*** SoundWorks HRTF Builder v0.1 ***\n");

	printf("> Loading model %s...\n", headModel);
	model = swLoadModel(headModel);

	if(!model) {
		printf("Failed to load model %s\n", headModel);
		return 1;
	}

	/* create vertices */
	printf("> Initializing graph vertices...\n");
	pf.graphVertexCount = model->vertexCount;
	pf.graphVertices = swAlloc(pf.graphVertexCount * sizeof(*pf.graphVertices));
	for(i = 0; i < model->vertexCount; ++i) {
		swGraphVertexInit(&pf.graphVertices[i], &model->vertices[i]);
	}

	/* link vertices */
	printf("> Vertex linkage...\n");
	for(i = 0; i < model->faceCount; ++i) {
		SWFace * face = &model->faces[i];
		swGraphVertexAddEgde(&pf.graphVertices[face->va], &pf.graphVertices[face->vb]);
		swGraphVertexAddEgde(&pf.graphVertices[face->vb], &pf.graphVertices[face->vc]);
		swGraphVertexAddEgde(&pf.graphVertices[face->vc], &pf.graphVertices[face->va]);
	}

	/* begin ray tracing using spherical coordinates */
	printf("> Starting ray-tracing and path-tracing...\n");
	for(phi = 0; phi < 180; ++phi) {
		for(theta = 0; theta < 360; ++theta) {
			SWRay rayLeft, rayRight;
			SWVec3 p;
			SWVec3 leftRayIntPoint, rightRayIntPoint;
			float phiRad = swDegToRad(phi);
			float thetaRad = swDegToRad(theta);
			/* convert spherical to cartesian assuming that z looks toward screen, y - up, x - right
			   this is the same coordinate system that default in SoundWorks
			*/
			p.x = rad * sin(phiRad) * sin(thetaRad);
			p.y = rad * cos(phiRad);
			p.z = rad * sin(phiRad) * cos(thetaRad);
			BeginFrame(&p);
			/* create rays from point on the sphere to each ear */
			swRayByTwoPoints(&rayLeft, &leftEar, &p);
			swRayByTwoPoints(&rayRight, &rightEar, &p);
			/* do ray tracing to each ear */
			if(DoRayTrace(&rayLeft, &leftRayIntPoint) && DoRayTrace(&rayRight, &rightRayIntPoint)) {
				float leftPathLength = DoPathTrace(&pf, &leftRayIntPoint, &leftEar, 255, 0, 0);
				float rightPathLength = DoPathTrace(&pf, &rightRayIntPoint, &rightEar, 0, 0, 255);
				/* compute phase shift for each ear */
				float leftShift = leftPathLength / speedOfSound;
				float rightShift = rightPathLength / speedOfSound;
				/* compute pole parameters for one-pole filter for each ear */				
				float leftPole = (leftPathLength / approxDistanceBewteenEars) ;
				float rightPole = (rightPathLength / approxDistanceBewteenEars);
				/* compute gain for each ear */
				float leftGain = (1.0f - exp(-skinWaveDamping * leftPathLength)) * 0.5f;
				float rightGain = (1.0f - exp(-skinWaveDamping * rightPathLength)) * 0.5f;			
				++iterationsDone;
				percentDone = 100.0f *  ((float)(iterationsDone) / (180.0f * 360.0f));				
				printf("\r> %.1f%% ", percentDone);
				fprintf(hrtfFile, "%d %d %f %f %f %f %f %f\n", phi, theta, leftShift, rightShift, leftPole, rightPole, leftGain, rightGain);
			} else {
				printf("\r> (Phi:%d;Theta:%d) !!! Ray tracing failed !!!", phi, theta);
			}
			EndFrame();
		}
	}
	fclose(hrtfFile);

	swFreeModel(model);

	return 0;
}
