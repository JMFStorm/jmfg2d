#pragma once

#include "types.h"

u32 init_font_atlas(char* font_path, s32 font_height);

void get_char_from_atlas(char character, float* top_left, float* bot_right);
