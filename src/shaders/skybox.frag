#version 330 core

in vec3 direction;

out vec4 FragColor;

void main()
{
    vec3 dir = normalize(direction);
    float t = clamp(dir.y, 0.0, 1.0);
    vec3 horizonColor = vec3(0.65, 0.80, 1.0);
    vec3 topColor = vec3(0.25, 0.55, 0.95);
    vec3 skyColor = mix(horizonColor, topColor, t);
    FragColor = vec4(skyColor, 1.0);
}