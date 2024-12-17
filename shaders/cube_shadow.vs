#version 460 core

// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec2 aTexCoords;
// layout (location = 2) in vec3 aNormal;
// layout (location = 3) in float aBlockType;

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
    uint y = (aPackedData >> 24) & 0xFu;
    uint x = (aPackedData >> 28) & 0xFu;
    uint z = (aPackedData >> 20) & 0xFu;
    return vec3(float(x), float(y), float(z));
}

vec2 unpackTexCoords() {
    uint u = (aPackedData >> 16) & 0xFu;
    uint v = (aPackedData >> 12) & 0xFu;
    return vec2(float(u) / 15.0, float(v) / 15.0);
}

vec3 unpackNormal() {
    uint normalIdx = (aPackedData >> 9) & 0x7u;
    vec3 normals[6] = vec3[](
        vec3(0,0,-1), vec3(0,0,1), vec3(-1,0,0),
        vec3(1,0,0), vec3(0,1,0), vec3(0,-1,0)
    );
    return normals[normalIdx];
}

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = normal * aNormal;
    vs_out.BlockType = int(aBlockType);
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}