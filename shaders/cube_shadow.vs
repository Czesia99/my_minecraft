#version 460 core

layout (location = 0) in uint aPackedData;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    flat int BlockType;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;
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
    uint x = (aPackedData >> 11) & 1;
    uint y = (aPackedData >> 12) & 1;

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

    uint normalIndex = (aPackedData >> 13) & 8;

    return normals[normalIndex];
}

uint unpackBlockType()
{
    uint bt = (aPackedData >> 16) & 31;
    return bt;
}

void main()
{
    vec3 aPos = unpackPosition();
    vec2 aTexCoords = unpackTexCoords();
    vec3 aNormal = unpackNormal();
    uint aBlockType = unpackBlockType();


    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = normal * aNormal;
    vs_out.BlockType = int(aBlockType);
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // gl_Position = vec4(aPos, 1.0);
}