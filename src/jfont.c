#include "jfont.h"

#include <assert.h>
#include <stdlib.h>

#include "constants.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

stbtt_bakedchar cdata[CHARS_SIZE]; // ASCII 32..126 is 95 glyphs

u32 init_font_atlas(char* font_path, s32 font_height)
{
	u32 texture_id;
	f32 char_height = (float)font_height;
	byte* temp_bitmap = (byte*)malloc(BITMAP_WIDTH * BITMAP_HEIGHT);
	const s64 sizes = MEGABYTES(1);
	byte* ttf_buffer = (byte*)malloc(sizes);

	read_file_to_ptr(font_path, ttf_buffer, sizes);
	stbtt_BakeFontBitmap(ttf_buffer, 0, char_height, temp_bitmap, BITMAP_WIDTH, BITMAP_HEIGHT, FIRST_CHAR, CHARS_SIZE, cdata);
	texture_id = load_atlas_into_texture(temp_bitmap, BITMAP_WIDTH, BITMAP_HEIGHT);

	free(ttf_buffer);
	free(temp_bitmap);
	return texture_id;
}

void get_char_from_atlas(char character, float* top_left, float* bot_right)
{
	int char_index = character - FIRST_CHAR;
    float x, y;
    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(cdata, BITMAP_WIDTH, BITMAP_HEIGHT, char_index, &x,&y,&q, 1);

    top_left[0] = q.x0;
    top_left[1] = q.y0;
    top_left[2] = q.s0;
    top_left[3] = q.t0;

    bot_right[0] = q.x1;
    bot_right[1] = q.y1;
    bot_right[2] = q.s1;
    bot_right[3] = q.t1;
}