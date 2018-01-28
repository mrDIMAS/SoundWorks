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

#include "listener.h"
#include <time.h>
#include "sound.h"

TListener g_listener;

/*=========================================*/
void swListenerInit(void) {
	g_listener.gain = 1.0f;
	swVec3Zero(&g_listener.pos);
	swVec3Set(&g_listener.up, 0, 1, 0);
	swVec3Set(&g_listener.look, 0, 0, 1);
	swVec3Set(&g_listener.right, 1, 0, 0);
}

/*=========================================*/
void swListenerSetPosition(const SWVec3 * position) {
	g_listener.pos = *position;
}

/*=========================================*/
SWVec3 * swListenerGetPosition(SWVec3 * position) {
	*position = g_listener.pos;
	return position;
}

/*=========================================*/
void swListenerSetOrientation(const SWVec3 * upVector, const SWVec3 * lookVector) {
	swVec3Normalize(&g_listener.up, upVector);
	swVec3Normalize(&g_listener.look, lookVector);
	swVec3Normalize(&g_listener.right, swVec3Cross(&g_listener.right, lookVector, upVector));
}

/*=========================================*/
SWVec3 * swListenerGetOrientationUp(SWVec3 * up) {
	*up = g_listener.up;
	return up;
}

/*=========================================*/
SWVec3 * swListenerGetOrientationLook(SWVec3 * look) {
	*look = g_listener.look;
	return look;
}

/*=========================================*/
SWVec3 * swListenerGetOrientationRight(SWVec3 * right) {
	*right = g_listener.right;
	return right;
}

/*=========================================*/
void swListenerSetGain(float gain) {
	g_listener.gain = gain;
}

float swListenerGetGain(void) {
	return g_listener.gain;
}

/*=========================================*/
void swListenerSetHead(SWHead * head) {
	g_listener.head = head;
}

/*=========================================*/
SWHead * swListenerGetHead(void) {
	return g_listener.head;
}
