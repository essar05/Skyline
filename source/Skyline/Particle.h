#pragma once
#include <AnimationManager.h>
#include <Vertex.h>

class Particle {
  public:
    Particle(std::string type, float x, float y, float width, float height, float depth, float playbackRate);
    ~Particle();

    void update(float deltaTime);
    void draw();
    bool isDead();

  private:
    float _x;
    float _y;
    float _width;
    float _height;
    float _depth;

    Ess2D::ColorRGBA8 _color;
    Ess2D::AnimationManager* _animationManager;
};

