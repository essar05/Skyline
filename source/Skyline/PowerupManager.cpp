#include "PowerupManager.h"
#include <iostream>

PowerupManager::PowerupManager() {
}

PowerupManager::~PowerupManager() {
  for(int i = 0; i < _powerups.size(); i++) {
    delete _powerups[i];
  }
}

void PowerupManager::spawn(float x, float y) {
  float randomFloat = (float)(rand() % 1001) / 10;
  std::string type = "";

  //HP          [0 - 15)          15%
  //SCORE       [15 - 40)         25%
  //DMG         [40 - 55)         15%
  //NOTHING     [55 - 100)        50%

  if(randomFloat >= 0.0f && randomFloat < 15.0f) {
    type = "hp";
  } else if(randomFloat >= 15.0f && randomFloat < 40.0f) {
    type = "score";
  } else if(randomFloat >= 40.0f && randomFloat < 55.0f) {
    type = "dmg";
  } else {
    return;
  }

  Powerup* powerup = new Powerup(type, 2.5f, 2.5f, glm::vec2(x, y));
  powerup->createB2Data();
  powerup->spawn();

  _powerups.push_back(powerup);
}

void PowerupManager::update(float deltaTime) {
  for(int i = 0; i < _powerups.size(); i++) {
    if(!_powerups[i]->inViewport() || _powerups[i]->isDead()) {
      delete _powerups[i];
      _powerups.erase(_powerups.begin() + i);
      continue;
    }

    _powerups[i]->update(deltaTime);
  }
}

void PowerupManager::draw() {
  for(int i = 0; i < _powerups.size(); i++) {
    _powerups[i]->draw();
  }
}

void PowerupManager::cleanup() {
  for(int i = 0; i < _powerups.size(); i++) {
    delete _powerups[i];
    _powerups.erase(_powerups.begin() + i);
  }
  _powerups.clear();
}