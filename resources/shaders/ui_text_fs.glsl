#version 330 core

in vec2 uv;

uniform sampler2D texture1;

out vec4 frag_out;

void main()
{
    frag_out = texture(texture1, uv);
}
