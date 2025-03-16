#version 460 core
layout (location = 0) in vec3 aPos;

uniform ivec2 resolution;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // ourPosition = aPos;
    gl_Position = vec4(aPos - 0.5, 0.5);
    // gl_Position = projection * view * model * vec4(aPos, 1.0);
}