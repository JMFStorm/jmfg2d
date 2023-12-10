#pragma once

#include "types.h"
#include "jmath.h"
#include "jfont.h"

typedef struct BaseShader {
    u32 id;
    u32 vao;
    u32 vbo;
} BaseShader;

typedef struct RectangleShader {
    BaseShader base_shader;
    u32 offset_vbo;
} RectangleShader;

void check_shader_compile_error(u32 shader);
void check_shader_link_error(u32 shader);
u32 compile_shader(char* vertex_shader_path, char* fragment_shader_path);

void init_rectangle_shader();
void init_shaders();

void set_draw_area(s32 start_x, s32 start_y, s32 end_x, s32 end_y);

u32 load_atlas_into_texture(byte* atlas_bitmap, s32 width, s32 height);

void append_rect(vec2 offset);
void draw_rects(u32 test_texture_id);

vec2 append_text(char* text, CharData* char_data, vec2 screen_pos);
void append_char(char character, CharData* char_data);
void draw_chars(u32 atlas_texture_id);
