#version 330 core

in vec2 uv;

uniform sampler2D texture1;

out vec4 frag_out;

void main()
{
    frag_out = texture(texture1, uv);
    // frag_out = vec4(1.0, 0.5, 0.5, 1.0);
}
