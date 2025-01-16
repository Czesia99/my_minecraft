#version 460 core

layout (location = 0) in uint aPackedData;

uniform mat4 lightSpaceMatrix;
uniform ivec3 chunkpos;

vec3 unpackPosition()
{
    uint x = (aPackedData >> 0) & 0x1F;
    uint y = (aPackedData >> 5) & 0x1F;
    uint z = (aPackedData >> 10) & 0x1F;

    return vec3(float(x), float(y), float(z));
}

void main()
{
    vec3 aPos = unpackPosition();
    gl_Position = lightSpaceMatrix * vec4(aPos + chunkpos, 1.0);
}