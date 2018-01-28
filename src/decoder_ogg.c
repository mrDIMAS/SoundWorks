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
#include "memory.h"

/*=========================================*/
SWBool swOggDecoderOpenFile(SWDecoder * decoder, const char * filename) {
	vorbis_info * vi;
	
	decoder->file = fopen(filename, "rb");
	
	if(!decoder->file) {
		swLogWrite("Unable to load %s file!", filename);
		return SW_FALSE;
	}
	
	if(ov_open_callbacks(decoder->file, &decoder->vorbisFile, 0, -1, OV_CALLBACKS_DEFAULT) != 0) {
		fclose(decoder->file);
		swLogWrite("Invalid ogg file %s !", filename);
		return SW_FALSE;
	}
	
	vi = ov_info(&decoder->vorbisFile, -1);
	
	/* at this moment, only mono and stereo buffers supported */
	if(vi->channels > 2) {
		fclose(decoder->file);
		ov_clear(&decoder->vorbisFile);
		swLogWrite("Unsupported ogg file. Current version of SoundWorks supports only mono and stereo buffers.");
		return SW_FALSE;
	}
	
	decoder->channelCount = vi->channels;
	decoder->frequency = vi->rate;
	decoder->fullSize = ov_pcm_total(&decoder->vorbisFile, -1) * 4;
	decoder->data = NULL;
	decoder->size = 0;
	decoder->eof = SW_FALSE;
	decoder->format = vi->channels == 2 ? SW_FORMAT_STEREO16 : SW_FORMAT_MONO16;
	
	return SW_TRUE;
}

/*=========================================*/
void swOggDecoderDecodeBlock(SWDecoder * decoder, int blockSize) {
	uint32_t total_bytes_decoded = 0;
	int32_t curSec = 0;
	if(!decoder->data) {
		decoder->data = swAlloc(blockSize);
	}
	while(total_bytes_decoded < blockSize) {
		int32_t bytes_decoded = ov_read(&decoder->vorbisFile, (char*)(decoder->data + total_bytes_decoded), blockSize - total_bytes_decoded, 0, 2, 1, &curSec);
		if(bytes_decoded <= 0) {
			decoder->eof = SW_TRUE;
			decoder->Rewind(decoder);
			break;
		}
		total_bytes_decoded += bytes_decoded;
	}
	decoder->size = total_bytes_decoded;
}

/*=========================================*/
void swOggDecoderClean(SWDecoder * decoder) {
	fclose(decoder->file);
	ov_clear(&decoder->vorbisFile);
	if(decoder->data) {
		swFree(decoder->data);
	}
}

/*=========================================*/
void swOggDecoderRewind(SWDecoder * decoder) {
	decoder->eof = SW_FALSE;
	ov_time_seek(&decoder->vorbisFile, 0.0f);
}
