#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager() {
}


EntityManager::~EntityManager() {
  auto it = _entities.begin();
  while(it != _entities.end()) {
    delete it->second;

    it++;
  }
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

void EntityManager::deleteEntity(unsigned int id) {
  auto it = _entities.find(id);
  if(it != _entities.end()) {
    delete it->second;
    _entities.erase(it);
  }
}