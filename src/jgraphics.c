#include "jgraphics.h"

#include <assert.h>
#include <glad/glad.h>

#include "constants.h"
#include "files.h"
#include "globals.h"
#include "jimage.h"
#include "jfont.h"
#include "main.h"

void check_shader_compile_error(u32 shader)
{
    s32 shader_compile_success;
    char info_log[INFO_LOG_LENGTH] = {0};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compile_success);
    if (!shader_compile_success) debug_log(info_log);
    assert(shader_compile_success);
}

void check_shader_link_error(u32 shader)
{
    s32 shader_compile_success;
    char info_log[INFO_LOG_LENGTH] = {0};
    glGetProgramiv(shader, GL_LINK_STATUS, &shader_compile_success);
    if (!shader_compile_success) debug_log(info_log);
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

    read_file_to_ptr(vertex_shader_path, shader_src, SHADER_SOURCE_LENGTH);
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &shader_src, NULL);
    glCompileShader(vertex_shader);
    check_shader_compile_error(vertex_shader);

    read_file_to_ptr(fragment_shader_path, shader_src, SHADER_SOURCE_LENGTH);
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
    const char* vertex_shader_path   = "G:/projects/game/jmfg2d/resources/shaders/rectangle_vs.glsl";
    const char* fragment_shader_path = "G:/projects/game/jmfg2d/resources/shaders/rectangle_fs.glsl";

    BaseShader base_shader = {0};
    base_shader.id = compile_shader((char*)vertex_shader_path, (char*)fragment_shader_path);

    glGenVertexArrays(1, &base_shader.vao);
    glGenBuffers(1, &base_shader.vbo);

    glBindVertexArray(base_shader.vao);
    glBindBuffer(GL_ARRAY_BUFFER, base_shader.vbo);

    float vertices[] =
    {
        // Coords              // UV
        -0.25f, -0.25f, 0.0f,  0.0f, 0.0f, // bottom left
         0.25f, -0.25f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.25f,  0.25f, 0.0f,  0.0f, 1.0f, // top left
 
        -0.25f,  0.25f, 0.0f,  0.0f, 1.0f, // top left
         0.25f, -0.25f, 0.0f,  1.0f, 0.0f, // bottom right
         0.25f,  0.25f, 0.0f,  1.0f, 1.0f  // top right
    };

    rect_shader.base_shader = base_shader;

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Coord attribute 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV attribute 1
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Offsets instanced attribute 2
    s64 offsets_size = sizeof(float) * MAX_BUFFERED_RECTS;
    glGenBuffers(1, &rect_shader.offset_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, rect_shader.offset_vbo);
    glBufferData(GL_ARRAY_BUFFER, offsets_size, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void init_text_shader()
{
    const char* vertex_shader_path   = "G:/projects/game/jmfg2d/resources/shaders/ui_text_vs.glsl";
    const char* fragment_shader_path = "G:/projects/game/jmfg2d/resources/shaders/ui_text_fs.glsl";

    ui_text_shader.id = compile_shader((char*)vertex_shader_path, (char*)fragment_shader_path);

    glGenVertexArrays(1, &ui_text_shader.vao);
    glGenBuffers(1, &ui_text_shader.vbo);

    glBindVertexArray(ui_text_shader.vao);
    glBindBuffer(GL_ARRAY_BUFFER, ui_text_shader.vbo);

    s64 vbo_size = (sizeof(float) * 5 * 6) * MAX_BUFFERED_TEXT;
    glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, GL_DYNAMIC_DRAW);

    // Coord attribute 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV attribute 1
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void init_shaders()
{
	init_rectangle_shader();
    init_text_shader();
}

void set_draw_area(s32 start_x, s32 start_y, s32 end_x, s32 end_y)
{
    glViewport(start_x, start_y, end_x, end_y);
}

u32 load_image_to_texture(char* image_path)
{
    u32 texture_id = 0;
    ImageData image = load_image_data(image_path);
    assert(image.channels == 3 || image.channels == 4);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    bool use_linear_texture_filtering = true;
    bool generate_texture_mipmaps = true;
    bool load_texture_sRGB = false;

    GLint filtering_mode = use_linear_texture_filtering
        ? GL_LINEAR
        : GL_NEAREST;

    GLint mipmap_filtering_mode = use_linear_texture_filtering
        ? GL_LINEAR_MIPMAP_LINEAR
        : GL_NEAREST_MIPMAP_NEAREST;

    if (generate_texture_mipmaps) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_filtering_mode);
    else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering_mode);

    GLint use_format = image.channels == 3 ? GL_RGB : GL_RGBA;

    GLint internal_format;
    if (load_texture_sRGB) internal_format = image.channels == 3 ? GL_SRGB : GL_SRGB_ALPHA;
    else internal_format = image.channels == 3 ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image.size_px.x, image.size_px.y, 0, use_format, GL_UNSIGNED_BYTE, image.image_data);

    if (generate_texture_mipmaps) glGenerateMipmap(GL_TEXTURE_2D);

    free_loaded_image(image);
    return texture_id;
}

