#pragma once
#include "SpriteRenderer.h"

class Player {
	public:
		GLuint shaderProgram;

		Player(GLFWwindow* window, SpriteRenderer* spriteRenderer);
		void update();
		void processInput();
	private:
		SpriteRenderer* spriteRenderer;
		GLFWwindow* window;
		
		float xPos = 0.0f;
		float yPos = 0.0f;
		float movementSpeed = 1.0f;
		bool movingRight = true;
};