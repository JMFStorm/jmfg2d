#include "jgraphics.h"

#include <assert.h>
#include <glad/glad.h>

#include "constants.h"
#include "globals.h"
#include "files.h"

void check_shader_compile_error(u32 shader)
{
    s32 shader_compile_success;
    char info_log[INFO_LOG_LENGTH] = {0};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compile_success);
    assert(shader_compile_success);
}

void check_shader_link_error(u32 shader)
{
    s32 shader_compile_success;
    char info_log[INFO_LOG_LENGTH] = {0};
    glGetProgramiv(shader, GL_LINK_STATUS, &shader_compile_success);
    assert(shader_compile_success);
}

u32 compile_shader(char* vertex_shader_path, char* fragment_shader_path)
{
    u32 shader_id;
    u32 vertex_shader;
    u32 fragment_shader;

    bool success;
    char shader_memory[SHADER_SOURCE_LENGTH] = {0};
    char* shader_src = &shader_memory[0];

    read_file_to_string(vertex_shader_path, shader_src, SHADER_SOURCE_LENGTH);
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &shader_src, NULL);
    glCompileShader(vertex_shader);
    check_shader_compile_error(vertex_shader);

    read_file_to_string(fragment_shader_path, shader_src, SHADER_SOURCE_LENGTH);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &shader_src, NULL);
    glCompileShader(fragment_shader);
    check_shader_compile_error(fragment_shader);

    shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex_shader);
    glAttachShader(shader_id, fragment_shader);
    glLinkProgram(shader_id);
    check_shader_link_error(shader_id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_id;
}

void init_rectangle_shader()
{
    const char* vertex_shader_path   = "G:/projects/game/JMF_Engine/resources/shaders/rectangle_vs.glsl";
    const char* fragment_shader_path = "G:/projects/game/JMF_Engine/resources/shaders/rectangle_fs.glsl";

    simple_rect_shader.id = compile_shader((char*)vertex_shader_path, (char*)fragment_shader_path);

    glGenVertexArrays(1, &simple_rect_shader.vao);
    glGenBuffers(1, &simple_rect_shader.vbo);

    glBindVertexArray(simple_rect_shader.vao);
    glBindBuffer(GL_ARRAY_BUFFER, simple_rect_shader.vbo);

    float vertices[] =
    {
        // Coords            
        -0.25f, -0.25f, 0.0f, // bottom left
         0.25f, -0.25f, 0.0f, // bottom right
        -0.25f,  0.25f, 0.0f, // top left

        -0.25f,  0.25f, 0.0f, // top left
         0.25f, -0.25f, 0.0f, // bottom right
         0.25f,  0.25f, 0.0f  // top right
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Coord attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void init_shaders()
{
	init_rectangle_shader();
}

void set_draw_area(s32 start_x, s32 start_y, s32 end_x, s32 end_y)
{
    glViewport(start_x, start_y, end_x, end_y);
}
