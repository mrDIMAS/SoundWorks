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

#include "../sndwrk.h"
#include <ctype.h>
#include "SDL2/SDL.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "thirdparty/glext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* gui */
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_SDL_GL2_IMPLEMENTATION
#define NK_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_gl2.h"

SDL_Window * window;
SDL_GLContext glcontext;
struct nk_context * nkgui;

SWOutputDevice * dev;
SWEffect * reverb;


#define soundCount 4
#define PI 3.14159

typedef struct Face {
	short va, vb, vc;
	short ta, tb, tc;
} Face;

typedef struct TexCoord_s {
	float x, y;
} TexCoord;

typedef struct Model {
	SWVec3 * vertices;
	int vertexCount;
	TexCoord * texcoords;
	int texcoordCount;
	Face * faces;
	int faceCount;
} Model;

typedef struct Player {
	SWVec3 position;
	float height, yaw, pitch;
	SWVec3 lookAt;
	SWVec3 right;
	SWBool move;
	SWBool strafe;
	SWSound * footsteps[soundCount];
	float pathLen;
} Player;

Player g_player;
SWBool keys[255];

static GLuint LoadTextureFromTGA(const char * file) {
	int width, height, bytesPerPixel, bytesCount;
	unsigned char tgaHeader[12], tgaInfo[6];
	char * pixels;
	GLuint texture;
	FILE * tgaFile = fopen(file, "rb");
	if(!tgaFile) return 0;
	fread(tgaHeader, sizeof(tgaHeader), 1, tgaFile);
	fread(tgaInfo, sizeof(tgaInfo), 1, tgaFile);
	width = tgaInfo[1] * 256 + tgaInfo[0];
	height = tgaInfo[3] * 256 + tgaInfo[2];
	bytesPerPixel = tgaInfo[4] / 8;
	if((width <= 0) || (height <= 0) || ((bytesPerPixel != 3) && (bytesPerPixel != 4))) return 0;
	bytesCount = bytesPerPixel * width * height;
	pixels = malloc(bytesCount);
	if(fread(pixels, 1, bytesCount, tgaFile) != bytesCount) return 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, (bytesPerPixel == 3) ? GL_RGB : GL_RGBA, width, height, 0, (bytesPerPixel == 3) ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	free(pixels);
	fclose(tgaFile);
	return texture;
}

Model * LoadModelFromObj(const char * filename) {
	int n;
	Model * model;
	FILE * f = fopen(filename, "r");
	if(!f) return NULL;
	model = calloc(1, sizeof(Model));
	while(SW_TRUE) {
		char header[1024];
		if(fscanf(f, "%s", header) == EOF) {
			break;
		}
		if(strcmp(header, "v") == 0) {
			model->vertexCount++;
			model->vertices = realloc(model->vertices, model->vertexCount * sizeof(SWVec3));
			fscanf(f, "%f %f %f\n", &model->vertices[ model->vertexCount - 1 ].x, &model->vertices[ model->vertexCount - 1 ].y, &model->vertices[ model->vertexCount - 1 ].z);
		} else if(strcmp(header, "f") == 0) {
			int dummy, v0, v1, v2, t0, t1, t2;
			if(fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v0, &t0, &dummy, &v1, &t1, &dummy, &v2, &t2, &dummy) != 9) {
				return NULL;
			}
			n = model->faceCount;
			model->faceCount++;
			model->faces = realloc(model->faces, model->faceCount * sizeof(Face));
			model->faces[n].va = v0 - 1;
			model->faces[n].vb = v1 - 1;
			model->faces[n].vc = v2 - 1;
			model->faces[n].ta = t0 - 1;
			model->faces[n].tb = t1 - 1;
			model->faces[n].tc = t2 - 1;
		} else if(strcmp(header, "vt") == 0) {
			float dummy;
			model->texcoordCount++;
			model->texcoords = realloc(model->texcoords, model->texcoordCount * sizeof(TexCoord));
			fscanf(f, "%f %f %f\n", &model->texcoords[ model->texcoordCount - 1 ].x, &model->texcoords[ model->texcoordCount - 1 ].y, &dummy);
		}
	}
	return model;
}

void DrawModel(Model * mdl) {
	int i;
	glBegin(GL_TRIANGLES);

	for(i = 0; i < mdl->faceCount; ++i) {
		glTexCoord2fv(&mdl->texcoords[mdl->faces[i].ta].x);
		glVertex3fv(&mdl->vertices[mdl->faces[i].va].x);

		glTexCoord2fv(&mdl->texcoords[mdl->faces[i].tb].x);
		glVertex3fv(&mdl->vertices[mdl->faces[i].vb].x);

		glTexCoord2fv(&mdl->texcoords[mdl->faces[i].tc].x);
		glVertex3fv(&mdl->vertices[mdl->faces[i].vc].x);
	}

	glEnd();
}

