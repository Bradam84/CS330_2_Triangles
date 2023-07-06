// Assignment: CS 330 2-3 2-D Triangles
// Author: Bradley Goerke
// Date: 7/4/2023

#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>



// Title for window
const char* const WINDOW_TITLE = "CS 330 2-3 Bradley Goerke";

// Sets the dimensions of the window
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Struct for holding GL mesh 
struct GLMesh
{
	GLuint vao; // Holds the vertex array object
	GLuint vbos[2]; // Holds the vertex buffer objects
	GLuint nIndices; // Number of indices in the mesh
};

// Main GLFW window
GLFWwindow* gWindow = nullptr;
// Triangle mesh data
GLMesh gMesh;
// Shader Program
GLuint gProgramID;

using namespace std; 

// Declare Functions for the window, and graphics
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* winodw, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreateMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

// Vertex Shader Program Source Code
const char *vertexShaderSource = "#version 440 core\n"
	"layout (location =0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 colorFromVBO;\n"
    "out vec4 colorFromVS;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   colorFromVS = colorFromVBO;\n"
    "}\n\0";

// Fragment Shader Program Source Code
const char* fragmentShaderSource = "#version 440 core\n"
"in vec4 colorFromVS;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = colorFromVS;\n"
"}\n\0";

// The one and only main function.
int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh from the Vertex Buffer Object
    UCreateMesh(gMesh);

    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramID))
        return EXIT_FAILURE;

    // Sets the default color of the background of the window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black

    // Render Loop
    while (!glfwWindowShouldClose(gWindow))
    {
        UProcessInput(gWindow); // Processes input

        URender(); // Renders the frame

        glfwPollEvents();
    }

    UDestroyMesh(gMesh); // Destroys the mesh data upon closing of the window

    UDestroyShaderProgram(gProgramID); // Releases the shader program

    exit(EXIT_SUCCESS); // Terminates the program
}

// Initialize OpenGL and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // Intialize GLFW and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Window creation
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}

// Processes all inputs
void UProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Window resizing by OS or user
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Function to render a frame
void URender()
{
    // Clear the background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glClear(GL_COLOR_BUFFER_BIT);

    // Sets the shader to utilized
    glUseProgram(gProgramID);

    // Activate the VBOs that contain the mesh's VAO
    glBindVertexArray(gMesh.vao);

    // Draws the mesh
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangles

    // Deactivate the VAO
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// Implement the Mesh function
void UCreateMesh(GLMesh& mesh)
{
    GLfloat verts[] =
    {
        // Two Triangles
        // Left Triangle uses indices: 0, 1, 2
        // Right Triangle uses indices: 2, 3, 4

        // Index 0
        -1.0f, 1.0f, 0.0f, // Top left of screen
        1.0f, 0.0f, 0.0f, 1.0f, // Red

        // Index 1
        -1.0f, 0.0f, 0.0f, // Center left of screen
        0.0f, 0.0f, 1.0f, 1.0f, // Blue

        // Index 2 Shared vertex
        -0.5f, 0.0f, 0.0f, // Center slight left of screen
        0.0f, 1.0f, 0.0f, 1.0f, // Green

        // Index 3
        0.0f, 0.0f, 0.0f, // Center of screen
        1.0f, 0.0f, 0.0f, 1.0f, // Red

        // Index 4
        0.0f, -1.0f, 0.0f, // Bottom center of screen
        0.0f, 1.0f, 0.0f, 1.0f // Green
    };

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    // Create 2 buffers, one for the vertex data and the other for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the Buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends the coordinate data to the GPU

    // Buffer object for indices
    GLushort indices[] = { 0, 1, 2, 3, 4 };
    mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
    const GLuint floatsPerColor = 4; // Number of colors: (Red, Green, Blue, Alhpa)

    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor);

    // Creates the Vertex Attribut Pointer
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}

void UDestroyMesh(GLMesh &mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}

bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programID)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create the shader program object
    programID = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programID, vertexShaderId);
    glAttachShader(programID, fragmentShaderId);

    glLinkProgram(programID);   // links the shader program
    // check for linking errors
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programID);    // Uses the shader program

    return true;
}

void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}