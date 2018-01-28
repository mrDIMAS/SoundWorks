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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include "memory.h"
#include <string.h>
#include "log.h"
#include "sndwrk.h"

/*=========================================*/
void * swGenericAlloc(int size, SWBool clear) {
	void * block = clear ? calloc(size, 1) : malloc(size);
	if(!block) {
		swLogWrite("Unable to allocate %d bytes. Not enough memory!", size);
	}
	return block;
}

/*=========================================*/
void * swAlloc(int size) {
	return swGenericAlloc(size, SW_FALSE);
}

/*=========================================*/
void * swCAlloc(int size) {
	return swGenericAlloc(size, SW_TRUE);
}

/*=========================================*/
void * swRealloc(void * data, int newSize) {
	void * newData = 0;
	newData = realloc(data, newSize);
	if(!newData) {
		swLogWrite("Memory reallocation failed!");
	}

	return newData;
}

/*=========================================*/
void swFree(void * data) {
	free(data);
}
