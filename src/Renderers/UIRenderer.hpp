#pragma once
#include "iostream"
#include "Renderer.hpp"

class UIRenderer {
    public:
        UIRenderer(GLFWwindow* window);
        void drawFrame(glm::vec2 position, glm::vec2 size, GLuint shader);
        void update();

        const char* vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;
            out vec2 TexCoord;
            uniform vec2 position;  // Player position
            uniform vec2 windowSize; // The size of the window

            void main() {
                gl_Position = vec4((aPos.x - 1.0) + position.x * 2, (aPos.y + 1.0) - position.y * 2, aPos.z, 1.0);
                TexCoord = aTexCoord;
            }
        )";


		const char* fragmentShaderSource = R"(
			#version 330 core

			out vec4 FragColor;
			uniform vec3 color;

			void main() {
				FragColor = vec4(color, 1.0);
			}
		)";

		GLuint defaultShader = Renderer::compileShader(this->vertexShaderSource, this->fragmentShaderSource);
    private:
        GLuint VAO, VBO, EBO;
        GLFWwindow* window;
};