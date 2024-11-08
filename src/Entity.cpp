#include "Entity.hpp"

namespace game
{
    Entity::Entity(int id, glm::vec3 pos, float pitch, float yaw)
    : id(id), pos(pos), pitch(pitch), yaw(yaw) {

        Shader shader = Shader("entity_shader.vs", "entity_shader.fs");
    }

    void Entity::render(const Shader &shader, const ICamera &camera)
    {
        cube.transform.position = pos;
        cube.render(shader, camera);
    }

    void Entity::setValues(glm::vec3 npos, float npitch, float nyaw)
    {
        pos = npos;
        pitch = npitch;
        yaw = nyaw;
    }
}