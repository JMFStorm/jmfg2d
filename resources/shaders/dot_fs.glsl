#version 330 core

in vec3 color;

out vec4 frag_out;

void main()
{
    frag_out = vec4(color, 1.0);
}
