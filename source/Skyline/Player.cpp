#include "Player.h"
#include "Utils.h"

Player::Player() : Player(0, 0.0f, 0.0f, glm::vec2(0.0f, 0.0f)) { }

Player::Player(int textureId, float width, float height, glm::vec2 position) : Entity(textureId, width, height, position) {
  _projectileSpawner = ProjectileSpawner(_fireRate, glm::vec2(0.2f, 0.5f), 10.0f);
  _projectileSpawner.setSource(1);
}

Player::~Player() { }

bool Player::update(float deltaTime) {
  b2Vec2 velocity = _body->GetLinearVelocity();
  b2Vec2 force(0.0f, 0.0f), acceleration(0.0f, 0.0f);
  float maxSpeed = b2Max(_maxVelocity.x, _maxVelocity.y);

  if(_direction.x != 0) {
    acceleration.x = (_direction.x * _maxVelocity.x - velocity.x);
  } else if(velocity.x != 0) {
    acceleration.x = (_defaultVelocity.x - velocity.x);
  }

  if(_direction.y != 0) {
    if(_direction.y < 0) {
      acceleration.y = (b2Max(velocity.y - 0.05f, _direction.y * 0.1f) - velocity.y);
    } else {
      acceleration.y = (b2Min(_direction.y * _maxVelocity.y, velocity.y + 0.05f) - velocity.y);
    }
  } else {
    acceleration.y = (_defaultVelocity.y - velocity.y);
  }

  force = _body->GetMass() * deltaTime * acceleration;

  _body->ApplyForce(force, _body->GetWorldCenter(), true);
  //_body->SetLinearVelocity(Utils::toB2Vec2(_direction * _maxVelocity));

  float currentSpeed = velocity.Length();
  if(currentSpeed > maxSpeed) {
    _body->SetLinearVelocity((maxSpeed / currentSpeed) * velocity);
  }

  glm::vec2 position = Utils::toVec2(_body->GetPosition());

  _projectileSpawner.update(deltaTime, _isFiring, position + glm::vec2(0.0f, _height / 2 + _projectileSpawner.getProjectileHeight() / 2), glm::vec2(0.0f, 10.0f));

  return true;
}