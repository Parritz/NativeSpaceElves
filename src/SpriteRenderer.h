#pragma once
#include <iostream>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

class SpriteRenderer {
    public:
        GLuint VBO, VAO, EBO;

        SpriteRenderer();
        ~SpriteRenderer();
        GLuint loadTexture(const char* texturePath);
        GLuint compileShader(const char *vertexShaderSource, const char *fragmentShaderSource);
        void drawSprite(const char* texturePath, glm::vec2 position, glm::vec2 size, GLuint shader, GLFWwindow* window);
    private:
        std::unordered_map<std::string, GLuint> textureCache;
};