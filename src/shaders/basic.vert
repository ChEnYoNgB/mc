#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTexIndex;
layout (location = 3) in float brightness;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float Brightness;
out vec3 vPosition;
out vec2 TexCoord;
out vec3 WorldPosition;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vPosition = aPos;
    if(aTexIndex == 1){
        TexCoord = vec2(
        1.0 - ((aTexCoord.x + float(aTexIndex)) / 3.0),
        1.0 - aTexCoord.y
    );
    }
    else{
        TexCoord = vec2(
            (aTexCoord.x + float(aTexIndex)) / 3.0,
            aTexCoord.y
        );
    } 
    Brightness = brightness;
    vec4 worldPos = model * vec4(aPos, 1.0);
    WorldPosition = worldPos.xyz;
}