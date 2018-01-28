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

#include "decoder_ogg.h"
#include "list.h"
#include "memory.h"
#include "buffer.h"

SWBuffer * bufferList;

#define SW_BUFFER_STREAM_HALF_SIZE (32768)

/*=========================================*/
SWBool swBufferIsValid(SWBuffer * buf) {
	SWBuffer * b;

	for(b = bufferList; b; b = b->next) {
		if(b == buf) return SW_TRUE;
	}

	return SW_FALSE;
}

/*=========================================*/
SWBuffer * swBufferGetList() {
	return bufferList;
}

/*=========================================
- Converts raw integer samples to IEEE floats
- Must be called after SW_Buffer_DecodeNextBlock
=========================================*/
static void swBufferProcessDecodedData(SWBuffer * buf, int count, float * dest) {
	/* convert samples from 16-bit int to IEEE float */
	int i, k;
	int decodedSampleCount = buf->decoder.size / sizeof(int16_t);
	for(i = 0; i < count; i += buf->decoder.channelCount) {
		for(k = 0; k < buf->decoder.channelCount; k++) {
			int index = i + k;
			dest[index] = (index < decodedSampleCount) ? ((int16_t*)buf->decoder.data)[index] : 0;
		}
	}
	buf->frequency = buf->decoder.frequency;
	buf->channelCount = buf->decoder.channelCount;
}

/*=========================================*/
SWBuffer * swBufferCreateFromFile(const char * fileName, SWBufferType type) {
	SWBool decoderPresent = SW_FALSE;
	SWBuffer * buf = SW_NEW(SWBuffer);
	LIST_ADD(bufferList, buf);

	buf->type = type;
	buf->refCount = 0;
	/* -1 indicates that at first time, buffer requires two blocks when doing streaming */
	buf->blockNum = -1;

	/* select decoder */
	if(strstr(fileName, ".ogg")) {
		buf->decoder.Open = swOggDecoderOpenFile;
		buf->decoder.Clean = swOggDecoderClean;
		buf->decoder.Rewind = swOggDecoderRewind;
		buf->decoder.Decode = swOggDecoderDecodeBlock;

		buf->decoder.Open(&buf->decoder, fileName);

		decoderPresent = SW_TRUE;
	} else {
		swLogWrite("%s is not a valid file!", fileName);

		/* in case of error, remove buffer from list and free memory */
		LIST_ERASE(bufferList, buf);
		swFree(buf);
		buf = NULL;
	}

	if(decoderPresent) {
		/* begin decoding */
		if(buf->type == SW_BUFFER_FULL) {
			/* decode entire file */
			buf->decoder.Decode(&buf->decoder, buf->decoder.fullSize);
			/* allocate samples buffer */
			buf->sampleCount = buf->decoder.size / sizeof(int16_t);
			buf->samples = swAlloc(buf->sampleCount * sizeof(float));
			/*/ convert and copy decoded data */
			swBufferProcessDecodedData(buf, buf->sampleCount, buf->samples);
			buf->decoder.Clean(&buf->decoder);
		} else if(buf->type == SW_BUFFER_STREAM) {
			/* allocate samples buffer */
			buf->sampleCount = SW_BUFFER_STREAM_HALF_SIZE / sizeof(int16_t);
			buf->samples = swAlloc(2 * buf->sampleCount * sizeof(float));
			swBufferGetNextBlock(buf);
		}
	}

	return buf;
}

/*=========================================*/
SWBuffer * swBufferCreateFromMemory(char * ptr, int sizeBytes, int channels, int frequency) {
	int i, k;
	SWBuffer * buf = SW_NEW(SWBuffer);
	LIST_ADD(bufferList, buf);

	buf->type = SW_BUFFER_FULL;
	buf->refCount = 0;
	buf->blockNum = 0;

	buf->sampleCount = sizeBytes / sizeof(int16_t);
	buf->samples = swAlloc(buf->sampleCount * sizeof(float));

	/* convert samples from 16-bit int to IEEE float */
	for(i = 0; i < buf->sampleCount; i += channels) {
		for(k = 0; k < channels; k++) {
			int index = i + k;
			buf->samples[index] = (index < buf->sampleCount) ? ((int16_t*)ptr)[index] : 0;
		}
	}
	buf->frequency = frequency;
	buf->channelCount = channels;

	return buf;
}

/*=========================================*/
float * swBufferGetNextBlock(SWBuffer * buf) {
	if(buf->blockNum == -1) {
		/* decode first block */
		buf->decoder.Decode(&buf->decoder, SW_BUFFER_STREAM_HALF_SIZE);
		swBufferProcessDecodedData(buf, buf->sampleCount, buf->samples);
		/* decode second block */
		buf->decoder.Decode(&buf->decoder, SW_BUFFER_STREAM_HALF_SIZE);
		swBufferProcessDecodedData(buf, buf->sampleCount, buf->samples + buf->sampleCount);
		buf->blockNum = 0;
		return buf->samples;
	}
	if(buf->blockNum == 0) {
		/* decode first block */
		buf->decoder.Decode(&buf->decoder, SW_BUFFER_STREAM_HALF_SIZE);
		swBufferProcessDecodedData(buf, buf->sampleCount, buf->samples);
		buf->blockNum = 1;
		/* return pointer to second block */
		return buf->samples + buf->sampleCount; 
	}
	if(buf->blockNum == 1) {
		/* decode second block */
		buf->decoder.Decode(&buf->decoder, SW_BUFFER_STREAM_HALF_SIZE);
		swBufferProcessDecodedData(buf, buf->sampleCount, buf->samples + buf->sampleCount);
		buf->blockNum = 0;
		/* return pointer to first block */
		return buf->samples;
	}
	/* should never happen */
	return NULL;
}

/*=========================================*/
void swBufferRawFree(SWBuffer * buf) {
	swFree(buf->samples);
	swFree(buf);
}

/*=========================================*/
void swBufferFree(SWBuffer * buf) {
	if(swBufferIsValid(buf)) {
		--buf->refCount;
		if(buf->refCount <= 0) {
			LIST_ERASE(bufferList, buf);
			swBufferRawFree(buf);
		}
	}
}

/*=========================================*/
void swBufferFreeAll() {
	if(bufferList) {
		SWBuffer * buf = bufferList;
		while(buf) {
			SWBuffer * next = buf->next;
			swBufferRawFree(buf);
			buf = next;
		}
		bufferList = NULL;
	}
}

/*=========================================*/
float * swBufferGetSamples(SWBuffer * buf) {
	if(swBufferIsValid(buf)) {
		++buf->refCount;
		return buf->samples;
	}
	return NULL;
}

/*=========================================*/
uint32_t swBufferGetSampleCount(SWBuffer * buf) {
	if(swBufferIsValid(buf)) {
		return buf->sampleCount;
	}
	return 0;
}

/*=========================================*/
uint32_t swBufferGetChannelCount(SWBuffer * buf) {
	if(swBufferIsValid(buf)) {
		return buf->channelCount;
	}
	return 0;
}

/*=========================================*/
uint32_t swBufferGetFrequency(SWBuffer * buf) {
	if(swBufferIsValid(buf)) {
		return buf->frequency;
	}
	return 0;
}

/*=========================================*/
SWBufferType swBufferGetType(SWBuffer * buf) {
	return buf->type;
}
