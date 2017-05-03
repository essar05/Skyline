#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager() {
}


EntityManager::~EntityManager() {
  deleteQueuedEntities();

  auto it = _entities.begin();
  while(it != _entities.end()) {
    delete it->second;

    it++;
  }
}

unsigned int EntityManager::addEntity(Entity* entity) {
  _entities[_entityCount++] = entity;
  entity->setId(_entityCount - 1);
  return (_entityCount - 1);
}

Entity* EntityManager::getEntity(unsigned int id) {
  auto it = _entities.find(id);
  if(it == _entities.end()) {
    return nullptr;
  }
  return it->second;
}

void EntityManager::deleteEntity(unsigned int id, bool queued) {
  if(queued) {
    _deleteQueue.push_back(id);
    return;
  }

  auto it = _entities.find(id);
  if(it != _entities.end()) {
    
    delete it->second;
    _entities.erase(it);
  }
}

void EntityManager::deleteQueuedEntities() {
  for(unsigned int i = 0; i < _deleteQueue.size(); i++) {
    auto entityIt = _entities.find(_deleteQueue[i]);
    if(entityIt != _entities.end()) {
      entityIt->second->die();
      delete entityIt->second;
      _entities.erase(entityIt);
    }
  }
  _deleteQueue.clear();
}