void FreeModel(Model * mdl) {
	free(mdl->vertices);
	free(mdl->faces);
}

void InitGraphics() {
	struct nk_font_atlas *atlas;
	struct nk_color table[NK_COLOR_COUNT];
	SDL_DisplayMode mode;
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	window = SDL_CreateWindow("SoundWorks Test Suite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mode.w * 0.9, mode.h * 0.9, SDL_WINDOW_OPENGL);
	glcontext = SDL_GL_CreateContext(window);
	/*SDL_SetRelativeMouseMode(SDL_TRUE);*/
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPointSize(10);
	
	/* init nuklear gui*/
	nkgui = nk_sdl_init(window);
	
	nk_sdl_font_stash_begin(&atlas);
	nk_sdl_font_stash_end();	
	
	
	table[NK_COLOR_TEXT] = nk_rgba(190, 190, 190, 255);
	table[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
	table[NK_COLOR_HEADER] = nk_rgba(181, 45, 69, 220);
	table[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 255);
	table[NK_COLOR_BUTTON] = nk_rgba(181, 45, 69, 255);
	table[NK_COLOR_BUTTON_HOVER] = nk_rgba(190, 50, 70, 255);
	table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(195, 55, 75, 255);
	table[NK_COLOR_TOGGLE] = nk_rgba(51, 55, 67, 255);
	table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 60, 60, 255);
	table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(181, 45, 69, 255);
	table[NK_COLOR_SELECT] = nk_rgba(51, 55, 67, 255);
	table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(181, 45, 69, 255);
	table[NK_COLOR_SLIDER] = nk_rgba(51, 55, 67, 255);
	table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(181, 45, 69, 255);
	table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(186, 50, 74, 255);
	table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(191, 55, 79, 255);
	table[NK_COLOR_PROPERTY] = nk_rgba(51, 55, 67, 255);
	table[NK_COLOR_EDIT] = nk_rgba(51, 55, 67, 225);
	table[NK_COLOR_EDIT_CURSOR] = nk_rgba(190, 190, 190, 255);
	table[NK_COLOR_COMBO] = nk_rgba(51, 55, 67, 255);
	table[NK_COLOR_CHART] = nk_rgba(51, 55, 67, 255);
	table[NK_COLOR_CHART_COLOR] = nk_rgba(170, 40, 60, 255);
	table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
	table[NK_COLOR_SCROLLBAR] = nk_rgba(30, 33, 40, 255);
	table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
	table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
	table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
	table[NK_COLOR_TAB_HEADER] = nk_rgba(181, 45, 69, 220);
	nk_style_from_table(nkgui, table);
}

void InitPlayer() {
	int i;
	const char * footsteps[soundCount] = {
		"data/sounds/FootStep_shoe_metal_step1.ogg", "data/sounds/FootStep_shoe_metal_step2.ogg",
		"data/sounds/FootStep_shoe_metal_step3.ogg", "data/sounds/FootStep_shoe_metal_step4.ogg"
	};

	g_player.height = 5;

	/* load footsteps sounds */
	for(i = 0; i < soundCount; i++) {
		SWBuffer * buffer = swBufferCreateFromFile(footsteps[i], SW_BUFFER_FULL);
		g_player.footsteps[i] = swSoundCreate(buffer);
		swSoundSet3D(g_player.footsteps[i], SW_TRUE);
		swSoundSetLooping(g_player.footsteps[i], SW_FALSE);
		swSoundSetEffect(g_player.footsteps[i], reverb);
		swSoundSetRadius(g_player.footsteps[i], 50);
		swSoundSetEffectRadius(g_player.footsteps[i], 120);
	}
}

void UpdatePlayer() {
	int i;
	SWVec3 up = {0, 1, 0};
	SWVec3 v;
	if(g_player.pitch < -89.0f) g_player.pitch = -89.0f;
	if(g_player.pitch > 89.0f) g_player.pitch = 89.0f;

	g_player.lookAt.x = -sin(g_player.yaw/180*PI);
	g_player.lookAt.y = tan(g_player.pitch/180*PI);
	g_player.lookAt.z = -cos(g_player.yaw/180*PI);

	swVec3Cross(&g_player.right, &g_player.lookAt, &up);

	if(keys[SDLK_w]) {
		g_player.position.x += g_player.lookAt.x;
		g_player.position.y += g_player.lookAt.y;
		g_player.position.z += g_player.lookAt.z;

		g_player.pathLen += swVec3Len(&g_player.lookAt);
	}
	if(keys[SDLK_s]) {
		g_player.position.x -= g_player.lookAt.x;
		g_player.position.y -= g_player.lookAt.y;
		g_player.position.z -= g_player.lookAt.z;

		g_player.pathLen += swVec3Len(&g_player.lookAt);
	}
	if(keys[SDLK_a]) {
		g_player.position.x -= g_player.right.x;
		g_player.position.z -= g_player.right.z;

		g_player.pathLen += swVec3Len(&g_player.right);
	}
	if(keys[SDLK_d]) {
		g_player.position.x += g_player.right.x;
		g_player.position.z += g_player.right.z;

		g_player.pathLen += swVec3Len(&g_player.right);
	}

	if(g_player.pathLen > 15) {
		int soundNum = rand() % soundCount;
		while(swSoundIsPlaying(g_player.footsteps[soundNum])) {
			soundNum = rand() % soundCount;
		}
		swSoundPlay(g_player.footsteps[ soundNum ]);
		g_player.pathLen = 0.0f;
	}
	
	for(i = 0; i < soundCount; ++i) {
		swSoundSetPosition(g_player.footsteps[ i ], swVec3Add(&v, &g_player.position, &g_player.lookAt ));
	}

	swListenerSetOrientation(&up, &g_player.lookAt);
	swListenerSetPosition(&g_player.position);
}

void InitSoundWorks() {
	dev = swDeviceCreate(8000);
	swMixerCreate(dev);
	swMixerStart();
}

float GetTime() {
	return clock() / (float)CLOCKS_PER_SEC;
}

int main(int argc, char **argv) {
	SWVec3 v, musPos;
	float gameClock = GetTime();
	float soundClock = GetTime();
	float fixedTimeStep = 1.0f / 60.0f;
	float fixedTimeStepSound = 1.0f / 16.0f; /* update sound 16 times per second */
	SDL_Event event;
	Model * scene;
	GLuint texture;
	SWBuffer * musicBuffer;
	SWSound * music;
	SWHead * head;

	InitGraphics();
	InitSoundWorks();
	
	head = swHeadLoad("data/hrtf/head.hrtf");
	swListenerSetHead(head);

	scene = LoadModelFromObj("data/models/scene.obj");
	texture = LoadTextureFromTGA("data/textures/wall.tga");

	reverb = swEffectCreate(SW_EFFECT_REVERB);

	InitPlayer();

	/* begin music streaming */
	/*
	music_buffer = swBufferCreateFromFile("data/sounds/music3.ogg", SW_BUFFER_STREAM);
	music = swSoundCreate(music_buffer);
	swSoundSetLooping(music, SW_TRUE);
	swSoundSet3D(music, SW_TRUE);
	swSoundSetEffect(music, reverb);
	swSoundSetPosition(music, swVec3Set(&v, 0, 5, 0));
	swSoundSetRadius(music, 50);
	swSoundSetEffectRadius(music, 120);*/
	
	musicBuffer = swBufferCreateFromFile("data/sounds/fire2.ogg", SW_BUFFER_FULL);
	music = swSoundCreate(musicBuffer);
	swSoundSetLooping(music, SW_TRUE);
	swSoundSet3D(music, SW_TRUE);
	swSoundSetEffect(music, reverb);
	swSoundSetPosition(music, swVec3Set(&v, 0, 5, 0));
	swSoundSetRadius(music, 50);
	swSoundSetEffectRadius(music, 120);

	while(swMixerIsActive()) {
		float dt = GetTime() - gameClock;
		float soundDt = GetTime() - soundClock;
		if(soundDt >= fixedTimeStepSound) {
			swUpdate();
			soundClock = GetTime();
		}

		while(dt >= fixedTimeStep) {			
			dt -= fixedTimeStep;
			gameClock += fixedTimeStep;

			if(keys[SDLK_ESCAPE]) {
				swMixerStop();
			}			
			UpdatePlayer();
		}
		
		nk_input_begin(nkgui);
		while(SDL_PollEvent(&event)) {
			nk_sdl_handle_event(&event);
			if(event.type == SDL_QUIT) {
				swMixerStop();
			} else if(event.type == SDL_KEYUP) {
				keys[event.key.keysym.sym] = SW_FALSE;
			} else if(event.type == SDL_KEYDOWN) {
				keys[event.key.keysym.sym] = SW_TRUE;
			} else if(event.type == SDL_MOUSEMOTION) {
				g_player.pitch -= event.motion.yrel * 0.1;
				g_player.yaw -= event.motion.xrel * 0.1;					
			}
		}
		nk_input_end(nkgui);
		
		/* rendering goes here */
		glClearColor(0.4, 0.5, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(70, 1, 0.01, 1024);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		gluLookAt(
		    g_player.position.x,
		    g_player.position.y + g_player.height,
		    g_player.position.z,
		    g_player.position.x + g_player.lookAt.x,
		    g_player.position.y + g_player.height + g_player.lookAt.y,
		    g_player.position.z + g_player.lookAt.z,
		    0, 1, 0 );

		glBindTexture(GL_TEXTURE_2D, 0);

		swSoundGetPosition(music, &musPos);
		glBegin(GL_POINTS);
		glVertex3fv(&musPos.x);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture);
		DrawModel(scene);
		
		 
		
		/* Draw GUI using nuklear library */
		if(nk_begin(nkgui, "SoundWorks Demo", nk_rect(0, 0, 300, 400), NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {		
			/* Sound Properties Tab */
			if (nk_tree_push(nkgui, NK_TREE_TAB, "Sound Properties", NK_MAXIMIZED)) {
				nk_layout_row_dynamic(nkgui, 30, 3);
				if(nk_button_label(nkgui, "Play")) {
					swSoundPlay(music);
				}
				if(nk_button_label(nkgui, "Pause")) {
					swSoundPause(music);
				}
				if(nk_button_label(nkgui, "Stop")) {
					swSoundStop(music);
				}
				
				
				nk_tree_pop(nkgui);
			}	
			
			/* Listener Properties Tab */
			if (nk_tree_push(nkgui, NK_TREE_TAB, "Listener Properties", NK_MAXIMIZED)) {
				SWVec3 position;
				static int useAdvancedHRTF = SW_TRUE;
				
				swListenerGetPosition(&position);
				nk_labelf(nkgui, NK_TEXT_LEFT, "Position: (%.2f; %.2f; %.2f)", position.x, position.y, position.z);
								
				if(nk_checkbox_label(nkgui, "Use Head-Related Transfer Function?", &useAdvancedHRTF)) {
					if(useAdvancedHRTF) {
						swListenerSetHead(head);
					} else {
						swListenerSetHead(NULL);
					}
				}
				
				nk_tree_pop(nkgui);
			}	
			
			/* Reverb Properties Tab */
			if (nk_tree_push(nkgui, NK_TREE_TAB, "Reverb Properties", NK_MAXIMIZED)) {
				static float lateDiffusion = 0.05f;
				static float reverbTime = 0.05f;
				static int reverbEnabled = SW_TRUE;
				
				nk_layout_row_dynamic(nkgui, 30, 2);		
				if(nk_slider_float(nkgui, 0.05, &reverbTime, 10.0, 0.05)) {
					swReverbSetDecayTime(reverb, reverbTime);
				}	
				nk_labelf(nkgui, NK_TEXT_LEFT, "Reverb Time, %.2f s", reverbTime);
				if(nk_slider_float(nkgui, 0.05, &lateDiffusion, 0.45, 0.05)) {
					swReverbSetLateDiffusion(reverb, lateDiffusion);
				}
				nk_labelf(nkgui, NK_TEXT_LEFT, "Late Diffusion, %.2f f", lateDiffusion);
				
				nk_layout_row_dynamic(nkgui, 30, 1);	
				nk_checkbox_label(nkgui, "Reverb Effect Enabled?", &reverbEnabled);
				swEffectSetEnabled(reverb, reverbEnabled);
				nk_tree_pop(nkgui);
			}
			
			/* Mixer Properties Tab */
			if (nk_tree_push(nkgui, NK_TREE_TAB, "Mixer Properties", NK_MAXIMIZED)) {
				nk_layout_row_dynamic(nkgui, 30, 1);
				nk_labelf(nkgui, NK_TEXT_LEFT, "Mixing time: %.3f ms", swMixerGetFrameTimeSeconds() * 1000.0f);
				
				nk_layout_row_dynamic(nkgui, 30, 2);	
				if(nk_button_label(nkgui, "Suspend")) {
					swMixerSuspend();
				}
				if(nk_button_label(nkgui, "Process")) {
					swMixerProcess();
				}
				nk_tree_pop(nkgui);
			}							

			/* Output Device Properties Tab */
			if (nk_tree_push(nkgui, NK_TREE_TAB, "Output Device Properties", NK_MAXIMIZED)) {
				nk_layout_row_dynamic(nkgui, 30, 1);	
				nk_labelf(nkgui, NK_TEXT_LEFT, "Buffer Size: %d", swGetHalfBufferSize(dev));
				nk_tree_pop(nkgui);
			}			
		}	
		nk_end(nkgui);		
		
		nk_sdl_render(NK_ANTI_ALIASING_ON);
		SDL_GL_SwapWindow(window);
	}

	swMixerStop();
	swMixerFree();
	swDeviceDestroy(dev);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
