#include "SpriteRenderer.h"

// Creates square/rectangle with two triangles
unsigned int squareIndices[] = {
	0, 1, 3,	// First triangle
	1, 2, 3		// Second triangle
};

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
	// Use texture cache to avoid high memory usage
	GLuint texture;
	if (this->textureCache.find(texturePath) != this->textureCache.end()) {
		texture = this->textureCache[texturePath];
		return texture;
	}

	// By default, sprites will render upside down without this set to true.
	stbi_set_flip_vertically_on_load(true); 

	int textureWidth, textureHeight;
	unsigned char* textureData = stbi_load(texturePath, &textureWidth, &textureHeight, nullptr, STBI_rgb_alpha);
	if (!textureData) {
		return -1;
	}
	
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

	this->textureCache.insert(std::make_pair(texturePath, texture));
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

void SpriteRenderer::setCameraPosition(glm::vec2 cameraPosition) {
	this->cameraXPos = cameraPosition.x;
	this->cameraYPos = cameraPosition.y;
}

void SpriteRenderer::drawSprite(const char* texturePath, glm::vec2 position, glm::vec2 size, GLuint shader, GLFWwindow* window) {
	// Get the window's width and height
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// Load the texture that will be drawn onto the sprite
	GLuint texture = loadTexture(texturePath);
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
	glUniform2f(glGetUniformLocation(shader, "viewPosition"), this->cameraXPos, this->cameraYPos);
	glUniform1f(glGetUniformLocation(shader, "viewScale"), this->cameraScale);
	
	// Draw the sprite
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderer::drawRepeatingSprite(const char* texturePath, glm::vec2 position, glm::vec2 size, glm::vec2 resolutionSize, GLuint shader, GLFWwindow* window) {
	// Get the window's width and height
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// Load the texture that will be drawn onto the sprite
	GLuint texture = loadTexture(texturePath);
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
	glUniform2f(glGetUniformLocation(shader, "viewPosition"), this->cameraXPos, this->cameraYPos);
	glUniform1f(glGetUniformLocation(shader, "viewScale"), this->cameraScale);

	// Draw the sprite
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}