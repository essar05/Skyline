#pragma once
#include <unordered_map>
#include "Particle.h"

class ParticleManager {
  public:
    ParticleManager();
    ~ParticleManager();

    void spawn(std::string type, float x, float y, float width, float height, float depth, float playbackRate);
    void update(float deltaTime);
    void draw();

  private:
    std::vector<Particle*> _particles;

};

