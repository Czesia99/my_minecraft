#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 ourPosition;
in vec2 TexCoords;
in vec3 Normal;

struct Material {
    sampler2DArray diffuse;
};

uniform Material material;

uniform ivec3 BlockTextures;

void main()
{
    vec3 NewTexCoord;
    if ((Normal.y) > 0.5f)
    {
        NewTexCoord = vec3(TexCoords.x, TexCoords.y, BlockTextures.x);
    }
    if (abs(Normal.z) > 0.5 || abs(Normal.x) > 0.5)
    {
        NewTexCoord = vec3(TexCoords.x, TexCoords.y, BlockTextures.y);
    }
    if ((Normal.y) < -0.5f)
    {
        NewTexCoord = vec3(TexCoords.x, TexCoords.y, BlockTextures.z);
    }

    vec4 color = texture(material.diffuse, NewTexCoord);
    FragColor = color;
    // FragColor = vec4((mod(FragPos, 16.0) / 16.0).xz, 0.0, 1.0);
    // if (BlockType == 0) FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red for block type 0
    // else if (BlockType == 1) FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green for block type 1
    // else if (BlockType == 2) FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue for block type 2
}