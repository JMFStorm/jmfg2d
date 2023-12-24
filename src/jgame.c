#include "jgame.h"

#include "constants.h"
#include "globals.h"

float get_debug_font_size()
{
    float font_size = (float)user_settings.window_height_px * DEBUG_FONT_SIZE_VH * 0.01f;
    return font_size;
}
