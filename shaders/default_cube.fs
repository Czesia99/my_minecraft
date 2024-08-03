#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 ourPosition;
in vec2 TexCoords;
in flat int BlockType;

struct Material {
    // sampler2D diffuse;
    sampler2DArray diffuse;
};

uniform Material material;

void main()
{
    // vec4 color = texture(material.diffuse, TexCoords);
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}