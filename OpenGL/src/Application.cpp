#include <iostream>
#include <format>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define ASSERT(X)
#define GLCall(x) x
#endif

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << std::format("[OpenGL Error] ({0}): {1} {2}:{3}", error, function, file, line) << std::endl;
        return false;
    }

    return true;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& shaderPath)
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
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

        std::cout << std::format("Failed to compile shader type: {0}\n", type);
        std::cout << message << std::endl;

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
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

int main()
{
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
    {
        std::cout << std::format("Glew Init failed with error: {0}\n", glewInitResult);
        glfwTerminate();
        return -1;
    }

    GLCall(std::cout << std::format("OpenGL Version: {0}\n", (const char*)glGetString(GL_VERSION)));

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}