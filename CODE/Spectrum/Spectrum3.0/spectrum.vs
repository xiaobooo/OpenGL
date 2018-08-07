#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    Normal = mat3(transpose(inverse(model))) * vec3(0.0f,0.0f,-1.0f);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);

}
