#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include "mygl/Context.hpp"
#include "GameScene.hpp"

int main()
{
    mygl::Context ctx;
    glfwSwapInterval(1); //vsync

    game::GameScene game(ctx); //idx 0

    ctx.loadScene(ctx.scenes[0]);
    ctx.run();

    glfwTerminate();
    return 0;
}