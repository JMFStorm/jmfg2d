#pragma once

#include "constants.h"
#include "types.h"

#include "stb_truetype.h"

typedef struct CharData {
	stbtt_bakedchar cdata[CHARS_SIZE];
} CharData;

u32 init_font_atlas(char* font_path, s32 font_height, CharData* char_data);

void get_char_from_atlas(char character, float* top_left, float* bot_right, CharData* char_data);
