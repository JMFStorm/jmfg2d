#include "jmath.h"

#include "globals.h"
#include "jgame.h"

float normalize_to_ndc(s64 px, s64 screen_max)
{
	float x_ndc = px / (screen_max * 0.5f);
	float res = x_ndc - 1.0f;
	return res;
}

float normalize_value(float value, float src_max, float dest_max)
{
	// Assume 0.0 is min value
	float intermediate = value / src_max;
	float result = dest_max * intermediate;
	return result;
}

float vw_into_screen_px(float value, float screen_width_px)
{
	float res = screen_width_px * value * 0.01f;
	return res;
}

float vh_into_screen_px(float value, float screen_height_px)
{
	float res = screen_height_px * value * 0.01f;
	return res;
}

float normalize_screen_px_to_ndc(int value, int max)
{
	float this1 = (float)(value) / (float)(max);
	float this2 = 2.0f * this1;
	float res = -1.0f + this2;
	return res;
}
