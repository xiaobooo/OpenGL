#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    //FragColor = vec4(lightColor * objectColor, 1.0);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);

}
