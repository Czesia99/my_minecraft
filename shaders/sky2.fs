#version 460 core

out vec4 FragColor;

in vec3 FragPos;

uniform mat4 view;

vec3 SkyColour( vec3 ray )
{
    return exp2(-ray.y/vec3(.1,.3,.6)); // blue
//    return exp2(-ray.y/vec3(.18,.2,.28))*vec3(1,.95,.8); // overcast
//    return exp2(-ray.y/vec3(.1,.2,.8))*vec3(1,.75,.5); // dusk
//    return exp2(-ray.y/vec3(.03,.2,.9)); // tropical blue
//    return exp2(-ray.y/vec3(.4,.06,.01)); // orange-red
//    return exp2(-ray.y/vec3(.1,.2,.01)); // green
}

void main()
{
    vec3 ray = normalize(FragPos);

    vec3 tint = vec3(1);
    if ( ray.y < .0 )
    {
        ray.y = -ray.y;
        tint = mix(vec3(.2), tint, pow(1.-ray.y,10.));
    }

    FragColor.rgb = SkyColour(ray) * tint;
}