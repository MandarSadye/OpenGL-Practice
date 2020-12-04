#include "Renderer.h"
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::vector<std::string>& filepath, const std::vector<std::string>& typeOfShader)
    : m_filepath(filepath), m_typeOfShader(typeOfShader), m_RendererID(0)
{
    std::cout << "Number of shaders to fetch: " << m_filepath.size() << std::endl;
    auto sources = std::vector<std::string>(filepath.size());
    for (size_t i = 0; i < m_filepath.size(); i++) {
        sources[i] = ParseShader(m_filepath[i]);
    }

    for (size_t i = 0; i < m_filepath.size(); i++) {
        std::cout << m_typeOfShader[i] << std::endl << sources[i] << std::endl;
    }

    m_RendererID = CreateShader(sources, m_typeOfShader);

    GLCall( glUseProgram(m_RendererID) );
}

Shader::~Shader()
{
    GLCall( glDeleteProgram(m_RendererID) );
}

void Shader::Bind() const
{
    GLCall( glUseProgram(m_RendererID) );
}

void Shader::Unbind() const
{
    GLCall( glUseProgram(0) );
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall( int location = glGetUniformLocation(m_RendererID, name.c_str()) );
    if (location == -1)
        std::cout << "No active uniform variable with name " << name << " found" << std::endl;

    m_UniformLocationCache[name] = location;

    return location;
}

void Shader::SetUniform1i(const std::string& name, int32_t value)
{
    GLCall( glUniform1i(GetUniformLocation(name), value) );
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall( glUniform1f(GetUniformLocation(name), value) );
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GLCall( glUniform4f(GetUniformLocation(name), f0, f1, f2, f3) );
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4 & matrix) {
    GLCall( glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

std::string Shader::ParseShader(const std::string& filepath)
{
    std::cout << "fetching shader from file : " << filepath << std::endl;
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss;

    while (getline(stream, line))
    {
        ss << line << '\n';
    }

    return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall( unsigned int id = glCreateShader(type) );
    const char* src = source.c_str();
    GLCall( glShaderSource(id, 1, &src, nullptr) );
    GLCall( glCompileShader(id) );

    // Error handling
    int result;
    GLCall( glGetShaderiv(id, GL_COMPILE_STATUS, &result) );
    std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compile status: " << result << std::endl;
    if ( result == GL_FALSE )
    {
        int length;
        GLCall( glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length) );
        char* message = (char*) alloca(length * sizeof(char));
        GLCall( glGetShaderInfoLog(id, length, &length, message) );
        std::cout 
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader"
            << std::endl;
        std::cout << message << std::endl;
        GLCall( glDeleteShader(id) );
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::vector<std::string>& shadertext, const std::vector<std::string>& typeOfShader) {
    unsigned int program = glCreateProgram();
    auto refShaders = std::vector<uint32_t>(shadertext.size());
    for (size_t i = 0; i < shadertext.size(); i++ ) {
        uint32_t type_i;
        std::string strtype = typeOfShader[i];
        if (strtype == "GL_VERTEX_SHADER") type_i = GL_VERTEX_SHADER;
        else if (strtype == "GL_FRAGMENT_SHADER") type_i = GL_FRAGMENT_SHADER;

        refShaders[i] = CompileShader(type_i, shadertext[i]);
        GLCall( glAttachShader(program, refShaders[i]) );
    }

    GLCall( glLinkProgram(program) );

    GLint program_linked;

    GLCall( glGetProgramiv(program, GL_LINK_STATUS, &program_linked) );
    std::cout << "Program link status: " << program_linked << std::endl;
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall( glGetProgramInfoLog(program, 1024, &log_length, message) );
        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall( glValidateProgram(program) );

    for (auto i: refShaders) GLCall( glDeleteShader(i) );

    return program;
}
