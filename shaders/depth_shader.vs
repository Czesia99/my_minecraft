#version 460 core

layout (location = 0) in uint aPackedData;

uniform mat4 lightSpaceMatrix;
uniform ivec3 chunkpos;

out vec2 TexCoords;
out flat int BlockType;

vec3 unpackPosition()
{
    int x = int((aPackedData >> 0) & 31u);
    int y = int((aPackedData >> 5) & 31u);
    int z = int((aPackedData >> 10) & 31u);

    return vec3(float(x), float(y), float(z));
}

vec2 unpackTexCoords()
{
    int x = int((aPackedData >> 15) & 1u);
    int y = int((aPackedData >> 16) & 1u);

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

int unpackBlockType()
{
    int bt = int((aPackedData >> 20) & 31u);
    return bt;
}

void main()
{
    vec3 aPos = unpackPosition();
    vec2 aTexCoords = unpackTexCoords();
    vec3 aNormal = unpackNormal();
    int aBlockType = unpackBlockType();

    TexCoords = aTexCoords;
    BlockType = aBlockType;
    gl_Position = lightSpaceMatrix * vec4(aPos + chunkpos, 1.0);
}