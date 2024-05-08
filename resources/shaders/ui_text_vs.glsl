#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

uniform mat4 transform;

out vec2 uv;

void main()
{
   gl_Position = transform * vec4(a_position, 1.0);
   uv = a_uv;
}
