#pragma once

#include "Projectile.h"
#include "Player.h"
#include <vector>

class Player;

class ProjectileManager {
  public:
    ProjectileManager();
    ~ProjectileManager();
    
    void add(Projectile* projectile);
    void update(float deltaTime);
    void deleteProjectile(unsigned int index, bool queued = false);
    void deleteQueuedProjectiles();
    void draw();

  private:
    std::unordered_map<unsigned int, Projectile*> _projectiles;
    unsigned int _projectileCount = 0;
    std::vector<unsigned int> _deleteQueue;

};

