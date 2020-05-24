#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
using namespace std;
static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};
string vertexShader = "#version 110\n"
                           "attribute vec3 pos;\n"
                           "void main() {\n"
                           "gl_Position = vec4(pos, 1.0);\n"
                           "}";

// string vertexShader = "#version 430\n"
//                            "attribute vec3 pos;\n"
//                            "void main() {\n"
//                            "gl_Position = vec4(pos, 1.0);\n"
//                            "}"; 
  
string fragmentShader = "#version 110\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor = ourColor;\n"
    "}";
int main()
{   
    glewExperimental = true;
    if(!glfwInit())
    {
        return -1;
    }
    char infoLog[512];
    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window;
    window = glfwCreateWindow(1024,768,"WINDOW",NULL,NULL);
    if(window == NULL)
    {
        return -1;
    }
    const char *vertexShadersource = vertexShader.c_str();
    const char *fragmentShadersource = fragmentShader.c_str();
    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window,glfwSetFramebufferSizeCallback);
    glewExperimental = true;
    if(glewInit()!=GLEW_OK)
    {
        return -1;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShadersource,NULL);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {   
        cout<<"Vertex shader compilation problem encountered\n";
        return -1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShadersource,NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout<< "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog <<endl;
    }
    int shader_program = glCreateProgram();
    glAttachShader(shader_program,vertexShader);
    glAttachShader(shader_program,fragmentShader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program,GL_LINK_STATUS,&success);
    if(!success)
    {
        cout<<"Program linking failed\n";
        //return -1;
    }
    float vertices [] = {
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        0.0f,  0.5f, 0.0f 
    };
    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO);
do{
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    glUseProgram(shader_program);
    float time = glfwGetTime();
    float green_value = sin(time)/2.0f + 0.5;
    float red_value = cos(time)/2.0 + 0.5;
    float blue_value = green_value*red_value;
    float vertexColorLocation = glGetUniformLocation(shader_program, "ourColor");
    glUniform4f(vertexColorLocation, red_value, green_value, blue_value, 1.0f);
    glDrawArrays(GL_TRIANGLES,0,3);
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

} 
while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
       glfwWindowShouldClose(window) == 0 );
    glfwTerminate();
    return 0;
}