#include "ProjectileSpawner.h"
#include "Game.h"
#include <iostream>

ProjectileSpawner::ProjectileSpawner() {

}

ProjectileSpawner::ProjectileSpawner(float fireRate, float projectileWidth, float projectileDamage) {
  _fireRate = fireRate;
  _projectileWidth = projectileWidth;
  _projectileDamage = projectileDamage;
}

ProjectileSpawner::~ProjectileSpawner() {

}

void ProjectileSpawner::update(float deltaTime, bool isFiring, const glm::vec2& position, const glm::vec2& velocity) {
  _fireRateCounter += _fireRate * deltaTime;
  int projectileCount = (int) floor(_fireRateCounter);

  if(isFiring && projectileCount > 0) {  
    spawn(projectileCount, position, velocity);
  }

  _fireRateCounter -= projectileCount;
}

void ProjectileSpawner::spawn(int projectileCount, const glm::vec2& position, const glm::vec2& velocity) {
  Game* gameInstance = Game::GetInstance();

  for(int i = 0; i < projectileCount; i++) {
    Projectile* projectile = new Projectile(gameInstance->getTextureCache()->getTexture("Textures/tesx.png")._id, 
                                            _projectileWidth, _projectileWidth, glm::vec2(position.x - _projectileWidth / 2, position.y - _projectileWidth), _source, _projectileDamage);
    projectile->setVelocity(velocity);
    projectile->spawn();
    gameInstance->getProjectileManager()->add(projectile);
  }
}


