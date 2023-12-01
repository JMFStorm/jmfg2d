#pragma once

#include "types.h"

typedef struct Size {
	s32 width;
	s32 height;
} Size;

typedef struct vec2 {
	f32 x;
	f32 y;
	f32 z;
} vec2;

typedef struct vec3 {
	f32 x;
	f32 y;
	f32 z;
} vec3;

vec2 vec2_init();

vec3 vec3_init();
