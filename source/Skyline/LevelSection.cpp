#include "LevelSection.h"
#include "Game.h"

LevelSection::LevelSection(float width, float height) :_height(height), _width(width) {
}


LevelSection::~LevelSection() {
}

void LevelSection::draw(float x, float y) {
  Game* gameInstance = Game::GetInstance();

  Essengine::SpriteBatch* spriteBatch = gameInstance->getSpriteBatch();
  glm::vec4 position(x, y, _width, _height);
  glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
  Essengine::ColorRGBA8 color(255, 255, 255, 255);
  spriteBatch->draw(position, uv, _backgroundId, color, 0);
}