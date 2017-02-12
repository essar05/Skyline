#pragma once

#include <math.h>
#include <glm\glm.hpp>

class ProjectileSpawner {
  public:
    ProjectileSpawner();
    ProjectileSpawner(float fireRate, glm::vec2 size, float projectileDamage);
    ~ProjectileSpawner();

    void setFireRate(float fireRate) { _fireRate = fireRate; }
    void setSource(int source) { _source = source; }
    int update(float deltaTime, bool isFiring, const glm::vec2& position, const glm::vec2& velocity);
    void spawn(int projectileCount, const glm::vec2& position, const glm::vec2& velocity);

    float getProjectileWidth() { return _projectileWidth; }
    float getProjectileHeight() { return _projectileHeight; }

  private:
    float _fireRate = 0.3f;
    float _fireRateCounter = 0.0f;
    float _projectileWidth = 5.0f;
    float _projectileHeight = 5.0f;
    float _projectileDamage = 10.0f;
    int _source = 2;


};

