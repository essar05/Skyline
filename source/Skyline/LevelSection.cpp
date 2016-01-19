#include "LevelSection.h"
#include "Game.h"
#include <algorithm>
#include "EntityManager.h"

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

  Essengine::Camera2D* camera = gameInstance->getMainCamera();
  EntityManager* entityManager = gameInstance->getEntityManager();
  Entity* entity;

  for(unsigned int k = _renderStartIt; k < _objects.size(); k++) {
    entity = entityManager->getEntity(_objects[k]);

    if(entity->getPosition()[1] + entity->getHeight() < camera->getScaledPosition()[1]) {
      _renderStartIt = k + 1;
    }
    if(entity->getPosition()[1] > camera->getScaledPosition()[1] + camera->getViewportSize()[1] + 10) {
      break;
    }

    entity->draw();
  }
}

void LevelSection::addObject(unsigned int objectId) {
  _objects.push_back(objectId);
  std::stable_sort(_objects.begin(), _objects.end(), compareObjectYPosition);
}

bool LevelSection::compareObjectYPosition(unsigned int a, unsigned int b) {
  Game* gameInstance = Game::GetInstance();
  EntityManager* entityManager = gameInstance->getEntityManager();
  return (entityManager->getEntity(a)->getPosition()[1] < entityManager->getEntity(b)->getPosition()[1]);
}