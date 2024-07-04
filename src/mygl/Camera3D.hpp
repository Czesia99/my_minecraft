#pragma once

#include "ICamera.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>


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
            Camera3D(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), float cam_width = 800, float cam_height = 600, float speed = 2.5f, bool is_fps = false);
            glm::mat4 getViewMatrix() const;
            glm::mat4 getProjectionMatrix() const;
            void processKeyboard(Camera3D_Movement direction, float delta_time);
            void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
            void processMouseScroll(float yoffset);

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
            void updateCameraVectors();
    };
}