#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer() {
	// Generate and bind Vertex Array Object (VAO)
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// Generate and bind Vertex Buffer Object (VBO)
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	// Generate and bind Element Buffer Object (EBO)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	// Set up vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

GLuint SpriteRenderer::loadTexture(const char* texturePath) {
	stbi_set_flip_vertically_on_load(true); // The default is that the sprite renders upside down

	int textureWidth, textureHeight;
	unsigned char* textureData = stbi_load(texturePath, &textureWidth, &textureHeight, nullptr, STBI_rgb_alpha);
	if (!textureData) {
		return -1;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Enable the alpha channel allowing transparency in textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load texture data then free the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);

	return texture;
}

GLuint SpriteRenderer::compileShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
	// Compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Create shader program to be used for rendering
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Cleanup and return shaderProgram
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

GLuint SpriteRenderer::drawSprite(const char* texturePath, GLuint shader) {
	// Load the texture that will be drawn onto the sprite
	GLuint texture = this->loadTexture(texturePath);

	// Creates square/rectangle with two triangles
	unsigned int indices[] = {
		0, 1, 3,  // First triangle
		1, 2, 3   // Second triangle
	};

	float spriteSize = 0.05f;
	float vertices[] = {
		// Positions                  // Texture Coordinates
		spriteSize,  spriteSize, 0.0f, 1.0f, 1.0f,  // Top Right
		spriteSize, -spriteSize, 0.0f, 1.0f, 0.0f,  // Bottom Right
		-spriteSize, -spriteSize, 0.0f, 0.0f, 0.0f,  // Bottom Left
		-spriteSize,  spriteSize, 0.0f, 0.0f, 1.0f   // Top Left
	};

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// glBindVertexArray(this->VAO);
	// glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	// // Load vertex data into VBO then draw the sprite
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glBindVertexArray(this->VAO);
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// // Unbind VAO and VBO
	// glBindVertexArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	return texture;
}

void SpriteRenderer::destroySprite(GLuint sprite) {
	glDeleteTextures(1, &sprite);
}