#include "main.h"

using namespace std;

const GLint Width = 800, Height = 600; // Window dimensions
GLuint VAO, VBO, shader; // VOA=vertex array object, VBO = vertex buffer object

//vertex shader
static const char* vShader = "\
    #version 330                                                    \n\
    layout(location = 0) in vec3 pos;                                   \n\
    void main()                                                         \n\
    {                                                                   \n\
        gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);           \n\
    }                                                                   \n\
";

//fragment shader
static const char* fShader = "\
    #version 330                                                    \n\
    out vec4 color                                                      \n\
    void main()                                                         \n\
    {                                                                   \n\
        colour = vec4(1.0, 0.0, 0.0, 1.0);          \n\
    }                                                                   \n\
";


void/* main::*/createTriangle() // vao will hold mutiple vbos for creating a traiangle
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, 1024, NULL, eLog);
        fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Failed to create shader\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

}


int main() 
{


    // GLFW initialization
    if (!glfwInit()) {
        printf("GLFW initialization failed");
        return 1;
    }
    

    // Setting window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Creating the window
    GLFWwindow* newWindow = glfwCreateWindow(Width, Height, "Test Window", NULL, NULL);
    if (!newWindow) {
        printf("Error creating window");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(newWindow, &bufferWidth, &bufferHeight);

    // Make the context of the specified window current for the calling thread
    glfwMakeContextCurrent(newWindow);

    // GLEW initialization
    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed");
        glfwDestroyWindow(newWindow);
        glfwTerminate();
        return 1;
    }

    // Set the viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    createTriangle();
    CompileShaders();

    while (!glfwWindowShouldClose(newWindow)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(newWindow);
    }

    // Clean up
    glfwDestroyWindow(newWindow);
    glfwTerminate();
    return 0;
}

