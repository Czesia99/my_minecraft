#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 ourPosition;
in vec2 TexCoords;
in flat int BlockType;

struct Material {
    sampler2DArray diffuse;
};

uniform Material material;

void main()
{
    vec3 NewTexCoord = vec3(TexCoords.x, TexCoords.y, BlockType);
    vec4 color = texture(material.diffuse, NewTexCoord);
    FragColor = color;
    // FragColor = vec4((mod(FragPos, 16.0) / 16.0).xz, 0.0, 1.0);
    // if (BlockType == 0) FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red for block type 0
    // else if (BlockType == 1) FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green for block type 1
    // else if (BlockType == 2) FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue for block type 2
}