#version 460 core

layout (location = 0) in uint aPackedData;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    flat int BlockType;
    vec4 FragPosLightSpace;
} vs_out;

uniform ivec3 chunkpos;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

vec3 unpackPosition()
{
    uint x = (aPackedData >> 0) & 0x1F;
    uint y = (aPackedData >> 5) & 0x1F;
    uint z = (aPackedData >> 10) & 0x1F;

    return vec3(float(x), float(y), float(z));
}

vec2 unpackTexCoords()
{
    uint x = (aPackedData >> 15) & 1;
    uint y = (aPackedData >> 16) & 1;

    return vec2(float(x), float(y));
}

vec3 unpackNormal()
{
    vec3 normals[6] = {
        vec3(0.0, 0.0, -1.0),
        vec3(0.0, 0.0, 1.0),
        vec3(-1.0, 0.0, 0.0),
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, -1.0, 0.0),
        vec3(0.0, 1.0, 0.0)
    };

    uint normalIndex = (aPackedData >> 17) & 7;

    return normals[normalIndex];
}

uint unpackBlockType()
{
    uint bt = (aPackedData >> 20) & 31;
    return bt;
}

void main()
{
    vec3 aPos = unpackPosition();
    vec2 aTexCoords = unpackTexCoords();
    vec3 aNormal = unpackNormal();
    uint aBlockType = unpackBlockType();


    vec3 worldpos = aPos + chunkpos;
    vs_out.FragPos = worldpos;
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = aNormal;
    vs_out.BlockType = int(aBlockType);
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(worldpos, 1.0);

    gl_Position = projection * view * vec4(worldpos, 1.0);
    // gl_Position = vec4(aPos, 1.0);
}