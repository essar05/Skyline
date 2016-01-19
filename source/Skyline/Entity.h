#pragma once

#include <glm\glm.hpp>

class Entity {
  public:
  Entity();
  Entity(int health, int textureId, float width, float height, glm::vec2 position);
  ~Entity();

  void setSpeed(float speed) { _speed = speed; }
  glm::vec2 getPosition();
  float getWidth();
  float getHeight();

  void draw();
  
  void move(int dir);
  void move(int dir, float speed);

  protected:
  int _health;
  float _width;
  float _height;
  glm::vec2 _position;
  float _isAlive = false;
  float _speed = 1.0f;

  int _textureId;


};

