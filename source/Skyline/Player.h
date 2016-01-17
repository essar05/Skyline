#pragma once
#include "Entity.h"

class Player : public Entity {
  public:
  Player();
  Player(int health, int textureId, float width, float height, glm::vec2 position);
  ~Player();
};

