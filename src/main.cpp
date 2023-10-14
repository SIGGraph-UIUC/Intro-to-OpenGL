#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera_util.h"
#include "shader_util.h"

const int width = 800;
const int height = 600;

void framebuffer_resize(GLFWwindow* window, int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

unsigned int create_triangle()
{
    const float vertices[] = {-0.5f, -0.5f, 0.0f,
                              0.5f, -0.5f, 0.0f, 
                              0.0f, 0.5f, 0.0f};
    const int indices[] = { 0, 1, 2 };

    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    unsigned int index_buffer = 0;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return vao;
}

int main()
{
    // initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window and make context current
    GLFWwindow* window = glfwCreateWindow(width, height, "Intro to OpenGL", NULL, NULL);
    if (window == NULL)
    {
        // throw exception
        std::cout << "GLFW Error" << std::endl;
    }
    glfwMakeContextCurrent(window);

    // load address of OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
          std::cout << "GLAD Error" << std::endl;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize);

    int shader_program = create_program("vertex.glsl", "fragment.glsl");
    unsigned int vao = create_triangle();

    Camera camera{ window, glm::vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f, 10.0f, 0.1f };

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);

        glm::mat4 projection = camera.get_projection_matrix();
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 model = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &view[0][0]);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        camera.handle_keys(0.0001f);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
