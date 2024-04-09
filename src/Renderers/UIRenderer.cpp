#include "UIRenderer.hpp"

UIRenderer::UIRenderer(GLFWwindow* window) {
	Renderer::init(VAO, VBO, EBO);
	this->window = window;
}

void UIRenderer::drawFrame(glm::vec2 position, glm::vec2 size, GLuint shader) {
	// Get the window's width and height
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);

	// Load the texture that will be drawn onto the sprite
	glm::vec2 spriteSize = size / glm::vec2(width, height);

	float vertices[] = {
		// Positions				// Texture Coordinates
		spriteSize.x,  spriteSize.y, 0.0f, 1.0f, 1.0f,		// Top Right
		spriteSize.x, -spriteSize.y, 0.0f, 1.0f, 0.0f,		// Bottom Right
		-spriteSize.x, -spriteSize.y, 0.0f, 0.0f, 0.0f,		// Bottom Left
		-spriteSize.x,  spriteSize.y, 0.0f, 0.0f, 1.0f		// Top Left
	};

	// Rebind the VAO and VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	glUseProgram(shader);
	glUniform3f(glGetUniformLocation(shader, "color"), 0.0f, 0.0f, 0.0f);
	glUniform2f(glGetUniformLocation(shader, "position"), 0.5f, 0.5f);
	glUniform2f(glGetUniformLocation(shader, "windowSize"), width, height);

	// Draw the sprite
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UIRenderer::update() {
	this->drawFrame(glm::vec2(0,0), glm::vec2(32,32), defaultShader);
}