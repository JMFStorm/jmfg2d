#version 330 core

in vec2 uv;
in vec3 color;

uniform sampler2D texture1;

out vec4 frag_out;

void main()
{
    frag_out = texture(texture1, uv) * vec4(color, 1.0);
}
