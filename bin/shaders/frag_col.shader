#version 430

in vec3 color;
out vec4 fragCol;

void main()
{
    fragCol = vec4(color, 1.0);
}