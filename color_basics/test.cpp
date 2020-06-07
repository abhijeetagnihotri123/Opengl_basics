#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
using namespace glm;
string load_shader(string);
const float SCR_WIDTH = 800;
const float SCR_HEIGHT = 600;
vec3 camerapos(0.0f,0.0f,3.0f);
vec3 lightpos(1.2f,1.0f,2.0f);
vec3 Position(0.0f,0.0f,0.0f);
vec3 UP(0.0f,1.0f,0.0f);
vec3 Front(0.0f,0.0f,-1.0f);
int main(int argc,char *argv[])
{   
    glewExperimental = true;
    char infolog[512];
    if(!glfwInit())
    {
        glfwTerminate();
        return -1;
    }
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
    glEnable(GL_DEPTH_TEST);
    string filename1 = argv[1];
    string filename2 = argv[2];
    string filename3 = argv[3];
    string filename4 = argv[4];
    string vertex_shader_cube = load_shader(filename1);
    string fragment_shader_cube = load_shader(filename2);
    string light_vertex_shader = load_shader(filename3);
    string light_fragment_shader = load_shader(filename4);
    vertex_shader_cube = vertex_shader_cube.substr(0,vertex_shader_cube.length()-2);
    fragment_shader_cube = fragment_shader_cube.substr(0,fragment_shader_cube.length()-2);
    light_vertex_shader = light_vertex_shader.substr(0,light_vertex_shader.length()-2);
    light_fragment_shader = light_fragment_shader.substr(0,light_fragment_shader.length()-2);
    const char* color_vs_source = vertex_shader_cube.c_str();
    const char* color_fs_source = fragment_shader_cube.c_str();
    const char* lightcube_vs_source = light_vertex_shader.c_str();
    const char* lightcube_fs_source = light_fragment_shader.c_str();
    GLuint VSC = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VSC,1,&color_vs_source,NULL);
    glCompileShader(VSC);
    GLint success;
    glGetShaderiv(VSC,GL_COMPILE_STATUS,&success);
    if(!success)
    {   
        glGetShaderInfoLog(VSC,512,NULL,infolog);
        
        cout<<"Vertex shader compilation failed:"<<infolog<<endl;
        return -1;
    }
    GLuint FSC = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FSC,1,&color_fs_source,NULL);
    glCompileShader(FSC);
    glGetShaderiv(FSC,GL_COMPILE_STATUS,&success);
    if(!success)
    {   
        glGetShaderInfoLog(FSC,512,NULL,infolog);
        cout<<"Fragment Shader compilation failed:"<<infolog<<endl;
        return -1;
    }
    GLuint shader_program_colors = glCreateProgram();
    glAttachShader(shader_program_colors,VSC);
    glAttachShader(shader_program_colors,FSC);
    glLinkProgram(shader_program_colors);
    glGetProgramiv(shader_program_colors,GL_LINK_STATUS,&success);
    if(!success)
    {
        cout<<"Program linking failed\n";
        return -1;
    }
    GLuint VSL = glCreateShader(GL_VERTEX_SHADER);
    GLuint FSL = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint shader_program_light = glCreateProgram();
    
    glShaderSource(VSL,1,&lightcube_vs_source,NULL);
    glShaderSource(FSL,1,&lightcube_fs_source,NULL);
    glCompileShader(VSL);
    glCompileShader(FSL);
    glGetShaderiv(VSL,GL_COMPILE_STATUS,&success);
    if(!success)
    {   
        glGetShaderInfoLog(VSL,512,NULL,infolog);
        
        cout<<"Vertex shader compilation failed at light:"<<infolog<<endl;
        return -1;
    }
    glGetShaderiv(FSL,GL_COMPILE_STATUS,&success);
    if(!success)
    {   
        glGetShaderInfoLog(FSL,512,NULL,infolog);
        
        cout<<"Fragment shader compilation failed1:"<<infolog<<endl;
        return -1;
    }
    glAttachShader(shader_program_light,VSL);
    glAttachShader(shader_program_light,FSL);
    glLinkProgram(shader_program_light);
    glGetProgramiv(shader_program_light,GL_LINK_STATUS,&success);
    if(!success)
    {
        cout<<"Program linking failed1\n";
        return -1;
    }
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };
    GLuint VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    do
    {
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLuint objcolor = glGetUniformLocation(shader_program_colors,"ObjectColor");
        vec3 cube_color = vec3(1.0f,0.5f,0.31f);
        glUniform3fv(objcolor,1,&cube_color[0]);
        GLuint light_color = glGetUniformLocation(shader_program_light,"lightColor");//vec3(1.0f,1.0f,1.0f);
        vec3 light_colors = vec3(1.0f);
        glUniform3fv(light_color,1,&light_colors[0]);
        mat4 projection = perspective(1.0f,(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.f);
        mat4 view = lookAt(Position,Position+Front,UP);
        mat4 model = mat4(1.0f);
        GLuint proj_loc = glGetUniformLocation(shader_program_colors,"projection");
        GLuint view_loc = glGetUniformLocation(shader_program_light,"view");
        GLuint model_loc = glGetUniformLocation(shader_program_light,"model");
        glUniformMatrix4fv(proj_loc,1,GL_FALSE,&projection[0][0]);
        glUniformMatrix4fv(view_loc,1,GL_FALSE,&view[0][0]);
        glUniformMatrix4fv(model_loc,1,GL_FALSE,&model[0][0]);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(glfwGetKey(window,GLFW_KEY_ESCAPE)!=GLFW_PRESS && 
    glfwWindowShouldClose(window)==0);
    glfwTerminate();
    return 0;
}
string load_shader(string filename)
{
    fstream file;
    string output,line;
    file.open(filename);
    if(!file)
    {
        return "NULL";
    }
    else
    {
        while(file)
        {
            getline(file,line);
            output.append(line+"\n");
        }
    }
    file.close();
    return output;
}