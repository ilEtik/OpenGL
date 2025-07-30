#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	unsigned int _rendererId;
	std::string _filePath;
	std::unordered_map<std::string, int> _uniformLocationCache;

private:
	ShaderProgramSource ParseShader(const std::string& shaderPath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);

	int GetUniformLocation(const std::string& name);
};