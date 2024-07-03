#include "Camera3D.hpp"

namespace mygl
{
    Camera3D::Camera3D(glm::vec3 pos, float cam_width, float cam_height, float speed, bool is_fps) 
    :   front(glm::vec3(0.0f, 0.0f, -1.0f)), world_up(UP), mouse_sensitivity(SENSITIVITY), fov(FOV), pitch(PITCH), yaw(YAW)
    {
        initial_pos = pos;
        position = pos;
        width = cam_width;
        height = cam_height;
        fps = is_fps;
        movement_speed = speed;
        update_camera_vectors();
    }

    glm::mat4 Camera3D::get_view_matrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 Camera3D::get_projection_matrix() const
    {
        return glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f);
    }

    void Camera3D::process_keyboard(Camera3D_Movement direction, float delta_time)
    {
        float velocity = movement_speed * delta_time;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        if (fps)
            position.y = initial_pos.y;
    }

    void Camera3D::process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        xoffset *= mouse_sensitivity;
        yoffset *= mouse_sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if (constrainPitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        update_camera_vectors();
    }

    void Camera3D::process_mouse_scroll(float yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

    void Camera3D::update_camera_vectors()
    {
        // calculate the new Front vector
        glm::vec3 nfront;
        nfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        nfront.y = sin(glm::radians(pitch));
        nfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(nfront);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = glm::normalize(glm::cross(right, front));
    }
}