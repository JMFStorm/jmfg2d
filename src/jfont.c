#include "jfont.h"

#include <assert.h>
#include <stdlib.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define BITMAP_WIDTH  512
#define BITMAP_HEIGHT BITMAP_WIDTH
#define FIRST_CHAR 32
#define CHARS_SIZE 96 // ASCII 32..126 is 95 glyphs

stbtt_bakedchar cdata[CHARS_SIZE];

u32 my_stbtt_initfont(char* font_path)
{
	u32 texture_id;
	byte temp_bitmap[BITMAP_WIDTH * BITMAP_HEIGHT];
	const s64 sizes = 1<<20;
	byte* ttf_buffer = (byte*)malloc(sizes);

	read_file_to_ptr(font_path, ttf_buffer, sizes);
	stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, BITMAP_WIDTH, BITMAP_HEIGHT, FIRST_CHAR, CHARS_SIZE, cdata);

	free(ttf_buffer);

	texture_id = load_atlas_into_texture(temp_bitmap, BITMAP_WIDTH, BITMAP_HEIGHT);
	return texture_id;
}
