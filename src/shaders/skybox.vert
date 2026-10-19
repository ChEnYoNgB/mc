#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 direction;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 centeredPos = aPos - vec3(0.5);
    direction = centeredPos;
    vec4 pos = projection * view * vec4(centeredPos, 1.0);
    gl_Position = pos.xyww;
}