#pragma once
#include "Renderer.hpp"

class SpriteRenderer {
    public:
        GLuint VBO, VAO, EBO;

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
				
				vec4 viewPos = vec4((aPos.x + normalizedPosition.x) * viewScale - normalizedCamera.x,
									(aPos.y - normalizedPosition.y) * viewScale + normalizedCamera.y,
									aPos.z, 1.0);

				// Transform position using pixel-based position
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
		
        GLuint defaultShader = Renderer::compileShader(this->vertexShaderSource, this->fragmentShaderSource);

        SpriteRenderer(GLFWwindow* window);
        ~SpriteRenderer();
		void setCameraPosition(glm::vec2 cameraPosition);
        void drawSprite(const char* texturePath, glm::vec2 position, glm::vec2 size, GLuint shader);
        void drawRepeatingSprite(const char* texturePath, glm::vec2 position, glm::vec2 size, glm::vec2 resolutionSize, GLuint shader);

    private:
		GLFWwindow* window;
};