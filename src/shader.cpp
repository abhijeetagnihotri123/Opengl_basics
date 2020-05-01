#include "shader.h"
std::string LoadShader(const std::string);
void CheckShaderError(GLuint,GLuint,bool,const std::string);
GLuint CreateShader(const std::string,GLenum);
Shader::Shader(const std::string &filename)
{
    m_program = glCreateProgram();
    m_shaders[0] = CreateShader(LoadShader(filename+".vs"),GL_VERTEX_SHADER);
    m_shaders[1] = CreateShader(LoadShader(filename+".fs"),GL_FRAGMENT_SHADER);
    for(int i=0;i<NUM_SHADERS;i++)
    {
        glAttachShader(m_program,m_shaders[i]);
    }
    glBindAttribLocation(m_program,0,"position");
    glLinkProgram(m_program);
    CheckShaderError(m_program,GL_LINK_STATUS,true,"Program linking failed\n");
    glValidateProgram(m_program);
    CheckShaderError(m_program,GL_VALIDATE_STATUS,true,"Program validation failed\n");
    m_uniforms[TRANSFORM_U]=glGetUniformLocation(m_program,"transform");
}

Shader::~Shader()
{
    for(int i=0;i<NUM_SHADERS;i++)
    {
        glDetachShader(m_program,m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program);
}
GLuint CreateShader(const std::string text,GLenum shadertype)
{
    GLuint shader = glCreateShader(shadertype);
    if(shader==0)
    {
        return NULL;
    }
    const GLchar *shaderSourceStrings[1];
    GLint shaderSourceStringsLengths[1];
    shaderSourceStrings[0]=text.c_str();
    shaderSourceStringsLengths[0]=text.length();
    glShaderSource(shader,1,shaderSourceStrings,shaderSourceStringsLengths);
    glCompileShader(shader);
    CheckShaderError(shader,GL_COMPILE_STATUS,false,"Error:Shader cannot be compiled\n");
    return shader;
}
std::string LoadShader(const std::string filename)
{
    std::fstream file;
    file.open(filename.c_str());
    std::string output;
    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file,line);
            output.append(line+"\n");
        }
    }
    else
    {
        return NULL;
    }
    return output;
}
void CheckShaderError(GLuint shader,GLuint flag,bool isprogram,const std::string error_message)
{
    GLint success=0;
    GLchar error[1024]={0};
    if(isprogram)
    {
        glGetProgramiv(shader,flag,&success);
    }
    else
    {
        glGetProgramiv(shader,flag,&success);
    }
    if(success==GL_FALSE)
    {
        if(isprogram)
        {
            glGetProgramInfoLog(shader,sizeof(error),NULL,error);
        }
        else
        {
            glGetShaderInfoLog(shader,sizeof(error),NULL,error);
        }
    }
}
void Shader::Bind(void)
{
    glUseProgram(m_program);
}
void Shader::Update(const Transform &transform,const Camera &camera)
{
    glm::mat4 Model =camera.GetViewProjection() * transform.GetModel();
    glUniformMatrix4fv(m_uniforms[TRANSFORM_U],1,GL_FALSE,&Model[0][0]);
}
