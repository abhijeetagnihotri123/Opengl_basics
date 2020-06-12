#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
using namespace glm;
string load_shader(string&);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
vec3 cameraPos(0.0f,0.0f,3.0f);
vec3 cameraFront(0.0f,0.0f,-1.0f);
vec3 cameraUp(0.0f,1.0f,0.0f);
bool firstMouse = true;
float Yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float Pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float Fov   =  45.0f;
void framebuffer_size_callback(GLFWwindow*,int,int);
void mouse_callback(GLFWwindow*,double,double);
void scroll_callback(GLFWwindow*,double,double);
void process_window(GLFWwindow*);
int main(int argc,char *argv[])
{   
    int c = argc-1;
    string files[c];
    for(int i=0;i<c;i++)
    {
        files[i]=argv[i+1];
    }
    glewExperimental = true;
    char infolog[512];
    if(!glfwInit())
    {
        glfwTerminate();
        return -1;
    }
    string vertex_shaders[c/2];
    string fragment_shaders[c/2];
    const char *vertex_shader_source[c/2];
    const char *fragment_shader_source[c/2];
    for(int i=0;i<c/2;i++)
    {
        vertex_shaders[i] = load_shader(files[i]);
        fragment_shaders[i] = load_shader(files[i+c/2]);
        vertex_shaders[i] = vertex_shaders[i].substr(0,vertex_shaders[i].length()-2);
        fragment_shaders[i] = fragment_shaders[i].substr(0,fragment_shaders[i].length()-2);
        vertex_shader_source[i] = vertex_shaders[i].c_str();
        fragment_shader_source[i] = fragment_shaders[i].c_str();
    }
    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window;
    window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"THREE_COLORED_TRIANGLE",NULL,NULL);
    if(window == NULL)
    {
        glfwTerminate();
        cout<<"Cannot create window"<<endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glewExperimental = true;
    if(glewInit()!=GLEW_OK)
    {
        return -1;
    }
    glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
    GLuint vs[c/2],fs[c/2],shader_program[c/2];
    for(int i=0;i<c/2;i++)
    {
        vs[i] = glCreateShader(GL_VERTEX_SHADER);
        fs[i] = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vs[i],1,&vertex_shader_source[i],NULL);
        glCompileShader(vs[i]);
        GLint success;
        glGetShaderiv(vs[i],GL_COMPILE_STATUS,&success);
        if(!success)
        {   
            glGetShaderInfoLog(vs[i],512,NULL,infolog);
            printf("vertex_shader compilation failed[%d]\n",i);
            cout<<infolog;
            return -1;
        }
        glShaderSource(fs[i],1,&fragment_shader_source[i],NULL);
        glCompileShader(fs[i]);
        glGetShaderiv(fs[i],GL_COMPILE_STATUS,&success);
        if(!success)
        {   
            glGetShaderInfoLog(fs[i],512,NULL,infolog);
            printf("fragment_shader compilation failed[%d]\n",i);
            cout<<infolog;
            return -1;
        }
        shader_program[0] = glCreateProgram();
        glAttachShader(shader_program[i],vs[i]);
        glAttachShader(shader_program[i],fs[i]);
        glLinkProgram(shader_program[i]);
        glGetProgramiv(shader_program[i],GL_LINK_STATUS,&success);
        if(!success)
        {
            printf("Program linking failed[%d]",i);
            return -1;
        }
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
    GLuint VAO,VBO,lightCube;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glGenVertexArrays(1,&lightCube);
    glBindVertexArray(lightCube);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    GLuint modelloc[c/2];
    GLuint viewLoc[c/2];
    GLuint projLoc[c/2];
    do
    {
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        process_window(window);
        mat4 model = mat4(1.0f);
        mat4 view = mat4(1.0f);
        mat4 projection = mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindVertexArray(VAO);
        glUseProgram(shader_program[0]);
        for(int i=0;i<c/2;i++)
        {    
            modelloc[i] = glGetUniformLocation(shader_program[i],"model");
            viewLoc[i] = glGetUniformLocation(shader_program[i],"view");
            projLoc[i] = glGetUniformLocation(shader_program[i],"projection");
        }
        glBindVertexArray(VAO);
        model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));
        glUniformMatrix4fv(modelloc[0],1,GL_FALSE,value_ptr(model));
        glUniformMatrix4fv(viewLoc[0],1,GL_FALSE,value_ptr(view));
        glUniformMatrix4fv(projLoc[0],1,GL_FALSE,value_ptr(projection));
        glDrawArrays(GL_TRIANGLES,0,36);
        // model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));
        // glUniformMatrix4fv(modelloc[1],1,GL_FALSE,value_ptr(model));
        // glUniformMatrix4fv(viewLoc[1],1,GL_FALSE,value_ptr(view));
        // glUniformMatrix4fv(projLoc[1],1,GL_FALSE,value_ptr(projection));
        // glDrawArrays(GL_TRIANGLES,0,36);
        // model = glm::translate(model, glm::vec3(-0.75f,-0.75f, 0.0f));
        // glUniformMatrix4fv(modelloc[2],1,GL_FALSE,value_ptr(model));
        // glUniformMatrix4fv(viewLoc[2],1,GL_FALSE,value_ptr(view));
        // glUniformMatrix4fv(projLoc[2],1,GL_FALSE,value_ptr(projection));
        // glDrawArrays(GL_TRIANGLES,0,36);
        // model = glm::translate(model, glm::vec3(0.75f,-0.75f, 0.0f));
        // glUniformMatrix4fv(modelloc[3],1,GL_FALSE,value_ptr(model));
        // glUniformMatrix4fv(viewLoc[3],1,GL_FALSE,value_ptr(view));
        // glUniformMatrix4fv(projLoc[3],1,GL_FALSE,value_ptr(projection));
        // glDrawArrays(GL_TRIANGLES,0,36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(glfwGetKey(window,GLFW_KEY_ESCAPE)!=GLFW_PRESS && 
    glfwWindowShouldClose(window)==0);
    glfwTerminate();
    return 0;
}
string load_shader(string &filename)
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
void mouse_callback(GLFWwindow *window,double xpos,double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xOffset = xpos - lastX;
    float yOffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    if(Pitch > 89.9)
    {
        Pitch = 89.9;
    }
    if(Pitch < -89.9)
    {
        Pitch = -89.9;
    }
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    cameraFront = glm::normalize(front);
}
void scroll_callback(GLFWwindow *window,double xoffset,double yoffset)
{
    Fov -= (float)yoffset;
    if(Fov < 1.0)
    {
        Fov = 1.0f;
    }
    if(Fov > 45.0)
    {
        Fov = 45.0f;
    }
}
void framebuffer_size_callback(GLFWwindow *window,int width,int height)
{
    glViewport(0,0,width,height);
}
void process_window(GLFWwindow *window)
{
    float cameraSpeed = 2.5 * 0.1;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}