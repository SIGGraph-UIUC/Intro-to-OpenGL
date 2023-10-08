#include "camera_util.h"

#include <glm/gtc/matrix_transform.hpp>

static void mouse_callback(GLFWwindow* window, double x, double y) {
    Camera* camera =
        reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        camera->handle_mouse(
            x, y,
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    }
}

Camera::Camera(GLFWwindow* window, const glm::vec3& initial_pos,
    float pitch, float yaw, float speed, float sensitivity)
    : window_{ window }, origin_{ initial_pos }, pitch_{ pitch }, yaw_{ yaw },
    speed_{ speed }, sensitivity_{ sensitivity } {
    glfwSetWindowUserPointer(window_, reinterpret_cast<void*>(this));
    glfwSetCursorPosCallback(window_, mouse_callback);

    world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    recompute_vectors();
}

void Camera::recompute_vectors() {
    float sin_pitch = sin(glm::radians(pitch_));
    float cos_pitch = cos(glm::radians(pitch_));
    float sin_yaw = sin(glm::radians(yaw_));
    float cos_yaw = cos(glm::radians(yaw_));
    front_ = glm::normalize(
        glm::vec3(cos_pitch * sin_yaw, sin_pitch, cos_pitch * cos_yaw));

    right_ = glm::normalize(glm::cross(front_, world_up_));
}

void Camera::handle_keys(float delta_t) {
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        origin_ += delta_t * speed_ * front_;
    }

    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        origin_ -= delta_t * speed_ * front_;
    }

    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        origin_ -= delta_t * speed_ * right_;
    }

    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        origin_ += delta_t * speed_ * right_;
    }

    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        origin_ += delta_t * speed_ * world_up_;
    }

    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
        origin_ -= delta_t * speed_ * world_up_;
    }
}

void Camera::handle_mouse(float x, float y, bool left_mouse_held) {
    static bool first_mouse = true;

    if (first_mouse) {
        last_x = x;
        last_y = y;
        first_mouse = false;
    }

    if (left_mouse_held) {
        float delta_x = last_x - x;
        float delta_y = last_y - y;

        yaw_ += delta_x * sensitivity_;
        pitch_ = std::clamp(pitch_ + delta_y * sensitivity_, -89.0f, 89.0f);

        recompute_vectors();
    }

    last_x = x;
    last_y = y;
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(origin_, origin_ + front_, world_up_);
}

glm::mat4 Camera::get_projection_matrix() const {
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    return glm::perspective(90.0f, static_cast<float>(width) / static_cast<float>(height), 0.01f, 100.0f);
}