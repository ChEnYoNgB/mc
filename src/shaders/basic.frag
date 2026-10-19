#version 330 core
in vec3 vPosition;
in vec2 TexCoord;
in float Brightness;
in vec3 WorldPosition;
uniform sampler2D ourTexture;
uniform vec3 cameraPosition;
uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;
out vec4 FragColor;



void main()
{
    // FragColor = vec4(vPosition * 0.5 + 0.5, 1.0);
    vec4 texColor = texture(ourTexture, TexCoord) * Brightness;
    float distanceToCamera = length(WorldPosition - cameraPosition);
    float fogFactor = smoothstep(fogStart, fogEnd, distanceToCamera);
    vec3 finalColor = mix(texColor.rgb, fogColor, fogFactor);
    FragColor =vec4(finalColor, texColor.a);
}