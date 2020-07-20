
#include <iostream>

//GLEW
#include <gl/glew.h>
#include <GL/glu.h>

//GLFW
#include <GLFW/glfw3.h>

//EZ declarations
#include "Shader.h"
#include "ComputeShaders.h"
#include "GraphicsUtilities.h"

#define UNUSED(x) x;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


//window dimensions
const GLint gWidth = 1024, gHeight = 512;

constexpr float frameTime = 1.f / 60.f;
constexpr float camMoveSpeed = 1.f;
float x = 0.f;
float y = 0.f;
float z = 0.f;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(gWidth, gHeight, "RayTracer", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(window, (vidmode->width - gWidth) / 2, (vidmode->height - (gHeight + 300)) / 2);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int tex = Graph::CreateFrameBufferTexture(gWidth, gHeight);
    UNUSED(tex);
    Graph::ComputeShader computeShaderProgram{ "Shaders//RayTracing.glcs" };
    computeShaderProgram.Init();
    computeShaderProgram.SetFloat3("eye", x, y, z);
    computeShaderProgram.SetFloat3("lower_left_corner", -2.f, -1.f, -1.f);
    computeShaderProgram.SetFloat3("horizontal", 4.f, 0.f, 0.f);
    computeShaderProgram.SetFloat3("vertical", 0.f, 2.f, 0.f);

    Graph::Shader shaderProgram{ "Shaders//VertShader.vs", "Shaders//FragShader.fs" };

    shaderProgram.Use();
    glUniform1i(glGetUniformLocation(shaderProgram.mID, "srcTex"), 0);

    GLuint vertArray;
    glGenVertexArrays(1, &vertArray);
    glBindVertexArray(vertArray);

    GLuint posBuf;
    glGenBuffers(1, &posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    float data[] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, data, GL_STREAM_DRAW);
    GLint posPtr = glGetAttribLocation(shaderProgram.mID, "pos");
    glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posPtr);
   
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        int worksizeX = gWidth;
        int worksizeY = gHeight;
        
        //Run compute shader
        computeShaderProgram.Use();
        computeShaderProgram.SetFloat3("eye", x, y, z);
        glDispatchCompute(worksizeX / 16, worksizeY / 16, 1);
        glUseProgram(0);

        //Render the image
        shaderProgram.Use();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glUseProgram(0);


        // Swap the screen buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Properly de-allocate all resources once they've outlived their purpose
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return EXIT_SUCCESS;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        z -= camMoveSpeed * frameTime;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        z += camMoveSpeed * frameTime;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        x -= camMoveSpeed * frameTime;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        x += camMoveSpeed * frameTime;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        y -= camMoveSpeed * frameTime;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        y += camMoveSpeed * frameTime;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback([[maybe_unused]]GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}