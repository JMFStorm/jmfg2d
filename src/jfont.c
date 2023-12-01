#include "jfont.h"

#include <assert.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

const s32 first_char = 32;
const s32 chars_size = 96; // ASCII 32..126 is 95 glyphs

stbtt_bakedchar cdata[chars_size];

u32 my_stbtt_initfont(char* font_path)
{
	u32 texture_id;
	const s64 bitmap_length = 512;
	byte temp_bitmap[bitmap_length * bitmap_length];
	unsigned char ttf_buffer[1<<20] = {0};

	FILE* file = fopen(font_path, "rb");
	assert(file);
	fread(ttf_buffer, 1, 1<<20, file);
	stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, bitmap_length, bitmap_length, first_char, chars_size, cdata);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return texture_id;
}