#define STB_IMAGE_IMPLEMENTATION
#include "SpriteRenderer.hpp"
#include "Player.hpp"
#include "Map.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(1280, 800, "Space Elves", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	SpriteRenderer* spriteRenderer = new SpriteRenderer();
	Player* player = new Player(window, spriteRenderer);
	Map* map = new Map(window, spriteRenderer);
	
	while (!glfwWindowShouldClose(window)) {
		// Clear screen
		glClearColor(0.2f, 0.2f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Handle tick events
		map->update();
		player->update();
		
		// Check for events in queue, then swap window buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Cleanup
	glfwTerminate();
	return 0;
}
