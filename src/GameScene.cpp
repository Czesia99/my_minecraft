#include "GameScene.hpp"
#include "MYGL/Texture.hpp"
#include "memory.hpp"

#include <algorithm>

namespace game
{
    GameScene::GameScene(Context &ctx) : Scene(ctx)
    {
        storeSceneInCtx();
        if (client.status == -1)
        {
            std::cout << "connexion failed: loading default scene" << std::endl;
            ctx.loadScene(ctx.default_scene);
            ctx.run();
            return;
        }

        camera.setCameraSpeed(100.0f);
        // camera.setCameraNearFarPlanes(0.1f, 50.0f);
        camera_ortho = CameraOrtho(glm::vec3(0.0f, 0.0f, 0.0f), ctx.win_width, ctx.win_height);

        loadTextureArray(block_textures_path2, block_textures, 16, 16, GL_NEAREST, GL_NEAREST);
        loadTexture("../assets/cursor.png", cursor_img.texture);

        cube_shader = Shader("cube_shadow.vs", "cube_shadow.fs");
        cube_shadow = Shader("cube_shadow.vs", "cube_shadow.fs");
        cursor_shader = Shader("cursor.vs", "cursor.fs");
        depth_shader = Shader("depth_shader.vs", "depth_shader.fs");
        quad_depth_shader = Shader("debug_depth.vs", "debug_depth.fs");

        client.startThread();
        client.sendRenderDistance(16);

        cursor_img.transform.scale.x = ctx.win_width;
        cursor_img.transform.scale.y = ctx.win_height;

        createDepthQuadTexture();

        cube_shadow.use();
        cube_shadow.setInt("shadowMap", 1);
        cursor_shader.use();
        cursor_shader.setInt("texture0", 0);

    }

    GameScene::~GameScene()
    {

    }

    void GameScene::storeSceneInCtx()
    {
        ctx.scenes.push_back(this);
    }

    void GameScene::openScene()
    {
        glEnable(GL_DEPTH_TEST);
        glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void GameScene::closeScene()
    {
        return;
    }

    void GameScene::createDepthQuadTexture()
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

    void GameScene::renderCursorQuad()
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        cursor_shader.use();
        glBindTextureUnit(0, cursor_img.texture);
        cursor_img.render(cursor_shader, camera_ortho);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }

    void GameScene::renderShadowMapQuad()
    {
        quad_depth_shader.use();
        quad_depth_shader.setFloat("near_plane", near_plane);
        quad_depth_shader.setFloat("far_plane", far_plane);

        glBindTextureUnit(0, depthMap);
        depth_quad.render(quad_depth_shader, camera_ortho);
    }

