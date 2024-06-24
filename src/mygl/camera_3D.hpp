#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "icamera.hpp"

namespace mygl 
{
    enum Camera3D_Movement  {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    const float YAW         = -90.0f;
    const float PITCH       =  0.0f;
    const float SENSITIVITY =  0.1f;
    const float FOV         =  45.0f;
    const glm::vec3 UP      =  glm::vec3(0.0f, 1.0f, 0.0f);

    class Camera3D : public ICamera {
        public:
            Camera3D(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), float win_width = 800, float win_height = 600, float speed = 2.5f, bool is_fps = false) : front(glm::vec3(0.0f, 0.0f, -1.0f)), world_up(UP), mouse_sensitivity(SENSITIVITY), fov(FOV), pitch(PITCH), yaw(YAW)
            {
                position = pos;
                initial_pos = pos;
                width = win_width;
                height = win_height;
                fps = is_fps;
                movement_speed = speed;
                update_camera_vectors();
            }

            glm::mat4 get_view_matrix() const
            {
                return glm::lookAt(position, position + front, up);
            }

            glm::mat4 get_projection_matrix() const
            {
                return glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f);
            }

            void process_keyboard(Camera3D_Movement direction, float delta_time)
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

            void process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch = true)
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

            void process_mouse_scroll(float yoffset)
            {
                fov -= (float)yoffset;
                if (fov < 1.0f)
                    fov = 1.0f;
                if (fov > 45.0f)
                    fov = 45.0f;
            }

        public:
            glm::vec3 position;
            glm::vec3 initial_pos;
            glm::vec3 front;
            glm::vec3 up;
            glm::vec3 right;
            glm::vec3 world_up;
            
            float yaw;
            float pitch;
            
            float movement_speed;
            float mouse_sensitivity;
            float fov;

            float width;
            float height;

            bool fps;

        private:
            void update_camera_vectors()
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
    };
}