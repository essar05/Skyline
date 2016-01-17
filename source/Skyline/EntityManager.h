#pragma once

#include <unordered_map>
#include "Entity.h"

class EntityManager {
  public:
  EntityManager();
  ~EntityManager();
  void addEntity(Entity*);
  void getEntity(int id);

  private:
  std::unordered_map<int, Entity*> _entities;
  int _entityCount = 0;

};

