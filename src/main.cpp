#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include "classes/Shader.h"

#include <iostream>

// Solicita GPU NVIDIA/AMD discreta en sistemas Optimus/PowerXpress.
// Colocar esto en la parte superior del fichero, fuera de cualquier función.
#ifdef _WIN32
    extern "C" {
        __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
        __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
    }
#endif
// Prototipo de la funci�n para inicializar GLFW y OpenGL
void initializeGLFW();
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // Llamada a la funci�n de inicializaci�n
    initializeGLFW();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //"D:\Dev\OpenGL\LeornOpenGL\shaders\shader1.fs"
    Shader ourShader("shaders/shader1.vs", "shaders/shader1.fs");

    float vertices[] = {
    // positions        // colors         // texture coords
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(1);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("textures/Idle.jpg", &width, &height, &nrChannels, 0);
    std::cout << "Loaded texture Idle.jpg: " << width << "x" << height << " channels=" << nrChannels << std::endl;

    // Ejecutar justo antes de glTexImage2D
    //printf("window ptr = %p\n", (void*)window);
    //printf("glfwGetCurrentContext() = %p\n", (void*)glfwGetCurrentContext());
    // printf("glGetString(GL_VENDOR) = %s\n", glGetString(GL_VENDOR));
    // printf("glGetString(GL_RENDERER) = %s\n", glGetString(GL_RENDERER));
    // //printf("glGetString(GL_VERSION) = %s\n", glGetString(GL_VERSION));
    // //GLenum err = glGetError();
    // //printf("glGetError() before = 0x%04X\n", err);
    // //GLint align = 0; glGetIntegerv(GL_UNPACK_ALIGNMENT, &align);
    // //printf("GL_UNPACK_ALIGNMENT = %d\n", align);
    // //printf("glIsTexture(%u) = %d\n", texture, glIsTexture(texture));

    // Antes de glTexImage2D: usar formato interno explícito y comprobar errores
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        GLint internalFormat = (nrChannels == 4) ? GL_RGBA8 : GL_RGB8;

        // Calcular la alineación mínima necesaria en base a bytes por fila
        int rowBytes = width * nrChannels;
        GLint unpackAlignment = 1;
        if ((rowBytes % 8) == 0) unpackAlignment = 8;
        else if ((rowBytes % 4) == 0) unpackAlignment = 4;
        else if ((rowBytes % 2) == 0) unpackAlignment = 2;
        else unpackAlignment = 1;

        GLint prevAlignment = 0;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevAlignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        GLenum errAfter = glGetError();
        printf("glGetError() after glTexImage2D = 0x%04X\n", errAfter);

        glGenerateMipmap(GL_TEXTURE_2D);

        // Restaurar la alineación previa por si otras partes del código dependen de ella
        glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    //char alarm = '\a';
    //std::cout << alarm << std::endl; // Hace sonido por el caracter

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);


    // Bind Vertex Array Object first
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // Inputs
        processInput(window);

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap and events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

// Implementaci�n de la funci�n para inicializar GLFW y OpenGL
void initializeGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
