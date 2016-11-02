#pragma once
#include "Entity.h"
#include "ProjectileSpawner.h"

class Player : public Entity {
  public:
    Player();
    Player(int textureId, float width, float height, glm::vec2 position);
    ~Player();

    void setIsFiring(bool isFiring) { _isFiring = isFiring; }
    void update(float deltaTime);

  private:
    glm::vec2 _defaultVelocity = glm::vec2(0.0f, 0.2f);
    glm::vec2 _maxVelocity = glm::vec2(0.4f, 0.5f);
    
    float _fireRate = 0.2f;
    bool _isFiring = false;
    ProjectileSpawner _projectileSpawner;

};

