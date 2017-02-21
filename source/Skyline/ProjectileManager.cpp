#include "ProjectileManager.h"
#include "Game.h"
#include <iostream>

ProjectileManager::ProjectileManager() {
}


ProjectileManager::~ProjectileManager() {
  auto it = _projectiles.begin();
  while(it != _projectiles.end()) {
      delete it->second;
      it++;
  }
  _projectiles.clear();
}

void ProjectileManager::add(Projectile* projectile) {
  _projectiles[_projectileCount++] = projectile;
  projectile->setId(_projectileCount - 1);
}

void ProjectileManager::update(float deltaTime) {
  Game* gameInstance = Game::GetInstance();
  Player* player = gameInstance->getLevel()->getPlayer();

  auto it = _projectiles.begin();
  while(it != _projectiles.end()) {
    if(!it->second->update(deltaTime)) {
      delete it->second;
      it = _projectiles.erase(it);
      continue;
    }

    it++;
  }
}

void ProjectileManager::deleteProjectile(unsigned int index, bool queued) {
  if(queued) {
    _deleteQueue.push_back(index);
    return;
  }

  auto it = _projectiles.find(index);
  if(it != _projectiles.end()) {
    delete it->second;
    _projectiles.erase(it);
  }
}

void ProjectileManager::deleteQueuedProjectiles() {
  for(unsigned int i = 0; i < _deleteQueue.size(); i++) {
    auto it = _projectiles.find(_deleteQueue[i]);
    if(it != _projectiles.end()) {
      delete it->second;
      _projectiles.erase(it);
    }
  }

  _deleteQueue.clear();
}

void ProjectileManager::draw() {
  auto it = _projectiles.begin();
  while(it != _projectiles.end()) {
    it->second->draw();
    it++;
  }
}