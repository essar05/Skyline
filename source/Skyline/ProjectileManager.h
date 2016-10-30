#pragma once

#include "Projectile.h"
#include "Player.h"
#include <vector>

class ProjectileManager {
  public:
    ProjectileManager();
    ~ProjectileManager();
    
    void add(Projectile* projectile);
    void update(float deltaTime);
    void discard(unsigned int index);
    void draw();
    bool checkCollision(Projectile* projectile, Player* player, const std::vector<unsigned int>& objects);
  
  private:
    std::vector<Projectile*> _projectiles;

};

