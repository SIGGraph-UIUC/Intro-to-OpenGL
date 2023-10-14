#include <array>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera_util.h"
#include "geometry_util.h"
#include "shader_util.h"

const int width = 800;
const int height = 600;

void framebuffer_resize(GLFWwindow* window, int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

unsigned int create_quad()
{
    const float vertices[] = {-1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                              -1.0f, 1.0f, 0.0f, 1.0f, // top-left
                              1.0f, 1.0f, 1.0f, 1.0f, // top-right
                              1.0f, -1.0f, 1.0f, 0.0f}; // bottom-right
    const int indices[] = { 0, 1, 2, 0, 2, 3 };

    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (0 * sizeof(float)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));

    unsigned int index_buffer = 0;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return vao;
}

// The first handle is the handle to the FBO,
// and the rest are handles to the image textures
std::array<unsigned int, 4> create_offscreen_framebuffer() {
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create images that will be attached to the framebuffer
    unsigned int position, normal, diffuse_specular;

    glGenTextures(1, &position);
    glBindTexture(GL_TEXTURE_2D, position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position, 0);

    glGenTextures(1, &normal);
    glBindTexture(GL_TEXTURE_2D, normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal, 0);

    glGenTextures(1, &diffuse_specular);
    glBindTexture(GL_TEXTURE_2D, diffuse_specular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, diffuse_specular, 0);

    // Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    return std::array<unsigned int, 4>{fbo, position, normal, diffuse_specular};
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

    Shader shader{ "vertex.glsl", "fragment.glsl" };
    Shader deferred_shader{ "deferred_vertex.glsl", "deferred_fragment.glsl" };
    unsigned int quad_vao = create_quad();
    std::array<unsigned int, 4> gbuffer = create_offscreen_framebuffer();

    Camera camera{ window, glm::vec3(0.0f, 0.0f, -5.0f), 0.0f, 0.0f, 10.0f, 0.1f };

    ImageTexture diffuse{ "skull/skull_diffuse.jpg" };
    ImageTexture normal{ "skull/skull_normal.jpg" };
    ObjMesh mesh{ "skull/skull.obj" };

    while(!glfwWindowShouldClose(window))
    {
        // First draw to the offscreen gBuffer
        glBindFramebuffer(GL_FRAMEBUFFER, gbuffer[0]);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader.handle);
        glBindVertexArray(mesh.handle);

        glm::mat4 projection = camera.get_projection_matrix();
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 model = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "view"), 1, GL_FALSE, &view[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse.handle);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normal.handle);

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, 3 * mesh.num_triangles, GL_UNSIGNED_INT, 0);

        // Draw to the default framebuffer now
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(deferred_shader.handle);

        // Bind the gBuffer textures for use
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbuffer[1]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gbuffer[2]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gbuffer[3]);

        glm::vec3 camera_pos = camera.get_pos();
        glUniform3f(glGetUniformLocation(deferred_shader.handle, "viewPos"), camera_pos.x, camera_pos.y, camera_pos.z);

        glBindVertexArray(quad_vao);
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        camera.handle_keys(0.0001f);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
