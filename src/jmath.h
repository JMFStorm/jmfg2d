#pragma once

#include "types.h"

typedef struct Point {
	s32 x;
	s32 y;
} Point;

typedef struct vec2 {
	f32 x;
	f32 y;
} vec2;

typedef struct vec3 {
	f32 x;
	f32 y;
	f32 z;
} vec3;

float normalize_to_ndc(s64 px, s64 screen_max);

float normalize_value(float value, float src_max, float dest_max);

float vw_into_screen_px(float value, float screen_width_px);

float vh_into_screen_px(float value, float screen_height_px);

float normalize_screen_px_to_ndc(int value, int max);
