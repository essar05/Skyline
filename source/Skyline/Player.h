#pragma once
#include "Entity.h"
#include "ProjectileSpawner.h"

class Player : public Entity {
  public:
    Player();
    Player(int textureId, float width, float height, glm::vec2 position);
    ~Player();

    void setIsFiring(bool isFiring) { _isFiring = isFiring; }
    bool update(float deltaTime);

  private:
    glm::vec2 _defaultVelocity = glm::vec2(0.0f, 10.0f);
    glm::vec2 _maxVelocity = glm::vec2(15.0f, 20.0f);
    
    float _fireRate = 8.0f; //
    bool _isFiring = false;
    ProjectileSpawner _projectileSpawner;

};

