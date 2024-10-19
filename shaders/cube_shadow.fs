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
    sampler2DArray diffuse;
};

uniform Material material;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 NewTexCoord = vec3(fs_in.TexCoords.x, fs_in.TexCoords.y, fs_in.BlockType);           
    vec3 color = texture(material.diffuse, NewTexCoord).rgb;

    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.3 * lightColor;    
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    // vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    // vec3 reflectDir = reflect(-lightDir, normal);
    // vec3 halfwayDir = normalize(lightDir + viewDir);  
    // float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    // float specularStrength = 0.1;
    // vec3 specular = specularStrength * spec * lightColor;
    
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse /* + specular*/)) * color; 
    
    FragColor = vec4(lighting, 1.0);
}