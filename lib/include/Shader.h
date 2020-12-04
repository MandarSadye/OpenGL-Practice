#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "glm/glm.hpp"

class Shader
{
    private:
        std::unordered_map<std::string, int> m_UniformLocationCache;

    public:
        unsigned int m_RendererID;
        const std::vector<std::string>& m_filepath;
        const std::vector<std::string>& m_typeOfShader;
        Shader(const std::vector<std::string>& filepath, const std::vector<std::string>& typeOfShader);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        // Set uniforms
        void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
        void SetUniformMat4f(const std::string& name, glm::mat4 & matrix);
        void SetUniform1f(const std::string& name, float value);
        void SetUniform1i(const std::string& name, int32_t value);

    private:
        int GetUniformLocation(const std::string& name);
        std::string ParseShader(const std::string& filepath);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::vector<std::string>& shadertext, const std::vector<std::string>& typeOfShader);

};
