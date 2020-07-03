#version 430

uniform vec4 boxRect[200];
uniform int boxHp[200];

layout(location = 0) in vec2 vertPos;

out vec3 color;

void main()
{
    uint id = gl_InstanceID;
    float hp = boxHp[id]/10000.0;
    vec4 rect = boxRect[id];
    vec2 vPos = (vertPos * rect.zw) + rect.xy;
    color = mix(vec3(0,0,1), vec3(0,1,0), hp);
    gl_Position = vec4(vPos,0.0,1.0);
}