#include "Player.h"

Player::Player(GLFWwindow* window, SpriteRenderer* spriteRenderer) {
	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoord;
		out vec2 TexCoord;
		uniform mat4 model;  // Model matrix
		uniform vec2 playerPos;  // Player position
		void main() {
			gl_Position = model * vec4(aPos.x + playerPos.x, aPos.y + playerPos.y, aPos.z, 1.0);
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
	})";

	GLuint shaderProgram = spriteRenderer->compileShader(vertexShaderSource, fragmentShaderSource);
	GLuint playerSprite = spriteRenderer->drawSprite("assets/jetbike.png", shaderProgram);	
	
	this->window = window;
	this->spriteRenderer = spriteRenderer;
	this->shaderProgram = shaderProgram;
}

// Called every frame
void Player::update() {
	this->processInput();
	this->move(this->xPos, this->yPos);
}

// Moves player to the designated x and y coordinates
void Player::move(float x, float y) {
	std::cout << "Position: X: " << x << " | Y: " << y << std::endl;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, 0.0f));

	// Use shader and set position
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform2f(glGetUniformLocation(shaderProgram, "playerPos"), x, y);
}

void Player::processInput() {
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// Forward
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		this->yPos += this->movementSpeed;
	}

	// Backwards
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		this->yPos -= this->movementSpeed;
	}

	// Left
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		this->xPos -= this->movementSpeed;
	}

	// Right
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		this->xPos += this->movementSpeed;
	}
}