#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    flat int BlockType;
    vec4 FragPosLightSpace;
} fs_in;

struct Material {
    sampler2DArray diffuse; // 0
};

uniform Material material;
layout(binding = 1) uniform sampler2D shadowMap;

uniform vec3 lightDir;
uniform vec3 viewPos;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightDir - fs_in.FragPos);
    float bias = max(0.00025 * (1.0 - dot(normal, lightDir)), 0.00025);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 offset = vec2(x, y) + rand(projCoords.xy + vec2(x, y));
            float pcfDepth = texture(shadowMap, projCoords.xy + offset * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    vec3 NewTexCoord = vec3(fs_in.TexCoords.x, fs_in.TexCoords.y, fs_in.BlockType);
    vec4 color = texture(material.diffuse, NewTexCoord).rgba;

    if (color.a <= 0.65)
        discard;

    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    // vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float dotLightNormal = dot(-lightDir, normal);

    float diff = max(dotLightNormal, 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    // vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    // vec3 reflectDir = reflect(-lightDir, normal);
    // vec3 halfwayDir = normalize(lightDir + viewDir);
    // float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    // float specularStrength = 0.1;
    // vec3 specular = specularStrength * spec * lightColor;

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    if (dotLightNormal < 0.0)
        shadow = 1.0;
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse /* + specular*/)) * color.rgb;
    // vec3 lighting = (1.0 - shadow) * color;

    FragColor = vec4(lighting, 1.0);
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // FragColor = vec4(fs_in.FragPos / 50.0, 1.0);
}