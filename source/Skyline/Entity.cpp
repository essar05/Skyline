#include "Entity.h"
#include "Game.h"


Entity::Entity() {
}

Entity::Entity(int health, int textureId, float width, float height, glm::vec2 position) : 
  _health(health), _textureId(textureId), _width(width), _height(height), _position(position) {}

Entity::~Entity() {
}

void Entity::draw() {
  Game* gameInstance = Game::GetInstance();

  Essengine::SpriteBatch* spriteBatch = gameInstance->getSpriteBatch();
  glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
  Essengine::ColorRGBA8 color(255, 255, 255, 255);
  spriteBatch->draw(glm::vec4(_position, _width, _height), uv, _textureId, color, 0);
}

void Entity::move(int dir) {
  move(dir, _speed);
}

void Entity::move(int dir, float speed) {
  switch(dir) {
    //down
    case 2:
      _position += glm::vec2(0.0f, -speed);
      break;

      //left
    case 4:
      _position += glm::vec2(-speed, 0.0f);
      break;

      //right
    case 6:
      _position += glm::vec2(speed, 0.0f);
      break;

      //up
    case 8:
      _position += glm::vec2(0.0f, speed);
      break;
  }
}