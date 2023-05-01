#version 450

in vec3 finalColor;
in vec2 TexCoord;
in vec3 scaledNormal;
in vec3 fragPos;

// Object material properties
uniform float ka;
uniform float kd;
uniform float ks;
uniform float q;

// Texture sampler
uniform sampler2D texSampler; // Added texture sampler

// Light source properties
uniform vec3 lightPos;
uniform vec3 lightColor;

// Camera position
uniform vec3 cameraPos;

// Output buffer (color buffer)
out vec4 color;

void main()
{
    // Sample the texture using the texture coordinates
    vec3 textureColor = texture(texSampler, TexCoord).rgb;

    // Ambient
    vec3 ambient =  lightColor * ka;

    // Diffuse 
    vec3 N = normalize(scaledNormal);
    vec3 L = normalize(lightPos - fragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightColor * kd;

    // Specular
    vec3 R = reflect(-L, N);
    vec3 V = normalize(cameraPos - fragPos);
    float spec = pow(max(dot(R, V), 0.0), q);
    vec3 specular = spec * ks * lightColor;

    // Combine texture color with the lighting calculation
    vec3 result = (ambient + diffuse) * textureColor + specular;

    color = vec4(result, 1.0f);
}
