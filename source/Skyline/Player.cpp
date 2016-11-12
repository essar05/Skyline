#include "Player.h"
#include "Utils.h"
#include <iostream>

Player::Player() : Player(0, 0.0f, 0.0f, glm::vec2(0.0f, 0.0f)) { }

Player::Player(int textureId, float width, float height, glm::vec2 position) : Entity(textureId, width, height, position) {
  _projectileSpawner = ProjectileSpawner(8.0f, glm::vec2(0.2f, 0.5f), 10.0f);
  _projectileSpawner.setSource(1);
}

Player::~Player() { }

bool Player::update(float deltaTime) {
  b2Vec2 velocity = _body->GetLinearVelocity();
  b2Vec2 force(0.0f, 0.0f), acceleration(0.0f, 0.0f), desiredVelocity(0.0f, 0.0f);
  float maxSpeed = b2Max(_maxVelocity.x, _maxVelocity.y);

  if(_direction.x != 0) {
    acceleration.x = (_direction.x * _maxVelocity.x - velocity.x);
    //desiredVelocity.x = _direction.x * 15.0f;
  } else if(velocity.x != 0) {
    acceleration.x = (_defaultVelocity.x - velocity.x);
  }

  if(_direction.y != 0) {
    //desiredVelocity.y = _direction.y * 15.0f;
    if(_direction.y < 0) {
      acceleration.y = (b2Max(velocity.y - 3.5f, _direction.y * 5.0f) - velocity.y);
    } else {
      acceleration.y = (b2Min(_direction.y * _maxVelocity.y, velocity.y + 3.5f) - velocity.y);
    }
  } else {
    acceleration.y = (_defaultVelocity.y - velocity.y);
  }

  force = _body->GetMass() * acceleration;

  _body->ApplyLinearImpulse(force, _body->GetWorldCenter(), true);
  //_body->SetLinearVelocity(desiredVelocity);

  float currentSpeed = velocity.Length();
  if(currentSpeed > maxSpeed) {
    //_body->SetLinearVelocity((maxSpeed / currentSpeed) * velocity);
  }

  glm::vec2 position = Utils::toVec2(_body->GetPosition());
  //test
  _projectileSpawner.update(deltaTime, _isFiring, position + glm::vec2(0.0f, _height / 2 + _projectileSpawner.getProjectileHeight()), glm::vec2(0.0f, 50.0f));

  return true;
}