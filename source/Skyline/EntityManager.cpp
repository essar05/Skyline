#include "EntityManager.h"


EntityManager::EntityManager() {
}


EntityManager::~EntityManager() {
}

unsigned int EntityManager::addEntity(Entity* entity) {
  _entities[_entityCount++] = entity;
  return (_entityCount - 1);
}

Entity* EntityManager::getEntity(unsigned int id) {
  auto it = _entities.find(id);
  if(it == _entities.end()) {
    return nullptr;
  }
  return it->second;
}