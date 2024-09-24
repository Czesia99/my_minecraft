#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in float aBlockType;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out flat int BlockType;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;

void main()
{
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normal * aNormal;
    BlockType = int(aBlockType);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}