#pragma once

#include "jmath.h"

typedef struct UserSettings {
    int window_width_px;
    int window_height_px;
} UserSettings;

typedef struct FrameData {
    s64 draw_calls;
} FrameData;

typedef struct SystemData {
    u64 frames_drawn;
} SystemData;

float get_debug_font_size();

void print_debug_info();
