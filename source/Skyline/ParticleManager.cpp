#include "ParticleManager.h"
#include <iostream>

ParticleManager::ParticleManager() {
}

ParticleManager::~ParticleManager() {
  for (int i = 0; i < _particles.size(); i++) {
    delete _particles[i];
  }
}

void ParticleManager::spawn(std::string type, float x, float y, float width, float height, float depth, float playbackRate) {
  Particle* particle = new Particle(type, x, y, width, height, depth, playbackRate);

  _particles.push_back(particle);
}

void ParticleManager::update(float deltaTime) {
  for (int i = 0; i < _particles.size(); i++) {
    if (_particles[i]->isDead()) {
      delete _particles[i];
      _particles.erase(_particles.begin() + i);
      continue;
    }

    _particles[i]->update(deltaTime);
  }
}

void ParticleManager::draw() {
  for (int i = 0; i < _particles.size(); i++) {
    _particles[i]->draw();
  }
}