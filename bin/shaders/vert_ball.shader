#version 430

layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec3 vertCol;

uniform vec2 pos;

out vec3 color;

void main()
{
    color = vertCol;
    gl_Position = vec4(vertPos + pos, 0.0, 1.0);
}