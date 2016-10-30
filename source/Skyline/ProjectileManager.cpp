#include "ProjectileManager.h"
#include "Game.h"
#include <iostream>

ProjectileManager::ProjectileManager() {
}


ProjectileManager::~ProjectileManager() {
  for(unsigned int i = 0; i < _projectiles.size(); i++) {
    delete _projectiles[i];
  }
}

void ProjectileManager::add(Projectile* projectile) {
  _projectiles.push_back(projectile);
}

void ProjectileManager::update(float deltaTime) {
  Game* gameInstance = Game::GetInstance();
  Player* player = gameInstance->getPlayer();
  std::vector<unsigned int> objects = gameInstance->getLevel()->getActiveObjects();

  for(unsigned int i = 0; i < _projectiles.size();) {
    if(!_projectiles[i]->update(deltaTime)) {
      discard(i);
    } else if(checkCollision(_projectiles[i], player, objects)) {
      discard(i);
    } else {
      i++;
    }
  }
}

void ProjectileManager::discard(unsigned int index) {
  delete _projectiles[index];
  _projectiles.erase(_projectiles.begin() + index);
}

bool ProjectileManager::checkCollision(Projectile* projectile, Player* player, const std::vector<unsigned int>& objects) {
  Game* gameInstance = Game::GetInstance();

  Entity* entity;
  EntityManager* entityManager = gameInstance->getEntityManager();

  if(projectile->getSource() == 1) {
    bool isColliding = false;
    for(unsigned int i = 0; i < objects.size(); i++) {
      entity = entityManager->getEntity(objects[i]);
      if(entity == nullptr) {
        continue;
      }

      bool isColliding = projectile->collidesWith(entity);
      if(isColliding) {
        entity->applyDamage(projectile->getDamage());
        return true;
      }
    }
  } else {
    return projectile->collidesWith(player);
  }

  return false;
}

void ProjectileManager::draw() {
  for(unsigned int i = 0; i < _projectiles.size(); i++) {
    _projectiles[i]->draw();
  }
}