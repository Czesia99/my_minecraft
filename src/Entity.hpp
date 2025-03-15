#pragma once

#include "MYGL/Shape.hpp"
#include "MYGL/Transform.hpp"
#include "MYGL/ICamera.hpp"

#include <glm/glm.hpp>

namespace game
{
    using namespace mygl;

    class Entity {
        public:
            Entity(int id, glm::vec3 pos, float pitch, float yaw, std::string name);
            ~Entity();
            void render(const Shader &shader, const ICamera &camera);
            void setValues(glm::vec3 npos, float npitch, float nyaw);
        public:
            Shader entity_shader;
            int id;
            glm::vec3 pos;
            float pitch;
            float yaw;
            std::string name;
        private:
            Cube cube;
            Cube cube2;
    };
}
