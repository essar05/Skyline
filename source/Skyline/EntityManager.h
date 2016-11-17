#pragma once

#include <unordered_map>
#include "Entity.h"

class EntityManager {
  public:
    EntityManager();
    ~EntityManager();
    unsigned int addEntity(Entity* entity);
    Entity* getEntity(unsigned int id);
    void deleteEntity(unsigned int id, bool scheduled = false);

    void deleteQueuedEntities();

  private:
    std::unordered_map<unsigned int, Entity*> _entities;
    std::vector<unsigned int> _deleteQueue;
    unsigned int _entityCount = 0;

};

