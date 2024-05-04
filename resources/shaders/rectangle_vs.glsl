#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec2 a_offset;
layout(location = 3) in vec3 a_color;

out vec2 uv;
out vec3 color;

void main()
{
   gl_Position = vec4(a_position.x + a_offset.x, a_position.y + a_offset.y, a_position.z, 1.0);
   uv = a_uv;
   color = a_color;
}
