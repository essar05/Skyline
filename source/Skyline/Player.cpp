#include "Player.h"

Player::Player() : Player(0, 0.0f, 0.0f, glm::vec2(0.0f, 0.0f)) { }

Player::Player(int textureId, float width, float height, glm::vec2 position) : Entity(textureId, width, height, position) {
  _projectileSpawner = ProjectileSpawner(_fireRate, 10.0f, 10.0f);
  _projectileSpawner.setSource(1);
}

Player::~Player() { }

void Player::update(float deltaTime) {
  b2Vec2 velocity = _body->GetLinearVelocity();
  b2Vec2 force(0.0f, 0.0f), acceleration(0.0f, 0.0f);
  float maxSpeed = b2Max(_maxVelocity.x, _maxVelocity.y);

  if(_direction.x != 0) {
    acceleration.x = (_direction.x * _maxVelocity.x - velocity.x) * 2.0f;
  } else if(velocity.x != 0) {
    acceleration.x = (_defaultVelocity.x - velocity.x) * 2.0f;
  }

  if(_direction.y != 0) {
    if(_direction.y < 0) {
      acceleration.y = (b2Max(velocity.y - 0.05f, _direction.y * 0.1f) - velocity.y) * 2.0f;
    } else {
      acceleration.y = (b2Min(_direction.y * _maxVelocity.y, velocity.y + 0.05f) - velocity.y) * 2.0f;
    }
  } else {
    acceleration.y = (_defaultVelocity.y - velocity.y) * 2.0f;
  }

  force = _body->GetMass() * deltaTime * acceleration;

  _body->ApplyForce(force, _body->GetWorldCenter(), true);

  float currentSpeed = velocity.Length();
  if(currentSpeed > maxSpeed) {
    _body->SetLinearVelocity((maxSpeed / currentSpeed) * velocity);
  }

  _projectileSpawner.update(deltaTime, _isFiring, _position + glm::vec2(_width / 2, _height), glm::vec2(0.0f, 15.0f));
}