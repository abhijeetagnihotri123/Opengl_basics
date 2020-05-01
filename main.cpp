#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "shader.h"
#include "Mesh.h"
#include <math.h>
#define WIDTH 800
#define HEIGHT 600
using namespace std;

int main()
{
    Display display(800,600,"Hello World!");
    string str="/home/agnitude5/Desktop/opengl_tut/shaders/basic_shader";
    Shader shader(str);
    Vertex vertices[]={
    Vertex(glm::vec3(-0.5,-0.5,0)),
    Vertex(glm::vec3(0,0.5,0)),
    Vertex(glm::vec3(0.5,-0.5,0)),
    };
    Transform transform;
    Camera camera(glm::vec3(0,0,-3),70.0f,(float)WIDTH/(float)HEIGHT,0.01f,1000.0f);
    float counter=0.0;
    while(!display.isClosed())
    {
        display.Clear(0.0f,0.15f,0.3f,1.0f);
        //transform.GetPos().x=sinf(counter);
        //transform.GetRot().z=counter*50;
        shader.Bind();
        Mesh mesh(vertices,sizeof(vertices)/sizeof(vertices[0]));
        vertices[0].GetPos().x+=abs(sin(counter*3.14/180)*0.1);
        vertices[1].GetPos().x+=abs(sin(counter*3.14/180)*0.1);
        vertices[2].GetPos().x+=abs(sin(counter*3.14/180)*0.1);
        mesh.Draw();
        shader.Update(transform,camera);
        display.Swap_Buffer();
        counter+=0.01;
    }
    return 0;
}
