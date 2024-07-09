#version 460 core

out vec4 FragColor;

in vec3 ourPosition;
in vec2 TexCoords;
in flat int BlockType;

struct Material {
    // sampler2D diffuse;
    sampler2DArray diffuse;
};

// uniform float textureUnit;

uniform Material material;

void main()
{
    vec3 NewTexCoord = vec3(TexCoords.x, TexCoords.y, BlockType);
    vec4 color = texture(material.diffuse, NewTexCoord);
    FragColor = color;
    // FragColor = vec4(TexCoords, 0.0, 1.0);
}