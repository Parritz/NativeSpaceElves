#include "Map.h"

Map::Map(GLFWwindow* window, SpriteRenderer* spriteRenderer) {
    this->spriteRenderer = spriteRenderer;
    this->window = window;
}

// Called every frame
void Map::update() {
    spriteRenderer->drawRepeatingSprite("assets/tiles/tile001.png", glm::vec2(0, 0), glm::vec2(200*48, 200*48), glm::vec2(48, 48), spriteRenderer->defaultShader, window);
}