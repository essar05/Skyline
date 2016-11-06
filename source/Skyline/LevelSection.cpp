#include "LevelSection.h"
#include "Game.h"
#include <algorithm>
#include "EntityManager.h"

LevelSection::LevelSection(float width, float height) {
  _game = Game::GetInstance();
  _width = _game->getMainCamera()->getWorldScalar(width);
  _height = _game->getMainCamera()->getWorldScalar(height);
}

LevelSection::~LevelSection() {
  EntityManager* entityManager = _game->getEntityManager();
  Level* level = _game->getLevel();

  if(entityManager != nullptr) {
    for(unsigned int k = 0; k < _objects.size(); k++) {
      unsigned int objectId = _objects[k];
      entityManager->deleteEntity(objectId);
      _objects.erase(_objects.begin() + k);
      level->discardActiveObject();
    }
  }
}

void LevelSection::update() {
  EntityManager* entityManager = _game->getEntityManager();
  Level* level = _game->getLevel();
  Entity* entity;

  for(unsigned int k = 0; k < _objects.size();) {
    entity = entityManager->getEntity(_objects[k]);

    //if this object had been spawned but went out of viewpot OR if it's health is 0 then we just discard it.
    if((entity->isSpawned() && !entity->inViewport()) || entity->getHealth() <= 0) {
      entityManager->deleteEntity(_objects[k]);
      _objects.erase(_objects.begin() + k);
      level->discardActiveObject();
      continue;
    }

    //stop once we reach the first object that is not in viewport and it's not spawned. it means it is inactive atm because we haven't gotten there yet, so no reason to update any further since objects are sorted by their y position.
    if(!entity->isSpawned() && !entity->inViewport()) {
      break;
    }

    //this entity IS in the viewport but it hasn't been spanwed it. so let's spawn it and set it as active.
    if(!entity->isSpawned()) {
      entity->spawn();
      level->addActiveObject(_objects[k]);
    }

    k++;
  }
}

void LevelSection::draw(float x, float y) {
  Essengine::SpriteBatch* spriteBatch = _game->getSpriteBatch();

  glm::vec4 position(x, y, _width, _height);
  glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
  Essengine::ColorRGBA8 color(255, 255, 255, 255);
  spriteBatch->draw(position, uv, _backgroundId, color, 0);

  Essengine::Camera2D* camera = _game->getMainCamera();
  EntityManager* entityManager = _game->getEntityManager();
  Entity* entity;

  for(unsigned int k = 0; k < _objects.size();) {
    entity = entityManager->getEntity(_objects[k]);

    if(!entity->isSpawned() && !entity->inViewport()) {
      break;
    }

    entity->draw();

    k++;
  }
}

void LevelSection::addObject(unsigned int objectId) {
  _objects.push_back(objectId);
  std::stable_sort(_objects.begin(), _objects.end(), compareObjectYPosition);
}

bool LevelSection::compareObjectYPosition(unsigned int a, unsigned int b) {
  EntityManager* entityManager = Game::GetInstance()->getEntityManager();
  return (entityManager->getEntity(a)->getPosition().y < entityManager->getEntity(b)->getPosition().y);
}