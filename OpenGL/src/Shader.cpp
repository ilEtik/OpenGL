#include "Shader.h"

#include <sstream>
#include <fstream>

#include "spdlog/spdlog.h"

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: _filePath(filepath), _rendererId(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	_rendererId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(_rendererId));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(_rendererId));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

ShaderProgramSource Shader::ParseShader(const std::string& shaderPath)
{
	std::ifstream stream(shaderPath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			if (type == ShaderType::NONE)
			{
				continue;
			}

			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		spdlog::error("Failed to compile shader type: {0}", type);
		spdlog::error(message);

		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	GLCall(unsigned int program = glCreateProgram());

	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	if (vertexShader == 0 || fragmentShader == 0)
	{
		return 0;
	}

	GLCall(glAttachShader(program, vertexShader));
	GLCall(glAttachShader(program, fragmentShader));

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDetachShader(program, vertexShader));
	GLCall(glDetachShader(program, fragmentShader));

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));

	return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
	auto cachedLocation = _uniformLocationCache.find(name);
	if (cachedLocation != _uniformLocationCache.end())
	{
		return cachedLocation->second;
	}

	GLCall(int location = glGetUniformLocation(_rendererId, name.c_str()));
	
	if (location == -1)
	{
		spdlog::error("Warning: uniform '{0}' doesn't exist!", name);
	}

	_uniformLocationCache[name] = location;
	return location;
}
