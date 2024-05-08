#include "jgame.h"

#include <stdio.h>
#include "constants.h"
#include "globals.h"

float get_debug_font_size() {
        float font_size = (float)user_settings.window_height_px * DEBUG_FONT_SIZE_VH * 0.01f;
        return font_size;
}

void print_debug_info() {
        Point cursor = {
                vw_to_screen_px(1.0f),
                vh_to_screen_px(3.0f)
        };

        f64 time = system_data.elapsed_time_ms;

        char str[100] = { 0 };
        sprintf_s(str, 100, "Time elapsed: %.2fms", time);
        append_ui_text(debug_font_ptr, str, cursor);

        cursor.y += debug_font_ptr->font_height_px + (debug_font_ptr->font_height_px * 0.15f);

        char str2[100] = { 0 };
        sprintf_s(str2, 100, "Frames: %llu", system_data.frames_drawn);
        append_ui_text(debug_font_ptr, str2, cursor);

        vec3 text_color = { 1.0f, 1.0f, 1.0f };
        draw_ui_text(debug_font_ptr, text_color);
}
