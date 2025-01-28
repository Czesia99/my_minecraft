#version 460 core

in vec2 TexCoords;
in flat int BlockType;

struct Material {
    sampler2DArray diffuse; // 0
};

uniform Material material;

void main()
{
    vec3 NewTexCoord = vec3(TexCoords.x, TexCoords.y, BlockType);
    vec4 color = texture(material.diffuse, NewTexCoord).rgba;
    if (color.a <= 0.65)
        discard;
    // gl_FragDepth = 1.0;// gl_FragCoord.z;
}