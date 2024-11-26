#include "Entity.hpp"

namespace game
{
    Entity::Entity(int id, glm::vec3 pos, float pitch, float yaw, std::string name)
    : id(id), pos(pos), pitch(pitch), yaw(yaw), name(name) {

        entity_shader = Shader("entity_shader.vs", "entity_shader.fs");
    }

    void Entity::render(const Shader &shader, const ICamera &camera)
    {
        cube.render(shader, camera);
    }

    void Entity::setValues(glm::vec3 npos, float npitch, float nyaw)
    {
        pos = npos;
        pitch = npitch;
        yaw = nyaw;
        // name = nname;

        cube.transform.position = pos;
    }
}