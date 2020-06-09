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

int main(int argc,char *argv[])
{   
    string shader_file1 = argv[1];
    string shader_file2 = argv[2];
    string shader_file3 = argv[3];
    string shader_file4 = argv[4];
    glewExperimental = true;
    char infolog[512];
    if(!glfwInit())
    {
        glfwTerminate();
        return -1;
    }
    string vertex_shader_cube = load_shader(shader_file1);
    string fragment_shader_cube = load_shader(shader_file2);
    string vertex_shader_light = load_shader(shader_file3);
    string fragment_shader_light = load_shader(shader_file4);
    vertex_shader_cube = vertex_shader_cube.substr(0,vertex_shader_cube.length()-2);
    fragment_shader_cube = fragment_shader_cube.substr(0,fragment_shader_cube.length()-2);
    vertex_shader_light = vertex_shader_light.substr(0,vertex_shader_light.length()-2);
    fragment_shader_light = fragment_shader_light.substr(0,fragment_shader_light.length()-2);
    const char *vertexshadersource = vertex_shader_cube.c_str();
    const char *fragmentshadersource = fragment_shader_cube.c_str();
    const char *vertexshadersource1 = vertex_shader_light.c_str();
    const char *fragmentshadersource1 = fragment_shader_light.c_str();

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
        
        cout<<"Vertex shader compilation failed1:"<<infolog<<endl;
        return -1;
    }
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FS,1,&fragmentshadersource,NULL);
    glCompileShader(FS);
    glGetShaderiv(FS,GL_COMPILE_STATUS,&success);
    if(!success)
    {   
        glGetShaderInfoLog(FS,512,NULL,infolog);
        cout<<"Fragment Shader compilation failed1:"<<infolog<<endl;
        //return -1;
    }
    GLuint shader_program1 = glCreateProgram();
    GLuint shader_program2 = glCreateProgram();
    glAttachShader(shader_program1,VS);
    glAttachShader(shader_program1,FS);
    glLinkProgram(shader_program1);
    glGetProgramiv(shader_program1,GL_LINK_STATUS,&success);
    if(!success)
    {
        cout<<"Program linking failed1\n";
        //return -1;
    }
    GLuint VS1,FS1;
    VS1=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VS1,1,&vertexshadersource1,NULL);
    glCompileShader(VS1);
    glGetShaderiv(VS1,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        cout<<"Vertex shader compilation failed2"<<endl;
        glGetShaderInfoLog(VS1,512,NULL,infolog);
        return -1;
    }
    FS1=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FS1,1,&fragmentshadersource1,NULL);
    glCompileShader(FS1);
    glGetShaderiv(FS1,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        cout<<"Fragment shader compilation failed2"<<endl;
        glGetShaderInfoLog(FS1,512,NULL,infolog);
        return -1;
    }
    glAttachShader(shader_program2,VS1);
    glAttachShader(shader_program2,FS1);
    glLinkProgram(shader_program2);
    glGetProgramiv(shader_program2,GL_LINK_STATUS,&success);
    if(!success)
    {
        cout<<"Program linking failed2"<<endl;
        return -1;
    }
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    GLuint VAO,VBO,lightCube;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glGenVertexArrays(1,&lightCube);
    glBindVertexArray(lightCube);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 6 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    do
    {
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program1);
        glBindVertexArray(VAO);
        float cameraSpeed = 2.5 * 0.1; 
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        mat4 model = mat4(1.0f);
        mat4 view = mat4(1.0f);
        mat4 projection = mat4(1.0f);
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = cos(glfwGetTime() * 0.7f);
        lightColor.z = lightColor.x * lightColor.y;//(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor = lightColor * vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor;// * vec3(0.2f);
        glm::vec3 SPC = vec3(1.0f);
        glm::vec3 MA = vec3(1.0f,0.5f,0.31f);
        glm::vec3 MD = vec3(1.0f,0.5f,0.31f);
        glm::vec3 MS = vec3(0.5f);
        GLfloat shininess = 32.0f;
        GLuint modelloc = glGetUniformLocation(shader_program1,"model");
        GLuint viewLoc = glGetUniformLocation(shader_program1,"view");
        GLuint projLoc = glGetUniformLocation(shader_program1,"projection");
        GLuint lightColor_pos = glGetUniformLocation(shader_program1,"lightColor");
        GLuint objectColor_loc = glGetUniformLocation(shader_program1,"objectColor");
        GLuint AC_LOC = glGetUniformLocation(shader_program1,"ambient_cube");
        GLuint DC_loc = glGetUniformLocation(shader_program1,"diffuse_cube");
        GLuint SC_loc = glGetUniformLocation(shader_program1,"specular_cube");
        GLuint SHINE_LOC = glGetUniformLocation(shader_program1,"shininess");
        GLuint POS_LIGHT_LOC = glGetUniformLocation(shader_program1,"position_light");
        GLuint AM_LIGHT_LOC = glGetUniformLocation(shader_program1,"ambient_light");
        GLuint DL_LOC = glGetUniformLocation(shader_program1,"diffuse_light");
        GLuint SL_LOC = glGetUniformLocation(shader_program1,"specular_light");
        GLuint viewPos_loc = glGetUniformLocation(shader_program1,"viewPos");
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(modelloc,1,GL_FALSE,value_ptr(model));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,value_ptr(view));
        glUniformMatrix4fv(projLoc,1,GL_FALSE,value_ptr(projection));
        glUniform3fv(lightColor_pos,1,value_ptr(lightColor));
        glUniform3fv(DL_LOC,1,value_ptr(diffuseColor));
        glUniform3fv(AM_LIGHT_LOC,1,value_ptr(ambientColor));
        glUniform3fv(objectColor_loc,1,value_ptr(objectColor));
        glUniform3fv(viewPos_loc,1,value_ptr(cameraPos));
        glUniform3fv(AC_LOC,1,value_ptr(MA));
        glUniform3fv(DC_loc,1,value_ptr(MD));
        glUniform3fv(SC_loc,1,value_ptr(MS));
        glUniform3fv(POS_LIGHT_LOC,1,value_ptr(lightPos));
        glUniform3fv(viewPos_loc,1,value_ptr(cameraPos));
        glUniform3fv(SL_LOC,1,value_ptr(SL));
        glUniform1f(SHINE_LOC,shininess);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glUseProgram(shader_program2);
        model = mat4(1.0f);
        model = translate(model,lightPos);
        model = scale(model,vec3(1.2f));
        modelloc = glGetUniformLocation(shader_program2,"model");
        viewLoc = glGetUniformLocation(shader_program2,"view");
        projLoc = glGetUniformLocation(shader_program2,"projection");
        glUniformMatrix4fv(modelloc,1,GL_FALSE,value_ptr(model));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,value_ptr(view));
        glUniformMatrix4fv(projLoc,1,GL_FALSE,value_ptr(projection));
        glBindVertexArray(lightCube);
        glDrawArrays(GL_TRIANGLES,0,36);
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