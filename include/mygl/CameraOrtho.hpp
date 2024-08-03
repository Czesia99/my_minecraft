#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "ICamera.hpp"

namespace mygl {
    class CameraOrtho : public ICamera {
        public:
            CameraOrtho(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float win_width = 800, float win_height = 600) : front(glm::vec3(0.0f, 0.0f, -1.0f))
            {
                position = position;
                width = win_width;
                height = win_height;

                // fov = 45.0f;
                // yaw = -90.0f;
                // pitch = 0.0f;
                movement_speed = 2.5f;
                mouse_sensitivity = 0.1f;
                world_up = glm::vec3(0.0f, 1.0f, 0.0f);

                update_camera_vectors();
            }

            glm::mat4 get_view_matrix() const
            {
                return glm::lookAt(position, position + front, up);
            }

            glm::mat4 get_projection_matrix() const
            {
                // return glm::ortho(0.f, width, 0.f, height, -1.0f, 1.0f);
                // return glm::ortho(0.f, width, height, 0.f, -1.0f, 1.0f);
                // return glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, -1.0f, 1.0f);
                return glm::ortho(0.0f, width, 0.0f, height, -0.1f, 100.0f);
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
                // calculate the new front vector
                // glm::vec3 nfront;
                // nfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                // nfront.y = sin(glm::radians(pitch));
                // nfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                // front = glm::normalize(nfront);
                // also re-calculate the right and up vector
                right = glm::normalize(glm::cross(front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
                up = glm::normalize(glm::cross(right, front));
            }
    };
}