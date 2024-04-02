#include "Player.hpp"

Player::Player(GLFWwindow* window, SpriteRenderer* spriteRenderer) {
	this->window = window;
	this->spriteRenderer = spriteRenderer;
}

// Called every frame
void Player::update() {
	this->processInput();

	// Render player sprite and shadow
	const char* jetbikeAsset = movingRight ? "assets/sprites/jetbike.png" : "assets/sprites/jetbike_l.png";
	this->spriteRenderer->drawSprite("assets/sprites/shadow.png", glm::vec2(this->xPos, this->yPos - 24), glm::vec2(48, 48), spriteRenderer->defaultShader, this->window);	
	this->spriteRenderer->drawSprite(jetbikeAsset, glm::vec2(this->xPos, this->yPos), glm::vec2(78, 48), spriteRenderer->defaultShader, this->window);
	
	glm::vec2 cameraPosition = glm::vec2(this->xPos, this->yPos);
	spriteRenderer->setCameraPosition(cameraPosition);

	// Render crosshair
	double cursorXPos, cursorYPos;
	glfwGetCursorPos(this->window, &cursorXPos, &cursorYPos);

	// Adjust cursor position based on the camera
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	cursorXPos += cameraPosition.x - (width / 2);
	cursorYPos += cameraPosition.y - (height / 2);

	// Calculate the cursor's distance from the player
	double angleX = cursorXPos - this->xPos;
	double angleY = cursorYPos - this->yPos; 
	double distance = sqrt(angleX * angleX + angleY * angleY);
	double crosshairDistance = 75.0;

	// Avoid division by 0 errors (haven't had it happen but just in case)
	if (distance > 0.0) {
		// Scale the crosshairX and crosshairY by the scaleFactor to limit how far the crosshair can go
		double scaleFactor = crosshairDistance / distance;
		double crosshairX = this->xPos + angleX * scaleFactor;
		double crosshairY = this->yPos - angleY * scaleFactor;

		this->spriteRenderer->drawSprite("assets/sprites/xhair.png", glm::vec2(crosshairX, crosshairY), glm::vec2(16, 16), spriteRenderer->defaultShader, this->window);
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