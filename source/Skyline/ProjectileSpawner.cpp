#include "ProjectileSpawner.h"
#include "Game.h"
#include <iostream>

ProjectileSpawner::ProjectileSpawner() {

}

ProjectileSpawner::ProjectileSpawner(float fireRate, glm::vec2 size, float projectileDamage) {
  _fireRate = fireRate;
  _projectileWidth = size.x;
  _projectileHeight = size.y;
  _projectileDamage = projectileDamage;
}

ProjectileSpawner::~ProjectileSpawner() {

}

int ProjectileSpawner::update(float deltaTime, bool isFiring, const glm::vec2& position, const glm::vec2& velocity) {
  _fireRateCounter += _fireRate * deltaTime;
  int projectileCount = 0;

  if (isFiring) {
    projectileCount = (int)floor(_fireRateCounter);

    if (projectileCount > 0) {
      spawn(projectileCount, position, velocity);
    }

    _fireRateCounter -= projectileCount;
  }

  return projectileCount;
}

void ProjectileSpawner::spawn(int projectileCount, const glm::vec2& position, const glm::vec2& velocity) {
  Game* gameInstance = Game::GetInstance();

  for(int i = 0; i < projectileCount; i++) {
    Projectile* projectile = new Projectile(gameInstance->getTextureCache()->getTexture("Textures/tesx.png")._id, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                            _projectileWidth, _projectileHeight, glm::vec2(position.x, position.y), _source, _projectileDamage);
    projectile->createB2Data();
    projectile->setVelocity(velocity);
    projectile->spawn();
    gameInstance->getProjectileManager()->add(projectile);
  }
}


