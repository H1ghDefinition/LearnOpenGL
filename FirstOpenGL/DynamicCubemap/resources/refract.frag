#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform samplerCube skybox;

uniform float ior;
uniform bool fresnelAccurate;

float Fresnel_SchilcksApproximation(vec3 I, vec3 N)
{
    float cosTheta = clamp(-1, 1, dot(-I, N));
    float R0 = ((1.0 - ior) * (1.0 - ior)) / ((1.0 + ior) * (1.0 + ior));
    float fresnel = R0 + (1 - R0) * pow((1 - cosTheta), 5);
    return fresnel;
}

float Fresnel(vec3 I, vec3 N)
{
    float cosi = clamp(-1, 1, dot(-I, N));
    // if(cosTheta < 0)
    float sint = 1.0 / ior * sqrt(max(0, 1 - cosi * cosi));
    
    if(sint >= 1) return 1.0f;

    float cost = sqrt(max(0, 1 - sint * sint));
    cosi = abs(cosi);

    float Rs = ((ior * cosi) - (1.0 * cost)) / ((ior * cosi) + (1.0 * cost)); 
    float Rp = ((1.0 * cosi) - (ior * cost)) / ((1.0 * cosi) + (ior * cost)); 
    float fresnel = (Rs * Rs + Rp * Rp) / 2;
    return fresnel; 
}

void main()
{
    vec3 I = normalize(FragPos - viewPos);
    vec3 N = normalize(Normal);

    vec3 reflectColor = texture(skybox, reflect(I, N)).rgb;
    vec3 refractColor = texture(skybox, normalize(refract(I, N, ior))).rgb;

    float fresnel = 1.0f;
    if(fresnelAccurate)
    {
        fresnel = Fresnel(I, N);    
    }
    else 
    {
        fresnel = Fresnel_SchilcksApproximation(I, N);
    }

//    FragColor = vec4(mix(reflectColor, refractColor, fresnel), 1.0);
    FragColor = vec4(mix(reflectColor, refractColor, dot(-I, N)), 1.0);
}