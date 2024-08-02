#pragma once

#include <GLFW/glfw3.h>

namespace mygl 
{
    class Clock
    {
        public:
            double current_time;
            float delta_time;
            float last_frame;

            Clock() { update(); };

            void update() {
                current_time = glfwGetTime();
                delta_time = current_time - last_frame;
                last_frame = current_time;
            };

            void reset() {
                current_time = glfwGetTime();
                delta_time = current_time - last_frame;
                last_frame = current_time;
            }
    };
}