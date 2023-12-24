#include "globals.h"

RectangleShader rect_shader = {0};
BaseShader ui_text_shader = {0};

s64 rects_buffered = 0;
s64 chars_buffered = 0;

CharData debug_font_data = {0};

UserSettings user_settings = {0};

s64 ui_chars_buffered = 0;

FrameData frame_data = {0};

SystemData system_data = {0};

FontData* debug_font_ptr = NULL;
