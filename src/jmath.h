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
