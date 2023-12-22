#include "jmath.h"

#include "globals.h"
#include "jgame.h"

float normalize_to_ndc(s64 px, s64 screen_max)
{
	float x_ndc = px / (screen_max * 0.5f);
	float res = x_ndc - 1.0f;
	return res;
}