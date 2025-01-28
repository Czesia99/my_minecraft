#include "World.hpp"

namespace game
{
    World::World()
    {
        cube_shadow = Shader("cube_shadow.vs", "cube_shadow.fs");
        depth_shader = Shader("depth_shader.vs", "depth_shader.fs");
        quad_depth_shader = Shader("debug_depth.vs", "debug_depth.fs");

        loadTextureArray(block_textures_path, block_textures, 16, 16, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);
        createDepthQuadTexture();

        cube_shadow.use();
        cube_shadow.setInt("shadowMap", 1);
    };

    void World::renderTerrain(const Shader &shader, const ICamera &camera)
    {
        for (const auto &it : chunks)
        {
            it.second->render(shader, camera);
        }
    }

    void World::render(const Camera3D &camera, const int &width, const int &height)
    {
        renderShadowMap(camera, width, height);

        glEnable(GL_CULL_FACE);
        cube_shadow.use();
        cube_shadow.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        cube_shadow.setVec3("lightDir", lightDir);
        cube_shadow.setVec3("viewPos", camera.position);
        cube_shadow.setMat4("projection", camera.getProjectionMatrix());
        cube_shadow.setMat4("view", camera.getViewMatrix());
        glBindTextureUnit(1, depthMap);
        glBindTextureUnit(0, block_textures);

        renderTerrain(cube_shadow, camera);
    }

    void World::renderShadowMap(const Camera3D &cam, const int &width, const int &height)
    {
        Camera3D camera(cam);

        depth_shader.use();

        glBindTextureUnit(0, block_textures);
        lightSpaceMatrix = computeLightSpaceMatrix(camera); //to fit in camera frustrum
        depth_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, shadow_width, shadow_height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        renderTerrain(depth_shader, camera);
        glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind

        //reset viewport
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void World::createDepthQuadTexture()
    {
        glCreateFramebuffers(1, &depthMapFBO);
        glCreateTextures(GL_TEXTURE_2D, 1, &depthMap);

        glTextureParameteri(depthMap, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(depthMap, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(depthMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(depthMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTextureStorage2D(depthMap, 1, GL_DEPTH_COMPONENT24, shadow_width, shadow_height);

        glNamedFramebufferTexture(depthMapFBO, GL_DEPTH_ATTACHMENT, depthMap, 0);
        glNamedFramebufferDrawBuffer(depthMapFBO, GL_NONE);
        glNamedFramebufferReadBuffer(depthMapFBO, GL_NONE);

        float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTextureParameterfv(depthMap, GL_TEXTURE_BORDER_COLOR, border_color);

        quad_depth_shader.use();
        quad_depth_shader.setInt("texture0", 0);
    }

    glm::mat4 World::computeLightSpaceMatrix(Camera3D &camera)
    {
        camera.setNearFarPlanes(0.1f, 85.0f);
        frustrum_corners = camera.getFrustumCornersWorldSpace();
        camera.setNearFarPlanes(0.1f, 1000.0f);

        lightDir = glm::normalize(glm::vec3(-0.3, -1.0, 0.2));

        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const auto& v : frustrum_corners)
        {
            center += glm::vec3(v);
        }
        center /= frustrum_corners.size();

        const auto lightView = glm::lookAt(center - lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();
        for (const auto& v : frustrum_corners)
        {
            const auto trf = lightView * v;
            minX = std::min(minX, trf.x);
            maxX = std::max(maxX, trf.x);
            minY = std::min(minY, trf.y);
            maxY = std::max(maxY, trf.y);
            minZ = std::min(minZ, trf.z);
            maxZ = std::max(maxZ, trf.z);
        }
        constexpr float zMult = 5.0f;
        if (minZ < 0)
        {
            minZ *= zMult;
        }
        else
        {
            minZ /= zMult;
        }
        if (maxZ < 0)
        {
            maxZ /= zMult;
        }
        else
        {
            maxZ *= zMult;
        }

        lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

        return lightSpaceMatrix = lightProjection * lightView;
    }

}