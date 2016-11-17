#include "Player.h"
#include "Utils.h"
#include <iostream>

Player::Player() : Player(0, 0.0f, 0.0f, glm::vec2(0.0f, 0.0f)) { }

Player::Player(int textureId, float width, float height, glm::vec2 position) : Entity(textureId, width, height, position) {
  _projectileSpawner = ProjectileSpawner(8.0f, glm::vec2(0.2f, 0.5f), 40.0f);
  _projectileSpawner.setSource(this->getType());
}

Player::~Player() { }

bool Player::update(float deltaTime) {
  b2Vec2 velocity = _body->GetLinearVelocity();
  b2Vec2 force(0.0f, 0.0f), acceleration(0.0f, 0.0f), desiredVelocity(0.0f, 0.0f);
  float maxSpeed = b2Max(_maxVelocity.x, _maxVelocity.y);

  if(_direction.x != 0) {
    acceleration.x = (_direction.x * _maxVelocity.x - velocity.x);
  } else if(velocity.x != 0) {
    acceleration.x = (_defaultVelocity.x - velocity.x);
  }

  if(_direction.y != 0) {
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

  float currentSpeed = velocity.Length();
  if(currentSpeed > maxSpeed) {
    //_body->SetLinearVelocity((maxSpeed / currentSpeed) * velocity);
  }

  /* BIND PLAYER WITHIN THE VIEWPORT */
  //calculate next step position
  glm::vec2 nextPosition = glm::vec2(
    _body->GetPosition().x + _body->GetLinearVelocity().x * deltaTime + (acceleration.x * deltaTime * deltaTime) / 2,
    _body->GetPosition().y + _body->GetLinearVelocity().y * deltaTime + (acceleration.y * deltaTime * deltaTime) / 2
  );

  glm::vec2 viewportSize = _game->getMainCamera()->getWorldViewportSize();
  glm::vec2 cameraPosition = _game->getMainCamera()->getPosition() / _game->getMainCamera()->getZoom();
  b2Vec2 correctedPosition = _body->GetPosition();
  b2Vec2 correctionAcceleration = b2Vec2(0.0f, 0.0f);
  b2Vec2 currentVelocity = _body->GetLinearVelocity();

  bool doCorrectPosition = false;

  if(nextPosition.x - _width / 2 < cameraPosition.x - viewportSize.x / 2) {
    correctedPosition.x = cameraPosition.x - viewportSize.x / 2 + _width / 2;
    correctionAcceleration.x = 0.0f - currentVelocity.x;
    doCorrectPosition = true;
  }
  if(nextPosition.x + _width / 2 > cameraPosition.x + viewportSize.x / 2) {
    correctedPosition.x = cameraPosition.x + viewportSize.x / 2 - _width / 2;
    correctionAcceleration.x = 0.0f - currentVelocity.x;
    doCorrectPosition = true;
  }
  if(nextPosition.y - _height / 2 < cameraPosition.y - viewportSize.y / 2 && _direction.y != 1) {
    correctedPosition.y = cameraPosition.y - viewportSize.y / 2 + _height / 2;
    correctionAcceleration.y = 0.0f - currentVelocity.y;
    doCorrectPosition = true;
  }
  if(nextPosition.y + _height / 2 > cameraPosition.y + viewportSize.y / 2 && _direction.y != -1) {
    correctedPosition.y = cameraPosition.y + viewportSize.y / 2 - _height / 2;
    correctionAcceleration.y = _defaultVelocity.y * 0.99f - currentVelocity.y;
    doCorrectPosition = true;
  }
  
  //if we have corrections to do, we must make sure to stop the body's velocity in the corrected direction as well.
  //still can be a bit weird... could be interpolated camera position or something...
  if(doCorrectPosition) {
    b2Vec2 force = _body->GetMass() * correctionAcceleration;
    //the impulse is applied in order to stop the body from moving further in that direction.
    _body->ApplyLinearImpulse(force, _body->GetWorldCenter(), true);

    _body->SetTransform(correctedPosition, _body->GetAngle());
  }

  glm::vec2 position = Utils::toVec2(_body->GetPosition()) + glm::vec2(0.0f, _height / 2 + _projectileSpawner.getProjectileHeight());

  _projectileSpawner.update(deltaTime, _isFiring, position, glm::vec2(0.0f, 50.0f));

  return true;
}

void Player::contact(Entity* e) {
  if(e->getType() == ET_ENTITY) {
    applyDamage(e->getCollisionDamage());
    _game->getEntityManager()->deleteEntity(e->getId(), true);
  }
}