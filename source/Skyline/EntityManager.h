#pragma once

#include <unordered_map>
#include "Entity.h"

class EntityManager {
  public:
    EntityManager();
    ~EntityManager();
    unsigned int addEntity(Entity* entity);
    Entity* getEntity(unsigned int id);
    void deleteEntity(unsigned int id);

  private:
    std::unordered_map<unsigned int, Entity*> _entities;
    unsigned int _entityCount = 0;

};

