#pragma once

#include "MYGL/Shader.h"
#include "MYGL/Camera3D.hpp"

#include "glm/gtc/matrix_transform.hpp"


namespace game
{
    using namespace mygl;
    class Shadowmap
    {
        public:
            Shadowmap();
            void renderShadowmap(const Camera3D &camera);
            unsigned int depthMap;
            //void renderShadowmapQuad(const CameraOrto &camera);
            glm::vec3 lightDir = glm::normalize(glm::vec3(-0.3, -1.0, 0.2));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        private:
            Shader depth_shader;
            Shader quad_depth_shader;

            unsigned int depthMapFBO;
            const unsigned int shadow_width = 4096, shadow_height = 4096;
            float near_plane = -40.0f;
            float far_plane = 40.0f;
            glm::mat4 lightProjection = glm::ortho(-64.0f, 64.0f, -64.0f, 64.0f, near_plane, far_plane);
            glm::mat4 lightView = glm::lookAt(-lightDir, glm::vec3( 0.0f, 0.0f,  0.0f), glm::vec3( 0.0f, 1.0f,  0.0f));


        private:
            void createShadowMap();
            void createShadowQuad();
            glm::mat4 computeLightSpaceMatrix(Camera3D &camera);
            std::vector<glm::vec4> frustrum_corners;
    };
}