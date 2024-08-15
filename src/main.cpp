#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameScene.hpp"

int main()
{
    mygl::Context ctx(800, 600, "MY_MINECRAFT");
    glfwSwapInterval(1); //vsync

    game::GameScene game(ctx); //idx 0

    ctx.loadScene(ctx.scenes[0]);
    ctx.run();

    glfwTerminate();
    return 0;
}