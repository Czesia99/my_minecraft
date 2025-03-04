#include "Shadowmap.hpp"
#include "World.hpp"

namespace game
{
    Shadowmap::Shadowmap()
    {
        depth_shader = Shader("depth_shader.vs", "depth_shader.fs");
        quad_depth_shader = Shader("debug_depth.vs", "debug_depth.fs");

        createShadowMap();
    }

    void Shadowmap::renderShadowmap(const Camera3D &cam)
    {
        GLint m_viewport[4];
        glGetIntegerv( GL_VIEWPORT, m_viewport);

        Camera3D camera(cam);

        depth_shader.use();

        // glBindTextureUnit(0, block_textures);
        lightSpaceMatrix = computeLightSpaceMatrix(camera); //to fit in camera frustrum
        depth_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, shadow_width, shadow_height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        World::instance().renderTerrain(depth_shader, camera);
        glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind

        //reset viewport
        glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Shadowmap::createShadowMap()
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

    void Shadowmap::createShadowQuad()
    {

    }

    glm::mat4 Shadowmap::computeLightSpaceMatrix(Camera3D &camera)
    {
        camera.setNearFarPlanes(0.1f, 85.0f);
        frustrum_corners = camera.getFrustumCornersWorldSpace();
        camera.setNearFarPlanes(0.1f, 1000.0f);

        light_dir = glm::normalize(glm::vec3(lightdir_x, lightdir_y, lightdir_z));

        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const auto& v : frustrum_corners)
        {
            center += glm::vec3(v);
        }
        center /= frustrum_corners.size();

        const auto lightView = glm::lookAt(center - light_dir, center, glm::vec3(0.0f, 1.0f, 0.0f));

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