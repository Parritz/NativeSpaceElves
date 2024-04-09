#pragma once
#include <iostream>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

static const unsigned int squareIndices[] = {
	0, 1, 3,    // First triangle
	1, 2, 3     // Second triangle
};
static std::unordered_map<std::string, GLuint> textureCache;

class Renderer {
public:
	inline static float cameraXPos = 0;
	inline static float cameraYPos = 0;
	inline static float cameraScale = 1;

	static void init(GLuint& VAO, GLuint& VBO, GLuint& EBO);
	static GLuint loadTexture(const char* texturePath);
	static GLuint compileShader(const char* vertexShaderSource, const char* fragmentShaderSource);
};