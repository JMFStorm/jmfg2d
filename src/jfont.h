#pragma once

#include "types.h"

typedef struct CharData {
	float UV_x0;
	float UV_y0;
	float UV_x1;
	float UV_y1;
	int width;
	int height;
	int x_offset;
	int y_offset;
	int advance;
	char character;
} CharData;

typedef struct FontData {
	CharData char_data[96];
	int texture_id;
	float font_scale;
	int font_height_px;
} FontData;

void load_font(FontData* font_data, int font_height_px, const char* font_path);

void create_font_atlas_texture(FontData* font_data, s32 bitmap_width, s32 bitmap_height, byte* bitmap_memory);
