#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <utility>

#include "ClassicDemoTemplate.h"

ClassicDemoTemplate::Pixel ClassicDemoTemplate::Pixel::operator+(const Pixel &p) const
{
    return std::move(
        Pixel(
            (p.R + this->R > 255) ? 255 : p.R + this->R,
            (p.G + this->G > 255) ? 255 : p.G + this->G,
            (p.B + this->B > 255) ? 255 : p.B + this->B));
}

ClassicDemoTemplate::Pixel ClassicDemoTemplate::Pixel::operator-(const Pixel &p) const
{
    return std::move(
        Pixel(
            (p.R - this->R < 0) ? 0 : p.R + this->R,
            (p.G - this->G < 0) ? 0 : p.G + this->G,
            (p.B - this->B < 0) ? 0 : p.B + this->B));
}

//Window, Engine and OpenGL initialization
bool ClassicDemoTemplate::Construct(const char *name, const int width, const int height, const bool fullscreen)
{
    if (!glfwInit())
        return ShowError("Failed to initialize GLFW");

    SetOpenGLVersion();
    SetWindowName(name);

    if (fullscreen)
        CreateFullscrenWindow();
    else
        CreateWindow(width, height);

    if (window == NULL)
        return ShowError("Failed to open GLFW window");

    AddGLFWOptions();
    InitEngineData();

    InitOpenGL();

    if (!Init())
        return ShowError("Failed to execute user defined Init function");

    return true;
}

#pragma region[CONSTRUCT PRIVATE FUNCTIONS]

void ClassicDemoTemplate::SetWindowName(const char *name)
{
    this->name = name;
}

void ClassicDemoTemplate::CreateFullscrenWindow()
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorPhysicalSize(monitor, &this->width, &this->height);
    window = glfwCreateWindow(this->width, this->height, name, monitor, NULL);
}

void ClassicDemoTemplate::CreateWindow(const int width, const int height)
{
    this->width = width;
    this->height = height;

    window = glfwCreateWindow(width, height, name, NULL, NULL);
}

void ClassicDemoTemplate::AddGLFWOptions()
{
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void ClassicDemoTemplate::InitEngineData()
{
    screenData = new unsigned char[width * height * channels];

    clockOld = std::chrono::system_clock::now();
    clockNow = clockOld;
    deltaTime = 0.f;
}

bool ClassicDemoTemplate::ShowError(const char *message)
{
    std::cerr << message << std::endl;
    glfwTerminate();
    return false;
}

#pragma endregion

#pragma region[OPENGL PRIVATE FUNCTIONS]

void ClassicDemoTemplate::SetOpenGLVersion()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
}

void ClassicDemoTemplate::InitOpenGL()
{
    InitGlew();
    CreateVAOandVBO();
    CreateElementBuffers();
    CreateOpenGLProgram();
    SetShaderVariables();
    SetTexture();
}

void ClassicDemoTemplate::InitGlew()
{
    glewExperimental = GL_TRUE;
    glewInit();
}

void ClassicDemoTemplate::CreateElementBuffers()
{
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0};

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void ClassicDemoTemplate::CreateVAOandVBO()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {//Every corner of the window
                          //Position | Texcoords
                          -1.0f, 1.0f, 0.0f, 0.0f,
                          1.0f, 1.0f, 1.0f, 0.0f,
                          1.0f, -1.0f, 1.0f, 1.0f,
                          -1.0f, -1.0f, 0.0f, 1.0f};

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void ClassicDemoTemplate::CreateOpenGLProgram()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vs = GetVertexShader();
    glShaderSource(vertexShader, 1, &vs, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs = GetFragmentShader();
    glShaderSource(fragmentShader, 1, &fs, NULL);
    glCompileShader(fragmentShader);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glBindFragDataLocation(programID, 0, "outColor");
    glLinkProgram(programID);
    glUseProgram(programID);

    glDetachShader(programID, fragmentShader);
    glDetachShader(programID, vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
}

const char *ClassicDemoTemplate::GetVertexShader()
{
    return R"glsl(
    #version 150 core
    in vec2 position;
    in vec2 texcoord;
    out vec2 Texcoord;
    void main()
    {
        Texcoord = texcoord;
        gl_Position = vec4(position, 0.0, 1.0);
    }
    )glsl";
}

const char *ClassicDemoTemplate::GetFragmentShader()
{
    return R"glsl(
    #version 150 core
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D screenData;
    void main()
    {
        outColor = texture(screenData, Texcoord);
    }
    )glsl";
}

void ClassicDemoTemplate::SetShaderVariables()
{
    GLint posAttrib = glGetAttribLocation(programID, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    GLint texAttrib = glGetAttribLocation(programID, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
}

void ClassicDemoTemplate::SetTexture()
{
    glGenTextures(1, &textureID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, screenData);
    glUniform1i(glGetUniformLocation(programID, "screenData"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

#pragma endregion

//Engine basic update
void ClassicDemoTemplate::Run(float duration)
{
    while (glfwWindowShouldClose(window) == 0 && duration > 0)
    {
        UpdateInput();
        UpdateTime();

        if (!Update(deltaTime))
            return; //Update function defined by the user

        DrawToScreen();

        duration -= deltaTime;
    }
}

#pragma region[RUN PRIVATE FUNCTIONS]

void ClassicDemoTemplate::UpdateInput()
{
    glfwPollEvents();

    int state = glfwGetKey(window, GLFW_KEY_ESCAPE);
    if (state == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

void ClassicDemoTemplate::UpdateTime()
{
    clockOld = clockNow;
    clockNow = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_seconds = clockNow - clockOld;
    deltaTime = elapsed_seconds.count();
}

void ClassicDemoTemplate::DrawToScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, screenData);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}

#pragma endregion

//Window and variables deletion
bool ClassicDemoTemplate::Close()
{
    bool ret = Destroy(); //Destroy function defined by the user

    delete[] screenData;

    glfwDestroyWindow(window);
    glfwTerminate();

    return ret;
}
