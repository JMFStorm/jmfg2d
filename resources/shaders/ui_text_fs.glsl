#version 330 core

in vec2 uv;

uniform vec3 textColor;
uniform sampler2D texture1;

out vec4 frag_out;

void main()
{
    frag_out = vec4(textColor.r, textColor.g, textColor.b, texture(texture1, uv).a);
}
