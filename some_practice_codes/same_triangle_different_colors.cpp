#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <string>
using namespace std;
string vertexShader = "#version 430\n"
                      "layout (location = 0) in vec3 pos;\n"
                      "layout (location = 1) in vec3 aColor;\n"
                      "out vec3 ourColor;\n"
                      "void main() {\n"
                      "gl_Position = vec4(pos, 1.0);\n"
                      "ourColor = aColor;"
                      "}";
string fragmentShader = "#version 110\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor = vec4(ourColor,1.0);\n"
    "}";
int main(int argc,char *argv[])
{   
    glewExperimental = true;
    char infolog[512];
    if(!glfwInit())
    {
        glfwTerminate();
        return -1;
    }
    const char *vertexShaderSource = vertexShader.c_str();
    const char *fragmentShaderSource = fragmentShader.c_str();
    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window;
    window = glfwCreateWindow(1024,768,"THREE_COLORED_TRIANGLE",NULL,NULL);
    if(window == NULL)
    {
        glfwTerminate();
        cout<<"Cannot create window"<<endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if(glewInit()!=GLEW_OK)
    {
        return -1;
    }
    glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VS,1,&vertexShaderSource,NULL);
    glCompileShader(VS);
    GLint success;
    glGetShaderiv(VS,GL_COMPILE_STATUS,&success);
    if(!success)
    {   
        glGetShaderInfoLog(VS,512,NULL,infolog);
        
        cout<<"Vertex shader compilation failed:"<<infolog<<endl;
        return -1;
    }
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FS,1,&fragmentShaderSource,NULL);
    glCompileShader(FS);
    glGetShaderiv(FS,GL_COMPILE_STATUS,&success);
    if(!success)
    {   
        glGetShaderInfoLog(FS,512,NULL,infolog);
        cout<<"Fragment Shader compilation failed:"<<infolog<<endl;
        return -1;
    }
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program,VS);
    glAttachShader(shader_program,FS);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program,GL_LINK_STATUS,&success);
    if(!success)
    {
        cout<<"Program linking failed\n";
        return -1;
    }
       float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
}; 
    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0); // This is where we send data to vertexShader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(VAO);
    do
    {
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        float VCL = glGetUniformLocation(shader_program,"ourColor");
        float time = glfwGetTime();
        float green_value = sin(time)/2.0f + 0.5;
        float red_value = cos(time)/2.0 + 0.5;
        float blue_value = green_value*red_value;
        glUniform4f(VCL,red_value,green_value,blue_value,1.0f);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(glfwGetKey(window,GLFW_KEY_ESCAPE)!=GLFW_PRESS && 
    glfwWindowShouldClose(window)==0);
    glfwTerminate();
    return 0;
}