u32 load_atlas_into_texture(byte* atlas_bitmap, s32 width, s32 height)
{
    u32 texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, atlas_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    return texture_id;
}

void append_rect(vec2 offset)
{
    s64 bytes_offset = rects_buffered * sizeof(vec2);
    glBindBuffer(GL_ARRAY_BUFFER, rect_shader.offset_vbo);
    float as_floats[2] = {offset.x, offset.y };
    glBufferSubData(GL_ARRAY_BUFFER, bytes_offset, sizeof(as_floats), as_floats);
    rects_buffered++;
}

void draw_rects(u32 texture_id)
{
    glUseProgram(rect_shader.base_shader.id);
    glBindVertexArray(rect_shader.base_shader.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, rects_buffered);

    rects_buffered = 0;
    glUseProgram(0);
    glBindVertexArray(0);
}

Point append_ui_text(FontData* font_data, char* text, Point start_px)
{
    CharData* chars = font_data->char_data;
    char* text_string = text;
    int length = strlen(text);

    int text_offset_x_px = 0;
    int text_offset_y_px = 0;

    // Assume font cursor start position is screen top left corner

    Point next_cursor = start_px;

    // Assume font UV start position is top left corner

    glBindBuffer(GL_ARRAY_BUFFER, ui_text_shader.vbo);

    for (int i = 0; i < length; i++)
    {
        char current_char = *text_string++;

        if (current_char == '\n')
        {
            text_offset_x_px = 0;
            text_offset_y_px += font_data->font_height_px;

            next_cursor.x = start_px.x;
            next_cursor.y = start_px.y + text_offset_y_px;

            continue;
        }

        int char_index = (int)(current_char) - 32;
        CharData current = chars[char_index];

        int x_offsets = current.x_offset + text_offset_x_px;
        int new_x = start_px.x + x_offsets;

        int y_offsets = text_offset_y_px + current.y_offset;
        int new_y = user_settings.window_height_px - (start_px.y - y_offsets);

        float x0 = normalize_screen_px_to_ndc(new_x, user_settings.window_width_px);
        float y0 = normalize_screen_px_to_ndc(new_y, user_settings.window_height_px);

        float x1 = normalize_screen_px_to_ndc(new_x + current.width, user_settings.window_width_px);
        float y1 = normalize_screen_px_to_ndc(new_y + current.height, user_settings.window_height_px);

        float vertices[] =
        {
            // Coords           // UV
            x0, y1, 0.0f,       current.UV_x0, current.UV_y1, // top left
            x0, y0, 0.0f,       current.UV_x0, current.UV_y0, // bottom left
            x1, y0, 0.0f,       current.UV_x1, current.UV_y0, // bottom right

            x1, y1, 0.0f,       current.UV_x1, current.UV_y1, // top right
            x0, y1, 0.0f,       current.UV_x0, current.UV_y1, // top left 
            x1, y0, 0.0f,       current.UV_x1, current.UV_y0  // bottom right
        };

        s64 bytes_offset = ui_chars_buffered * sizeof(vertices);
        glBufferSubData(GL_ARRAY_BUFFER, bytes_offset, sizeof(vertices), vertices);

        ui_chars_buffered++;
        next_cursor.x += current.advance;
        text_offset_x_px += current.advance;
        text++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return next_cursor;
}

void draw_ui_text(FontData* font_data_ptr, vec3 color)
{
    glUseProgram(ui_text_shader.id);
    glBindVertexArray(ui_text_shader.vao);

    int color_uniform = glGetUniformLocation(ui_text_shader.id, "textColor");
    glUniform3f(color_uniform, color.x, color.y, color.z);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, font_data_ptr->texture_id);

    s64 indicies = ui_chars_buffered * 6;
    glDrawArrays(GL_TRIANGLES, 0, indicies);

    ui_chars_buffered = 0;
    frame_data.draw_calls++;

    glUseProgram(0);
    glBindVertexArray(0);
}

void create_font_atlas_texture(FontData* font_data_ptr, s32 bitmap_width, s32 bitmap_height, byte* bitmap_memory)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint new_texture;
    GLuint prev_texture = font_data_ptr->texture_id;
    glDeleteTextures(1, &prev_texture);
    glGenTextures(1, &new_texture);
    font_data_ptr->texture_id = (int)(new_texture);
    glBindTexture(GL_TEXTURE_2D, font_data_ptr->texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, bitmap_width, bitmap_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap_memory);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}
