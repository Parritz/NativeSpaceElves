#include "SpriteRenderer.hpp"

SpriteRenderer::SpriteRenderer(GLFWwindow* window) {
	Renderer::init(VAO, VBO, EBO);
	this->window = window;
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

void SpriteRenderer::setCameraPosition(glm::vec2 cameraPosition) {
	Renderer::cameraXPos = cameraPosition.x;
	Renderer::cameraYPos = cameraPosition.y;
}

void SpriteRenderer::drawSprite(const char* texturePath, glm::vec2 position, glm::vec2 size, GLuint shader) {
	// Get the window's width and height
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);

	// Load the texture that will be drawn onto the sprite
	GLuint texture = Renderer::loadTexture(texturePath);
	glBindTexture(GL_TEXTURE_2D, texture);
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
	glUniform2f(glGetUniformLocation(shader, "position"), position.x, position.y);
	glUniform2f(glGetUniformLocation(shader, "windowSize"), width, height);
	glUniform2f(glGetUniformLocation(shader, "viewPosition"), Renderer::cameraXPos, Renderer::cameraYPos);
	glUniform1f(glGetUniformLocation(shader, "viewScale"), Renderer::cameraScale);
	
	// Draw the sprite
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderer::drawRepeatingSprite(const char* texturePath, glm::vec2 position, glm::vec2 size, glm::vec2 resolutionSize, GLuint shader) {
	// Get the window's width and height
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);

	// Load the texture that will be drawn onto the sprite
	GLuint texture = Renderer::loadTexture(texturePath);
	glm::vec2 spriteSize = size / glm::vec2(width, height);

	// Bind texture and set some wrapping and filteirng parameters so the texture can repeat
    glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Calculate the texture coordinates to maintain the correct resolution for the sprite
	float repeatX = size.x / resolutionSize.x;
	float repeatY = size.y / resolutionSize.y;
	float vertices[] = {
		// Positions				 // Texture Coordinates
		spriteSize.x,  spriteSize.y, 0.0f, repeatX, repeatY,	// Top Right
		spriteSize.x, -spriteSize.y, 0.0f, repeatX, 0.0f,		// Bottom Right
		-spriteSize.x, -spriteSize.y, 0.0f, 0.0f, 0.0f,			// Bottom Left
		-spriteSize.x,  spriteSize.y, 0.0f, 0.0f, repeatY		// Top Left
	};

	// Rebind the VAO and VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	glUseProgram(shader);
	glUniform2f(glGetUniformLocation(shader, "position"), position.x, position.y);
	glUniform2f(glGetUniformLocation(shader, "windowSize"), width, height);
	glUniform2f(glGetUniformLocation(shader, "viewPosition"), Renderer::cameraXPos, Renderer::cameraYPos);
	glUniform1f(glGetUniformLocation(shader, "viewScale"), Renderer::cameraScale);

	// Draw the sprite
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}