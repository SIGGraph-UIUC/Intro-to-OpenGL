#pragma once

#include <algorithm>
#include <iostream> 

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
    Camera(GLFWwindow* window, const glm::vec3& initial_pos,
        float pitch, float yaw, float speed, float sensitivity);

    float speed_;
    float sensitivity_;

    void handle_keys(float delta_t);
    void handle_mouse(float x, float y, bool left_mouse_held);

    void recompute_vectors();

    glm::vec3 get_pos() const;
    glm::mat4 get_view_matrix() const;
    glm::mat4 get_projection_matrix() const;

private:
    GLFWwindow* window_;
    glm::vec3 origin_, world_up_, front_, right_;
    float pitch_, yaw_;
    float last_x, last_y;
};