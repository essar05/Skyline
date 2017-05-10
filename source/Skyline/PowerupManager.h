#pragma once
#include <unordered_map>
#include "Powerup.h"

class PowerupManager {
public:
  PowerupManager();
  ~PowerupManager();

  void spawn(float x, float y);
  void update(float deltaTime);
  void draw();
  void cleanup();

private:
  std::vector<Powerup*> _powerups;

};

