#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 color=vec4(0.0);
    int coreSize=3;
    float texelOffset=1/150.0;
    float kernel[9];
    
    kernel[6]=1;kernel[7]=2;kernel[8]=1;
    kernel[3]=2;kernel[4]=4;kernel[5]=2;
    kernel[0]=1;kernel[1]=2;kernel[2]=1;
    
    //移动高斯核,对原图做卷积计算
    int index=0;
    for(int y=0;y<coreSize;y++)
    {
        for(int x=0;x<coreSize;x++)
        {
            
            //原图像素点
            vec4 currentColor=texture2D(screenTexture,TexCoords+vec2((-1+x)*texelOffset,(-1+y)*texelOffset));
            //卷积计算
            color+=currentColor*kernel[index++];
        }
    }
    //根据邻域内像素的加权平均灰度值去替代模板中心像素点的值
    color/=16.0;
    FragColor = color;
    
  //  vec3 col = texture(screenTexture, TexCoords).rgb;
    
  //  FragColor = vec4(col, 1.0);
}
