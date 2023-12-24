#include "jmath.h"

#include "globals.h"
#include "jgame.h"

float normalize_value(float value, float src_max, float dest_max)
{
	// Assume 0.0 is min value
	float intermediate = value / src_max;
	float result = dest_max * intermediate;
	return result;
}

float normalize_screen_px_to_ndc(int value, int max)
{
	float this1 = (float)(value) / (float)(max);
	float this2 = 2.0f * this1;
	float res = -1.0f + this2;
	return res;
}

s64 vw_to_screen_px(float vw)
{
	float result = vw * (float)user_settings.window_width_px * 0.01f;
	return result;
}

s64 vh_to_screen_px(float vh)
{
	float result = vh * (float)user_settings.window_height_px * 0.01f;
	return result;
}
