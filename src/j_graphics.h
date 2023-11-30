#pragma once

#include "types.h"

typedef struct Shader {
    u32 id;
    u32 vao;
    u32 vbo;
} Shader;

bool check_shader_compile_error(u32 shader);
bool check_shader_link_error(u32 shader);
u32 compile_shader(char* vertex_shader_path, char* fragment_shader_path);

void init_rectangle_shader();

void init_shaders();
