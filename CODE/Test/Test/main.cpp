#include <GLUT/GLUT.h>
//正方形的位置和大小
GLfloat x1 = 100.0f;
GLfloat y1 = 150.0f;
GLsizei rsize = 50;
//正方形运动变化的步长
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;
//窗口的大小
GLfloat windowWidth;
GLfloat windowHeight;

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f,0.0f,0.0f);
    glRectf(x1,y1,x1+rsize,y1+rsize);
    
    glutSwapBuffers();//清空命令缓冲区并交换帧缓存
}
void ChangeSize(GLsizei w,GLsizei h)
{
    if (h == 0)
    {
        h = 1;
    }
    glViewport(0,0,w,h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (w <= h)
    {
        windowHeight = 250.0f*h/w;
        windowWidth = 250.0f;
    }
    else
    {
        windowHeight = 250.0f;
        windowWidth =250.0f*w/h;
    }
    glOrtho(0.0f,windowWidth,0.0f,windowHeight,1.0f,-1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void TimerFunction(int value)
{
    //处理到达窗口边界的正方形，使之反弹
    if (x1 > windowWidth - rsize || x1 < 0)
    {
        xstep =-xstep;
    }
    
    if (y1 > windowHeight -rsize || y1 <0)
    {
        ystep = -ystep;
    }
    if (x1 > windowWidth - rsize)
    {
        x1 = windowWidth - rsize -1;
    }
    if (y1 > windowHeight -rsize)
    {
        y1 = windowHeight -rsize -1;
    }
    //根据步长修改正方形的位置
    x1 += xstep;
    y1 += ystep;
    
    //用新坐标重新绘图
    glutPostRedisplay();
    glutTimerFunc(50,TimerFunction,value);
}
void SetupRC()
{
    //设置窗口的清除色为蓝色
    glClearColor(0.0f,0.0f,1.0f,1.0f);
}

int main()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Bounce");
    
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(500,TimerFunction,1);
    
    SetupRC();
    glutMainLoop();
    
    return 0;
}
