#pragma once
#include <iostream>
#include <vector>
#include <tuple>
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
		float cameraXPos = 0;
		float cameraYPos = 0;
		float cameraScale = 1;

        // Default shader sources to use
        const char* vertexShaderSource = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec2 aTexCoord;
			out vec2 TexCoord;
			uniform vec2 position;
			uniform vec2 windowSize;
			uniform vec2 viewPosition;
			uniform float viewScale;

			void main() {
				// Convert from pixel coordinates to normalized device coordinates (-1 to 1) that OpenGL uses
				vec2 normalizedPosition = (position / vec2(windowSize.x, -windowSize.y)) * 2.0 - 1.0;
				vec2 normalizedCamera = (viewPosition / vec2(windowSize.x, -windowSize.y)) * 2.0 - 1.0;

				// Apply the view matrix only to the x and y components
				vec4 viewPos = vec4((aPos.x + normalizedPosition.x) * viewScale - normalizedCamera.x,
									(aPos.y - normalizedPosition.y) * viewScale + normalizedCamera.y,
									aPos.z, 1.0);

				// Transform position using pixel-based player position
				gl_Position = viewPos;
				TexCoord = aTexCoord;
			}
		)";

		const char* fragmentShaderSource = R"(
			#version 330 core
			in vec2 TexCoord;
			out vec4 FragColor;
			uniform sampler2D ourTexture;
			void main() {
				vec4 texColor = texture(ourTexture, TexCoord);
				FragColor = texColor;
			}
		)";
		
        GLuint defaultShader = this->compileShader(this->vertexShaderSource, this->fragmentShaderSource);

        SpriteRenderer();
        ~SpriteRenderer();
        GLuint loadTexture(const char* texturePath);
        GLuint compileShader(const char *vertexShaderSource, const char *fragmentShaderSource);
		void setCameraPosition(glm::vec2 cameraPosition);
        void drawSprite(const char* texturePath, glm::vec2 position, glm::vec2 size, GLuint shader, GLFWwindow* window);
        void drawRepeatingSprite(const char* texturePath, glm::vec2 position, glm::vec2 size, glm::vec2 resolutionSize, GLuint shader, GLFWwindow* window);
    private:
        std::unordered_map<std::string, GLuint> textureCache;
};