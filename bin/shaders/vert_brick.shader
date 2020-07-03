#version 430

uniform vec4 boxRect;
uniform int boxHp;

layout(location = 0) in vec2 vertPos;

out vec3 color;

void main()
{
    float hp = boxHp/10000.0;
    vec2 vPos = (vertPos * boxRect.zw) + boxRect.xy;
    color = mix(vec3(0,0,1), vec3(0,1,0), hp);
    gl_Position = vec4(vPos,0.0,1.0);
}