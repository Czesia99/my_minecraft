#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mygl 
{
    class ICamera {
        public:
            ICamera(){} 
            virtual ~ICamera(){}
            virtual glm::mat4 getViewMatrix() const = 0;
            virtual glm::mat4 getProjectionMatrix() const = 0;
        public:
            // glm::vec3 position;
            // glm::vec3 front;
            // glm::vec3 up;
            // glm::vec3 right;
            // glm::vec3 world_up;
    };
}