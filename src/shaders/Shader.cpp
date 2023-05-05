#include "Shader.h"

void Shader::compile(const char* vShaderCode, const char* fShaderCode)
{
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    deleteShader(vertex, fragment);
}

void Shader::checkCompileErrors(const GLuint& shader, const std::string& type)
{
    GLint success;         
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) 
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: "
                << type << "\n" << infoLog << "\n" << std::endl;
        }
    }
    else 
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) 
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: "
                << type << "\n" << infoLog << "\n" << std::endl;
        }
    }
}

void Shader::deleteShader(const unsigned int& vertexID, const unsigned int& fragmentID) const
{
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    compile(vertexCode.c_str(), fragmentCode.c_str());
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use() const
{
    glUseProgram(ID);
}
void Shader::setUniform(const std::string& name, const glm::vec2 vector)
{
    int propertyID = glGetUniformLocation(ID, name.c_str());
    glUniform2fv(propertyID, 1, &vector[0]);
}
void Shader::setUniform(const std::string& name, const glm::vec3 vector)
{
    int propertyID = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(propertyID, 1, &vector[0]);
}
void Shader::setUniform(const std::string& name, const glm::vec4 vector)
{
    int propertyID = glGetUniformLocation(ID, name.c_str());
    glUniform4fv(propertyID, 1, &vector[0]);
}
void Shader::setUniform(const std::string& name, const glm::mat4 matrix)
{
    int propertyID = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(propertyID, 1, GL_FALSE, &matrix[0][0]);
}
void Shader::setUniform(const std::string& name, const float value)
{
    int propertyID = glGetUniformLocation(ID, name.c_str());
    glUniform1f(propertyID, value);
}
