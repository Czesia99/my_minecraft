#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameScene.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int main()
{

    mygl::Context ctx(1920, 1080, "MY_MINECRAFT");
    glfwSwapInterval(1); //vsync

    // IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(ctx.window, true);
    ImGui_ImplOpenGL3_Init();
    // IMGUI END

    game::GameScene game(ctx); //idx 0

    ctx.loadScene(ctx.scenes[0]);
    ctx.run();

    glfwTerminate();
    return 0;
}