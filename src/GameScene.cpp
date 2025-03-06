#include "GameScene.hpp"
#include "MYGL/Texture.hpp"
#include "memory.hpp"

#include <algorithm>
#include <string>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace game
{
    GameScene::GameScene(Context &ctx) : Scene(ctx)
    {
        storeSceneInCtx();
        if (client.asio_status == -1)
        {
            std::cout << "connexion failed: loading default scene" << std::endl;
            ctx.loadScene(ctx.default_scene);
            ctx.run();
            return;
        }

        camera.setMovementSpeed(50.0f);
        camera_ortho = CameraOrtho(glm::vec3(0.0f, 0.0f, 0.0f), ctx.win_width, ctx.win_height);
        scube_cam.yaw = 90.0f;
        scube_cam.position.y += 2.0f;
        scube_cam.pitch -= 25.0f;
        scube_cam.updateCameraVectors();

        loadTextureArray(block_textures_path, block_textures, 16, 16, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);
        loadTexture("../assets/cursor.png", cursor_img.texture);

        cursor_shader = Shader("cursor.vs", "cursor.fs");

        //show selected cube
        scube_shader = Shader("cube.vs", "cube.fs");
        scube.transform.position = glm::vec3(0.0, 0.0, 0.0);
        scube_shader.use();
        glm::ivec3 blocktex = {textures_umap.at(selected_cube).at(0), textures_umap.at(selected_cube).at(1), textures_umap.at(selected_cube).at(2)};
        scube_shader.setVec3i("BlockTextures", blocktex);

        client.startThread();
        client.sendRenderDistance(16);

        cursor_img.transform.scale.x = static_cast<float>(ctx.win_width);
        cursor_img.transform.scale.y = static_cast<float>(ctx.win_height);

        cursor_shader.use();
        cursor_shader.setInt("texture0", 0);
    }

    GameScene::~GameScene() {}

    void GameScene::storeSceneInCtx()
    {
        ctx.scenes.push_back(this);
    }

    void GameScene::openScene()
    {
        glEnable(GL_DEPTH_TEST);
        glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cursor_input_mode = GLFW_CURSOR_DISABLED;
    }

    void GameScene::closeScene()
    {
        tp.stop();
        client.stopThread();
        World::instance().clearAllChunks();
    }

    void GameScene::update()
    {
        clock.update();

        World::instance().render(camera, ctx.win_width, ctx.win_height);
        renderEntities();
        sky.render(camera);
        renderSCubeQuad();
        renderCursorQuad();

        imguiLayout();

        request_interval += clock.delta_time;
        if (request_interval >= 1.0f/20.0f) {
            request_interval = 0;
            client.sendUpdateEntity(camera.position.x, camera.position.y, camera.position.z, glm::radians(camera.yaw), glm::radians(camera.pitch));
        }
        // updateClient();
        updateChunks();
        updateEntities();
        tq.execute();
    }

    void GameScene::sceneClear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GameScene::processInput()
    {
        if (cursor_input_mode == GLFW_CURSOR_NORMAL) {
            return;
        }

        if (glfwGetKey(ctx.window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.processKeyboardMovement(FORWARD, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.processKeyboardMovement(BACKWARD, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.processKeyboardMovement(LEFT, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.processKeyboardMovement(RIGHT, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            camera.processKeyboardMovement(UP, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            camera.processKeyboardMovement(DOWN, clock.delta_time);
        }
    }

    void GameScene::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(ctx.window, true);
        }

        if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
        {
           if (cursor_input_mode == GLFW_CURSOR_DISABLED)
            {
                camera.setLock(true);
                cursor_input_mode = GLFW_CURSOR_NORMAL;
                glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                camera.setLock(false);
                cursor_input_mode = GLFW_CURSOR_DISABLED;
                glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }

        if (key == GLFW_KEY_C && action == GLFW_PRESS) {
            if (selected_cube + 1 > textures_umap.size() - 1u) {
                selected_cube = (BlockType)1;
            } else {
                selected_cube = (BlockType)(selected_cube + 1u);
            }
            selectCubeUpdate();
        }

        if (key == GLFW_KEY_X && action == GLFW_PRESS) {
            if (selected_cube - 1 < 1) {
                selected_cube = (BlockType)(textures_umap.size() - 1);
            } else {
                selected_cube = (BlockType)(selected_cube - 1);
            }
            selectCubeUpdate();
        }
    }

    void GameScene::mouseCallback(GLFWwindow* window, int x, int y, float dx, float dy)
    {
        camera.processMouseMovement(dx, -dy);
    }

    void GameScene::leftClickCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (cursor_input_mode == GLFW_CURSOR_NORMAL) {
            return;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS)
        {
            // clearAllChunks();
            dda();
            client.sendUpdateBlock(BlockType::Air, dda_data.xpos, dda_data.ypos, dda_data.zpos);
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_PRESS)
        {
            dda();
            client.sendUpdateBlock(selected_cube, dda_data.xpos + dda_data.face.x, dda_data.ypos + dda_data.face.y, dda_data.zpos + dda_data.face.z);
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

    // void GameScene::createDepthQuadTexture()
    // {
    //     glCreateFramebuffers(1, &depthMapFBO);
    //     glCreateTextures(GL_TEXTURE_2D, 1, &depthMap);

    //     glTextureParameteri(depthMap, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //     glTextureParameteri(depthMap, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //     glTextureParameteri(depthMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //     glTextureParameteri(depthMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    //     glTextureStorage2D(depthMap, 1, GL_DEPTH_COMPONENT24, shadow_width, shadow_height);

    //     glNamedFramebufferTexture(depthMapFBO, GL_DEPTH_ATTACHMENT, depthMap, 0);
    //     glNamedFramebufferDrawBuffer(depthMapFBO, GL_NONE);
    //     glNamedFramebufferReadBuffer(depthMapFBO, GL_NONE);

    //     float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //     glTextureParameterfv(depthMap, GL_TEXTURE_BORDER_COLOR, border_color);

    //     quad_depth_shader.use();
    //     quad_depth_shader.setInt("texture0", 0);
    // }

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

    void GameScene::renderSCubeQuad()
    {
        glDisable(GL_DEPTH_TEST);
        glViewport(0, 0, 200, 200);
        glBindTextureUnit(0, block_textures);
        scube.transform.rotation.y += scube_rotspeed * static_cast<float>(clock.delta_time);
        scube.render(scube_shader, scube_cam);
        glViewport(0, 0, ctx.win_width, ctx.win_height);
        glEnable(GL_DEPTH_TEST);
    }

    // void GameScene::renderShadowMapQuad()
    // {
    //     quad_depth_shader.use();
    //     quad_depth_shader.setFloat("near_plane", near_plane);
    //     quad_depth_shader.setFloat("far_plane", far_plane);

    //     glBindTextureUnit(0, depthMap);
    //     depth_quad.render(quad_depth_shader, camera_ortho);
    // }

    void GameScene::renderEntities()
    {
        for (auto &it : entities)
        {
            it.second->render(it.second->entity_shader, camera);
        }
    }

    void GameScene::updateEntities()
    {
        while (client.data.entities.size() != 0)
        {
            client.data_mtx.lock();
            EntityData data = client.data.entities.front();
            client.data.entities.pop_front();
            client.data_mtx.unlock();

            glm::vec3 pos = {data.pos.x, data.pos.y, data.pos.z};

            auto it = entities.find(data.id);
            if (it != entities.end())
            {
                entities.at(data.id)->setValues(pos, data.pitch, data.yaw);
            } else
            {
                Entity *entity = new Entity(data.id, pos, data.pitch, data.yaw, std::string(data.name));
                entities[data.id] = entity;
                std::cout << "name in entities == " << entities[data.id]->name << std::endl;
            }
        }

        while(client.data.rm_entity.size() != 0)
        {
            client.data_mtx.lock();

            int id = client.data.rm_entity.front();
            client.data.rm_entity.pop_front();

            client.data_mtx.unlock();

            entities.erase(id);
        }
    }

    void GameScene::updateChunks()
    {
        client.data_mtx.lock();
        while (client.data.chunks.size() != 0)
        {
            ChunkData chunk_data = client.data.chunks.front();
            client.data.chunks.pop_front();

            // ChunkMesh *chunk = new ChunkMesh(chunk_data.pos, chunk_data.blocktypes);
            Chunk chunk;
            chunk.worldpos = chunk_data.pos;
            chunk.blocktypes = chunk_data.blocktypes;
            ChunkMesh *chunkmesh = new ChunkMesh(chunk.worldpos, chunk.blocktypes);

            auto it = World::instance().chunkMeshes.find(chunk.worldpos);
            if (it != World::instance().chunkMeshes.end())
            {
                auto old_chunk_mesh = it->second;
                it->second = chunkmesh;
                old_chunk_mesh->deleteChunk();
                delete(old_chunk_mesh);
            } else {
                World::instance().chunks[chunk.worldpos] = chunk;
                World::instance().chunkMeshes[chunk.worldpos] = chunkmesh;
            }

            // World::instance().chunk_mtx.lock();
            // tp.enqueue([chunk, this] {
                // chunk->createChunkVertices();
                for (auto &offsetpos : neighbor_chunkpos)
                {
                    auto it = World::instance().chunkMeshes.find(chunk.worldpos + offsetpos * 16);
                    if (it != World::instance().chunkMeshes.end())
                    {
                        it->second->createChunkVertices();
                    }

                    // i->createChunkVertices();
                }
                //find chunks in all direction
                //recreate mesh
                // tq.enqueue([chunk, this] {
                    // chunk->createChunkMesh();
                    for (auto &offsetpos : neighbor_chunkpos)
                    {
                        auto it = World::instance().chunkMeshes.find(chunk.worldpos + offsetpos * 16);
                        if (it != World::instance().chunkMeshes.end())
                        {
                            it->second->createChunkMesh();
                        }
                    }
                    // neighbor_chunks.clear();
                    // World::instance().chunk_mtx.unlock();
                // });
            // });
        }
        client.data_mtx.unlock();
    }

    void GameScene::updateClient()
    {
        request_interval += clock.delta_time;
        if (request_interval >= 1.0f/20.0f) {
            request_interval = 0;
            client.sendUpdateEntity(camera.position.x, camera.position.y, camera.position.z, glm::radians(camera.yaw), glm::radians(camera.pitch));
        }
    }

    void GameScene::dda()
    {
        //https://www.shadertoy.com/view/4dX3zl

        glm::ivec3 mapPos = glm::ivec3(floor(camera.position));
        glm::vec3 deltaDist = abs(length(camera.front) / camera.front);
        glm::vec3 rayStep = glm::ivec3(sign(camera.front));
        glm::vec3 sideDist = (sign(camera.front) * (glm::vec3(mapPos) - camera.position) + (sign(camera.front) * 0.5f) + 0.5f) * deltaDist;
        glm::vec3 mask;

        for (int i = 0; i < 100; i++) {
            uint8_t bt = World::instance().getBlockAt(mapPos.x, mapPos.y, mapPos.z);
            if (bt != 0)
            {
                dda_data.blocktype = bt;
                dda_data.xpos = mapPos.x;
                dda_data.ypos = mapPos.y;
                dda_data.zpos = mapPos.z;
                dda_data.face = -mask * rayStep;
                return;
            }

            mask = glm::step(sideDist, glm::vec3(sideDist.y, sideDist.z, sideDist.x)) * glm::step(sideDist, glm::vec3(sideDist.z, sideDist.x, sideDist.y));
            sideDist += (mask) * deltaDist;
            mapPos += mask * rayStep;
        }
    }

    void GameScene::selectCubeUpdate()
    {
        scube_shader.use();
        glm::ivec3 blocktex = {textures_umap.at(selected_cube).at(0), textures_umap.at(selected_cube).at(1), textures_umap.at(selected_cube).at(2)};
        scube_shader.setVec3i("BlockTextures", blocktex);
    }

    void GameScene::imguiLayout()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        // ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Camera Speed", &camera.movement_speed, 10.0f, 300.0f);
        ImGui::Text("Chunks Loaded: %ld", World::instance().chunks.size());
        ImGui::Text("FPS: %f", (1 / clock.delta_time));
        ImGui::Text("///LIGHTDIR///");
        ImGui::SliderFloat("x:", &World::instance().shadowmap.lightdir_x, -100.0f, 100.0f);
        ImGui::SliderFloat("y:", &World::instance().shadowmap.lightdir_y, -100.0f, 100.0f);
        ImGui::SliderFloat("z:", &World::instance().shadowmap.lightdir_z, -100.0f, 100.0f);
        ImGui::NewLine(); ImGui::Separator();
        ImGui::Checkbox("Fog", &World::instance().fog_display);
        ImGui::SliderFloat("Fog Min Distance", &World::instance().fog_mindist, 0.0f, 800.0f);
        ImGui::SliderFloat("Fog Max Distance", &World::instance().fog_maxdist, 0.0f, 800.0f);
        ImGui::Text("PLAYERS CONNECTED :");
        if (entities.size() > 0) {
            // std::cout << "entitiy name = " << entities[0]->name << std::endl;
            for (auto &it : entities)
            {
                ImGui::Text("%s\n", it.second->name.c_str());
            }
        } else {
            ImGui::Text("no other player connected");
        }
        ImGui::Text("CHAT:");
        ImGui::InputText("##chat", input_chat, 4096);
        ImGui::SameLine();
        if (ImGui::Button("send"))
        {
            client.sendTextChat(input_chat);
            std::memset(input_chat, 0, 4096);
        };
        ImGui::BeginChild("chatbox", ImVec2(ImGui::GetContentRegionAvail()), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto &c : client.data.chat_history) {
            ImGui::TextWrapped("%s", c.text.c_str());
            // ImGui::Spacing();
        }

        ImGui::EndChild();

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}