#include "Player.h"
#include "Utils.h"
#include <iostream>

Player::Player() : Player(0, glm::vec4(0.0f), 0.0f, 0.0f, glm::vec2(0.0f, 0.0f)) { }

Player::Player(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position) : Entity(textureId, uv, width, height, position) {
  _projectileSpawner = ProjectileSpawner(8.0f, glm::vec2(0.2f, 0.5f), 40.0f);
  _projectileSpawner.setSource(this->getType());

  Essengine::TextureAtlas * _playerAtlas = _game->getTextureCache()->getAtlas("Textures/player.png", "Textures/player.json");

  _animationManager = new Essengine::AnimationManager();

  Essengine::Animation* idleAnimation = _animationManager->add("IDLE");
  idleAnimation->setPlaybackRate(10.0f / 60.0f);
  idleAnimation->setTextureAtlas(_playerAtlas);
  idleAnimation->setFrames(std::vector<std::string> {"Spaceship_default"});

  Essengine::Animation* bankLeftAnimation = _animationManager->add("BANK_LEFT");
  bankLeftAnimation->setPlaybackRate(1.0f / 60.f);
  bankLeftAnimation->setTextureAtlas(_playerAtlas);
  bankLeftAnimation->setFrames(std::vector<std::string> {"Spaceship_left01", "Spaceship_left02", "Spaceship_left03"});

  Essengine::Animation* bankRightAnimation = _animationManager->add("BANK_RIGHT");
  bankRightAnimation->setPlaybackRate(1.0f / 60.f);
  bankRightAnimation->setTextureAtlas(_playerAtlas);
  bankRightAnimation->setFrames(std::vector<std::string> {"Spaceship_right01", "Spaceship_right02", "Spaceship_right03"});

  _animationManager->play("IDLE");

  glm::vec2 frameSize = idleAnimation->getTextureAtlas()->getSize(idleAnimation->getCurrentFrame());

  _horizontalScaleFactor = _width / frameSize.x;
}

Player::~Player() { 
  delete _animationManager;
}

bool Player::update(float deltaTime) {
  b2Vec2 velocity = _body->GetLinearVelocity();
  b2Vec2 force(0.0f, 0.0f), acceleration(0.0f, 0.0f), desiredVelocity(0.0f, 0.0f);
  float maxSpeed = b2Max(_maxVelocity.x, _maxVelocity.y);

  if (_direction.x < 0) {
    _animationManager->play("BANK_LEFT");
  } else if (_direction.x > 0) {
    _animationManager->play("BANK_RIGHT");
  } else {
    _animationManager->play("IDLE");
  }

  if(_direction.x != 0) {
    acceleration.x = (_direction.x * _maxVelocity.x - velocity.x);
  } else if(velocity.x != 0) {
    acceleration.x = (_defaultVelocity.x - velocity.x);
  }

  if(_direction.y != 0) {
    if(_direction.y < 0) {
      acceleration.y = _direction.y * _maxVelocity.y - velocity.y;
    } else {
      acceleration.y = (b2Min(_direction.y * (_maxVelocity.y + _defaultVelocity.y), velocity.y + 8.5f) - velocity.y);
    }
  } else {
    acceleration.y = (_defaultVelocity.y - velocity.y);
  }

  force = _body->GetMass() * acceleration;

  _body->ApplyLinearImpulse(force, _body->GetWorldCenter(), true);

  float currentSpeed = velocity.Length();
  if(currentSpeed > maxSpeed) {
    _body->SetLinearVelocity((maxSpeed / currentSpeed) * velocity);
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

  _animationManager->update(deltaTime);

  return true;
}

void Player::draw() {
  if (_isSpawned) {
    b2Vec2 bodyPosition = this->_body->GetPosition();
    glm::vec2 screenPosition = _position;

    Essengine::SpriteBatch* spriteBatch = _game->getSpriteBatch();
    Essengine::TextureAtlas* textureAtlas = _animationManager->getCurrent()->getTextureAtlas();
    std::string currentAnimationFrame = _animationManager->getCurrent()->getCurrentFrame();

    float width = textureAtlas->getSize(currentAnimationFrame).x * _horizontalScaleFactor;

    spriteBatch->draw(glm::vec4(screenPosition.x - width / 2, screenPosition.y - _height / 2, width, _height), textureAtlas->getUV(currentAnimationFrame), textureAtlas->getTextureId(), _color, 1);
  }
}

void Player::contact(Entity* e) {
  if(e->getType() == ET_ENTITY) {
    applyDamage(e->getCollisionDamage());
    _game->getEntityManager()->deleteEntity(e->getId(), true);
  }
}

