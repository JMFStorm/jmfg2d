#pragma once

#include "jmath.h"

typedef struct UserSettings {
    int window_width_px;
    int window_height_px;
} UserSettings;

typedef struct FrameData {
    s64 draw_calls;
    f64 deltatime_ms;
    vec2 mouse_pos;
} FrameData;

typedef struct SystemData {
    u64 frames_drawn;
    f64 elapsed_time_ms;

    // Windows specific
    u64 perf_counter_freq;
    u64 perf_counter;
    u64 prev_perf_counter;
} SystemData;

float get_debug_font_size();

void print_debug_info();
