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

float normalize_value(float value, float src_max, float dest_max);

float normalize_screen_px_to_ndc(int value, int max);

s64 vw_to_screen_px(float vw);

s64 vh_to_screen_px(float vh);
