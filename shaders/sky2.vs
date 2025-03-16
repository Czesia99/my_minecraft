#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // ourPosition = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    FragPos = aPos;
    // gl_Position = vec4(aPos - 0.5, 0.5);
    gl_Position = pos.xyww;
}