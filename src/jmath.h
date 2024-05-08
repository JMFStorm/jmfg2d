#pragma once

#include "types.h"

#define J_PI 3.14159265358979323846
#define DEG_TO_RAD(deg) ((deg) * J_PI / 180.0f)

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

typedef f32 mat4x4[16];

f32 j_cos(f32 x);
f32 j_sin(f32 x);
f32 j_sinf(f32 x);
f32 j_sqrtf(f32 x);

void mat4x4_identity(mat4x4 mat4);
void mat4x4_scale(mat4x4 mat4, vec3 vec3);
void mat4x4_translate(mat4x4 mat4, vec3 vec3);
void mat4x4_rotate(mat4x4 mat, vec3 axis, f32 angle_deg);

float normalize_value(float value, float src_max, float dest_max);
float normalize_screen_px_to_ndc(int value, int max);

s64 vw_to_screen_px(float vw);
s64 vh_to_screen_px(float vh);
