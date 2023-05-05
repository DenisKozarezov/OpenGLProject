#pragma once
#include <pch.h>

struct Shader
{
private:
	unsigned int ID;
	void compile(const char* vShaderCode, const char* fShaderCode);
	void checkCompileErrors(const GLuint& shader, const std::string& type);
	void deleteShader(const unsigned int& vertexID, const unsigned int& fragmentID) const;
public:
	Shader() = delete;
	Shader(const std::string& vs, const std::string& fs);
	~Shader();
	void use() const;

	void setUniform(const std::string& name, const glm::vec2 vector);
	void setUniform(const std::string& name, const glm::vec3 vector);
	void setUniform(const std::string& name, const glm::vec4 vector);
	void setUniform(const std::string& name, const glm::mat4 matrix);
	void setUniform(const std::string& name, const float value);
};

