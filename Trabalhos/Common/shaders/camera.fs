#version 450

in vec3 finalColor;
in vec3 scaledNormal;
in vec3 fragPos;

uniform float ka;
uniform float kd;
uniform float ks;
uniform float q;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

uniform mat4 view;

out vec4 color;

void main()
{
    vec3 transformedLightPos = vec3(view * vec4(lightPos, 1.0));

    vec3 ambient = lightColor * ka;
    vec3 N = normalize(scaledNormal);
    vec3 L = normalize(transformedLightPos - fragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightColor * kd;

    vec3 R = reflect(-L, N);
    vec3 V = normalize(cameraPos - fragPos);
    float spec = pow(max(dot(R, V), 0.0), q);
    vec3 specular = spec * ks * lightColor;

    vec3 result = (ambient + diffuse) * finalColor + specular;
    color = vec4(result, 1.0f);
}
