#pragma once
#include "SpriteRenderer.hpp"

class Map {
    public:
        Map(GLFWwindow* window, SpriteRenderer* spriteRenderer);
        void update();
    private:
        SpriteRenderer* spriteRenderer;
        GLFWwindow* window;
        char* currentMap;
};