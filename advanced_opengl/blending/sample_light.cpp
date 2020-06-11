#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;
using namespace glm;
string load_shader(string&);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
vec3 cameraPos(0.0f,0.0f,3.0f);
vec3 cameraFront(0.0f,0.0f,-1.0f);
vec3 cameraUp(0.0f,1.0f,0.0f);
vec3 lightPos(1.2f,1.0f,2.0f);
vec3 SL(1.0f);
vec3 objectColor(1.0f,0.5f,0.31f);
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
GLuint load_texture(const char*);
int main(int argc,char *argv[])
{   
    string shader_file1 = argv[1];
    string shader_file2 = argv[2];
    glewExperimental = true;
    char infolog[512];
    if(!glfwInit())
    {
        glfwTerminate();
        return -1;
    }
    string vertex_shader_cube = load_shader(shader_file1);
    string fragment_shader_cube = load_shader(shader_file2);
    vertex_shader_cube = vertex_shader_cube.substr(0,vertex_shader_cube.length()-2);
    fragment_shader_cube = fragment_shader_cube.substr(0,fragment_shader_cube.length()-2);
    const char *vertexshadersource = vertex_shader_cube.c_str();
    const char *fragmentshadersource = fragment_shader_cube.c_str();
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
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glewExperimental = true;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    if(glewInit()!=GLEW_OK)
    {
        return -1;
    }
    glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VS,1,&vertexshadersource,NULL);
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
    glShaderSource(FS,1,&fragmentshadersource,NULL);
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
    if(!success)
    {
        cout<<"Light program linking failed"<<endl;
        return -1;
    }
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    GLuint cubeVAO,cubeVBO;
    glGenVertexArrays(1,&cubeVAO);
    glGenBuffers(1,&cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glBindVertexArray(0);

    GLuint planeVAO,planeVBO;
    glGenVertexArrays(1,&planeVAO);
    glGenBuffers(1,&planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER,planeVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(planeVertices),planeVertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glBindVertexArray(0);
    unsigned int texture;

    GLuint transparentVAO,transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    
    string s = "bricks.jpg";
    const char *path1 = s.c_str();
    GLuint texture1 = load_texture(path1);
    s = "";
    string s1 = "grass.png";
    const char *path2 = s1.c_str();
    GLuint grass_texture = load_texture(path2);
    glEnable(GL_DEPTH_TEST);
    vector<glm::vec3> vegetation
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3( 1.5f, 0.0f, 0.51f),
        glm::vec3( 0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3 (0.5f, 0.0f, -0.6f)
    };
    GLuint texloc = glGetUniformLocation(shader_program,"Texture");
    glUniform1f(texloc,0);
    do
    {
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// | GL_STENCIL_BUFFER_BIT);
        process_window(window);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glUseProgram(shader_program);
        mat4 view = mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        mat4 projection = mat4(1.0f);
        mat4 model = mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        GLuint model_loc= glGetUniformLocation(shader_program,"model");
        GLuint view_loc=glGetUniformLocation(shader_program,"view");
        GLuint proj_loc=glGetUniformLocation(shader_program,"projection");
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        model = glm::translate(model,glm::vec3(-1.0f,0.0f,-1.0f));
        glUniformMatrix4fv(model_loc,1,GL_FALSE,value_ptr(model));
        glUniformMatrix4fv(view_loc,1,GL_FALSE,value_ptr(view));
        glUniformMatrix4fv(proj_loc,1,GL_FALSE,value_ptr(projection));
        glDrawArrays(GL_TRIANGLES,0,36);
        model = mat4(1.0f);
        model = glm::translate(model,glm::vec3(2.0f,0.0f,0.0f));
        GLuint model_loc1 = glGetUniformLocation(shader_program,"model");
        glUniformMatrix4fv(model_loc1,1,GL_FALSE,value_ptr(model));
        glUniformMatrix4fv(view_loc,1,GL_FALSE,value_ptr(view));
        glUniformMatrix4fv(proj_loc,1,GL_FALSE,value_ptr(projection));
        glDrawArrays(GL_TRIANGLES,0,36);
        
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D,texture1);
        model = glm::mat4(1.0f);
        model_loc = glGetUniformLocation(shader_program,"model");
        glUniformMatrix4fv(model_loc,1,GL_FALSE,value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,6);

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
GLuint load_texture(const char *path)
{
    GLuint textureID;
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
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