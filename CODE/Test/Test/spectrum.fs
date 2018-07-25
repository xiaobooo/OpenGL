#version 330 core
out vec4 FragColor;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
