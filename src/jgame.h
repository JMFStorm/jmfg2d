#pragma once

#include "jmath.h"

typedef struct UserSettings {
    int window_width_px;
    int window_height_px;
} UserSettings;

typedef struct FrameData {
    s64 draw_calls;
} FrameData;

float get_debug_font_size();
