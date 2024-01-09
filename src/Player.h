#pragma once
#include "SpriteRenderer.h"

class Player {
	public:
		GLuint shaderProgram;
	
		Player(GLFWwindow* window, SpriteRenderer* spriteRenderer);
		void update();
		void move(float x, float y);
		void processInput();
	private:
		SpriteRenderer* spriteRenderer;
		GLFWwindow* window;
		
		float xPos = 0.0f;
		float yPos = 0.0f;
		float movementSpeed = 0.001f;
};