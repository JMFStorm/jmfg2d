#include "jgame.h"

#include <stdio.h>
#include "constants.h"
#include "globals.h"

float get_debug_font_size()
{
    float font_size = (float)user_settings.window_height_px * DEBUG_FONT_SIZE_VH * 0.01f;
    return font_size;
}

void print_debug_info()
{
	char str[200] = {0};
    Point cursor = {
        vw_to_screen_px(1.0f),
        vh_to_screen_px(3.0f)
    };

    f64 time = system_data.elapsed_time_ms;

    sprintf(str, "Time elapsed: %.2fms Frames: %llu", time, system_data.frames_drawn);
    cursor = append_ui_text(debug_font_ptr, str, cursor);

    vec3 text_color = { 1.0f, 1.0f, 1.0f };
    draw_ui_text(debug_font_ptr, text_color);
}
