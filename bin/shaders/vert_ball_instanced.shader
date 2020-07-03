#version 430

layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec3 vertCol;

layout(std430, binding = 1) buffer posBuffer
{
    vec2 pos[];
};

out vec3 color;

void main()
{
    uint id = gl_InstanceID;
    color = vertCol;
    gl_Position = vec4(vertPos + pos[id], 0.0, 1.0);
}