#include "Player.h"

Player::Player(GLFWwindow* window, SpriteRenderer* spriteRenderer) {
	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoord;
		out vec2 TexCoord;
		uniform vec2 playerPos; // Player position in pixels
		uniform vec2 windowSize;

		void main() {
			// Convert from pixel coordinates to normalized device coordinates (-1 to 1) that OpenGL uses
			vec2 normalizedPlayerPos = (playerPos / vec2(windowSize.x, -windowSize.y)) * 2.0 - 1.0;
			
			// Transform position using pixel-based player position
			gl_Position = vec4(aPos.x + normalizedPlayerPos.x, aPos.y - normalizedPlayerPos.y, aPos.z, 1.0);
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
	
	this->window = window;
	this->spriteRenderer = spriteRenderer;
	this->shaderProgram = spriteRenderer->compileShader(vertexShaderSource, fragmentShaderSource);
}

// Called every frame
void Player::update() {
	this->processInput();

	// Render player sprite and shadow
	const char* jetbikeAsset = movingRight ? "assets/sprites/jetbike.png" : "assets/sprites/jetbike_l.png";
	this->spriteRenderer->drawSprite("assets/sprites/shadow.png", glm::vec2(this->xPos, this->yPos - 24), glm::vec2(48, 48), this->shaderProgram, this->window);	
	this->spriteRenderer->drawSprite(jetbikeAsset, glm::vec2(this->xPos, this->yPos), glm::vec2(78, 48), this->shaderProgram, this->window);

	// Render crosshair
	double cursorXPos, cursorYPos;
	glfwGetCursorPos(this->window, &cursorXPos, &cursorYPos);

	// Subtract on X and add for Y because cursorYPos starts as a negative while our player Y position is based on positives
	// Calculate the cursors distance from the player
	double angleX = cursorXPos - this->xPos;
	double angleY = cursorYPos + this->yPos; 
	double distance = sqrt(angleX * angleX + angleY * angleY);
	double crosshairDistance = 75.0;

	// Avoid division by 0 errors (haven't had it happen but just in case)
	if (distance > 0.0) {
		// Scale the crosshairX and crosshairY by the scaleFactor to limit how far the crosshair can go
		double scaleFactor = crosshairDistance / distance;
		double crosshairX = this->xPos + angleX * scaleFactor;
		double crosshairY = this->yPos + -angleY * scaleFactor;

		this->spriteRenderer->drawSprite("assets/sprites/xhair.png", glm::vec2(crosshairX, crosshairY), glm::vec2(16, 16), this->shaderProgram, this->window);
	}
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
		this->movingRight = false;
	}

	// Right
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		this->xPos += this->movementSpeed;
		this->movingRight = true;
	}
}