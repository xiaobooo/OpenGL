#version 330 core
layout (location = 0) in vec2 aPos;

void main()
{
    gl_Position = gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0);
}