    glm::mat4 GameScene::computeLightSpaceMatrix()
    {
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
        constexpr float zMult = 1.0f;
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

    void GameScene::renderShadowMap()
    {
        depth_shader.use();

        lightSpaceMatrix = computeLightSpaceMatrix(); //to fit in camera frustrum
        depth_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, shadow_width, shadow_height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        renderTerrain(depth_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind

        //reset viewport
        glViewport(0, 0, ctx.win_width, ctx.win_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GameScene::renderTerrain(const Shader &shader)
    {
        for (auto &it : chunks)
        {
            it.second->render(shader, camera);
        }
    }

    void GameScene::renderWorld()
    {
        glEnable(GL_CULL_FACE);
        cube_shadow.use();
        cube_shadow.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        cube_shadow.setVec3("lightDir", lightDir);
        cube_shadow.setVec3("viewPos", camera.position);
        glBindTextureUnit(1, depthMap);
        glBindTextureUnit(0, block_textures);

        renderTerrain(cube_shadow);
    }

    void GameScene::renderEntities()
    {
        for (auto &it : entities)
        {
            // std::cout << "id == " << it.second->id << std::endl;
            it.second->render(it.second->entity_shader, camera);
        }
    }

    void GameScene::update()
    {
        clock.update();

        renderShadowMap();
        // renderShadowMapQuad();
        renderWorld();
        renderEntities();
        sky.render(camera);
        renderCursorQuad();

        camera.setCameraNearFarPlanes(0.1f, 75.0f);
        frustrum_corners = getFrustumCornersWorldSpace(camera.getProjectionMatrix(), camera.getViewMatrix());
        camera.setCameraNearFarPlanes(0.1f, 1000.0f);
        request_interval += clock.delta_time;
        if (request_interval >= 1.0f/20.0f) {
            request_interval = 0;
            client.sendUpdateEntity(camera.position.x, camera.position.y, camera.position.z, camera.yaw, camera.pitch);
        }

        updateChunks();
        updateEntities();
        tq.execute();

        if (!client.data.entities.empty()) {
            // std::cout << "entities size = " << client.data.entities.size() << std::endl;
            std::cout << "entity id = " << (client.data.entities[0].id) << std::endl;
            std::cout << "entity pos x = " << (client.data.entities[0].pos.x) << std::endl;
            std::cout << "entity pos y = " << (client.data.entities[0].pos.y) << std::endl;
            std::cout << "entity pos z = " << (client.data.entities[0].pos.z) << std::endl;
            std::cout << "entity pitch = " << (client.data.entities[0].pitch) << std::endl;
            std::cout << "entity yaw = " << (client.data.entities[0].yaw) << std::endl;
        } else {
            std::cout << "empty" << std::endl;
        }
    }

    std::vector<glm::vec4> GameScene::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
    {
        const auto inv = glm::inverse(proj * view);

        std::vector<glm::vec4> frustumCorners;
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const glm::vec4 pt =
                        inv * glm::vec4(
                            2.0f * x - 1.0f,
                            2.0f * y - 1.0f,
                            2.0f * z - 1.0f,
                            1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }

        return frustumCorners;
    }

    void GameScene::updateEntities()
    {
        while (client.data.entities.size() != 0)
        {
            std::cout << "in while server pop entities" << std::endl;
            client.mtx_chunk_data.lock();
            EntityData data = client.data.entities.front();
            client.data.entities.pop_front();
            client.mtx_chunk_data.unlock();

            glm::vec3 pos = {data.pos.x, data.pos.y, data.pos.z};
            std::cout << "data pos x = " << data.pos.x << std::endl;
            std::cout << "data pos y = " << data.pos.y << std::endl;
            std::cout << "data pos z = " << data.pos.z << std::endl;
            auto it = entities.find(data.id);
            if (it != entities.end())
            {
                entities.at(data.id)->setValues(pos, data.pitch, data.yaw);
            } else
            {
                Entity *entity = new Entity(data.id, pos, data.pitch, data.yaw);
                entities[data.id] = entity;
            }
        }
    }

    void GameScene::updateChunks()
    {
        client.mtx_chunk_data.lock();
        while (client.data.chunks.size() != 0)
        {
            //lockmutex
            ChunkData chunk_data = client.data.chunks.front();
            client.data.chunks.pop_front();

            tp.enqueue([chunk_data, this] {
                Chunk *chunk = new Chunk(chunk_data.pos, chunk_data.blocktypes);
                chunk->createChunkVertices();
                tq.enqueue([chunk, this] {

                    chunk->createChunkMesh();

                    auto it = chunks.find(chunk->chunk_worldpos);
                    if (it != chunks.end())
                    {
                        auto old_chunk = it->second;
                        it->second = chunk;
                        old_chunk->deleteChunk();
                        free(old_chunk);
                    } else {
                        chunks[chunk->chunk_worldpos] = chunk;
                    }
                });
            });
        }
        client.mtx_chunk_data.unlock();
    }

    Chunk *GameScene::createChunk(const glm::ivec3 &pos, const std::vector<uint8_t>&blocktypes)
    {
        Chunk *chunk = new Chunk(pos, blocktypes);
        return chunk;
    }

    void GameScene::dda()
    {
        glm::ivec3 mapPos = glm::ivec3(floor(camera.position));
        glm::vec3 deltaDist = abs(length(camera.front) / camera.front);
        glm::ivec3 rayStep = glm::ivec3(sign(camera.front));
        glm::vec3 sideDist = (sign(camera.front) * (glm::vec3(mapPos) - camera.position) + (sign(camera.front) * glm::vec3(0.5)) + glm::vec3(0.5)) * deltaDist;
        glm::ivec3 mask;

        for (int i = 0; i < 100; i++) {
            uint8_t bt = getBlockAt(mapPos.x, mapPos.y, mapPos.z);
            if (bt != 0)
            {
                dda_data.blocktype = bt;
                dda_data.xpos = mapPos.x;
                dda_data.ypos = mapPos.y;
                dda_data.zpos = mapPos.z;
                dda_data.face = mask;
                return;
            }
            if (sideDist.x < sideDist.y) {
                if (sideDist.x < sideDist.z) {
                    sideDist.x += deltaDist.x;
                    mapPos.x += rayStep.x;
                    mask = glm::ivec3(-rayStep.x, 0, 0);
                }
                else {
                    sideDist.z += deltaDist.z;
                    mapPos.z += rayStep.z;
                    mask = glm::ivec3(0, 0, -rayStep.z);
                }
            }
            else {
                if (sideDist.y < sideDist.z) {
                    sideDist.y += deltaDist.y;
                    mapPos.y += rayStep.y;
                    mask = glm::ivec3(0, -rayStep.y, 0);
                }
                else {
                    sideDist.z += deltaDist.z;
                    mapPos.z += rayStep.z;
                    mask = glm::ivec3(0, 0, -rayStep.z);
                }
            }
        }
    }

    uint8_t GameScene::getBlockAt(int x, int y, int z)
    {
        glm::ivec3 chunk_pos = glm::floor(glm::vec3(x,y,z) / 16.0f) * 16.0f;
        glm::ivec3 local_pos = {x % 16, y % 16, z % 16};
        if (local_pos.x < 0) local_pos.x += 16;
        if (local_pos.y < 0) local_pos.y += 16;
        if (local_pos.z < 0) local_pos.z += 16;

        glm::ivec3 cube_pos = {x, y, z};

        auto it =  chunks.find(chunk_pos);
        if (it != chunks.end())
        {
            uint8_t blocktype = it->second->blocktypes[it->second->positionToIndex(local_pos)];
            return blocktype;
        } else
            return 0;
    }

    void GameScene::sceneClear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GameScene::processInput()
    {
        if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(ctx.window, true);

        if (glfwGetKey(ctx.window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.processKeyboard(FORWARD, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.processKeyboard(BACKWARD, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.processKeyboard(LEFT, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.processKeyboard(RIGHT, clock.delta_time);
        }
    }

    void GameScene::mouseCallback(GLFWwindow* window, int x, int y, int dx, int dy)
    {
        camera.processMouseMovement(dx, -dy);
    }

    void GameScene::leftClickCallback(GLFWwindow* window, int button, int action, int mods)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS)
        {
            dda();
            client.sendUpdateBlock(BlockType::Air, dda_data.xpos, dda_data.ypos, dda_data.zpos);
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_PRESS)
        {
            dda();
            client.sendUpdateBlock(BlockType::Grass, dda_data.xpos + dda_data.face.x, dda_data.ypos + dda_data.face.y, dda_data.zpos + dda_data.face.z);
        }
    }

    void GameScene::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.processMouseScroll(static_cast<float>(yoffset));
    }

    void GameScene::framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        camera.width = width;
        camera.height = height;
    }

    void GameScene::clearAllChunks()
    {
        for (auto &[key, value] : chunks)
        {
            value->deleteChunk();
            delete value;
        }
        chunks.clear();
    }
}