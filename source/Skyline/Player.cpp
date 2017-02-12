#include "Player.h"
#include "Utils.h"
#include <iostream>

Player::Player() : Player(0, glm::vec4(0.0f), 0.0f, 0.0f, glm::vec2(0.0f, 0.0f)) { }

Player::Player(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position) : Entity(textureId, uv, width, height, position) {
  _projectileSpawnerLeft = ProjectileSpawner(8.0f, glm::vec2(0.5f, 0.9f), 40.0f);
  _projectileSpawnerLeft.setSource(this->getType());
  _projectileSpawnerRight = ProjectileSpawner(8.0f, glm::vec2(0.5f, 0.9f), 40.0f);
  _projectileSpawnerRight.setSource(this->getType());

  // SPACESHIP

  Essengine::TextureAtlas * playerAtlas = _game->getTextureCache()->getAtlas("Textures/player.png", "Textures/player.json");

  _animationManager = new Essengine::AnimationManager();
  
  Essengine::Animation* idleAnimation = _animationManager->add("IDLE");
  idleAnimation->setPlaybackRate(10.0f / 60.0f);
  idleAnimation->setTextureAtlas(playerAtlas);
  idleAnimation->setFrames(std::vector<std::string> {"Spaceship_default"});

  Essengine::Animation* bankLeftAnimation = _animationManager->add("BANK_LEFT");
  bankLeftAnimation->setPlaybackRate(2.5f / 60.0f);
  bankLeftAnimation->setTextureAtlas(playerAtlas);
  bankLeftAnimation->setRepeat(false);
  bankLeftAnimation->setFrames(std::vector<std::string> {"Spaceship_left01", "Spaceship_left02", "Spaceship_left03"});

  Essengine::Animation* bankRightAnimation = _animationManager->add("BANK_RIGHT");
  bankRightAnimation->setPlaybackRate(2.5f / 60.f);
  bankRightAnimation->setTextureAtlas(playerAtlas);
  bankRightAnimation->setRepeat(false);
  bankRightAnimation->setFrames(std::vector<std::string> {"Spaceship_right01", "Spaceship_right02", "Spaceship_right03"});

  _animationManager->play("IDLE");

  glm::vec2 frameSize = idleAnimation->getTextureAtlas()->getSize(idleAnimation->getCurrentFrame());

  _horizontalScaleFactor = _width / frameSize.x;

  // THRUSTER

  Essengine::TextureAtlas * thrusterAtlas = _game->getTextureCache()->getAtlas("Textures/thruster.png", "Textures/thruster.json");

  _thrusterAnimationManager = new Essengine::AnimationManager();

  Essengine::Animation* thrusterIdleAnim = _thrusterAnimationManager->add("IDLE");
  thrusterIdleAnim->setPlaybackRate(1.0f / 60.0f);
  thrusterIdleAnim->setTextureAtlas(thrusterAtlas);
  std::vector<std::string> thrusterAnimationFrames;
  for (int i = 0; i < 36; i++) {
    std::string extraZero = "0";
    if (i > 8) {
      extraZero = "";
    }
    thrusterAnimationFrames.push_back("Thruster_" + extraZero + std::to_string(i + 1));
  }
  
  thrusterIdleAnim->setFrames(thrusterAnimationFrames);

  _thrusterAnimationManager->play("IDLE");

  _thrusterWidth = _game->getMainCamera()->getWorldScalar(_thrusterWidth);
  _thrusterHeight = _game->getMainCamera()->getWorldScalar(_thrusterHeight);

  //MUZZLE FLASH
  Essengine::TextureAtlas * muzzleAtlas = _game->getTextureCache()->getAtlas("Textures/muzzle.png", "Textures/muzzle.json");

  _muzzleLeftAnimationManager = new Essengine::AnimationManager();
  _muzzleRightAnimationManager = new Essengine::AnimationManager();

  std::vector<std::string> muzzleAnimationFrames;
  for (int i = 0; i <= 16; i++) {
    muzzleAnimationFrames.push_back("muzzle_1_" + std::to_string(i));
  }

  Essengine::Animation* muzzleLeftAnimation = _muzzleLeftAnimationManager->add("MUZZLE");
  muzzleLeftAnimation->setPlaybackRate(0.8f / 60.0f);
  muzzleLeftAnimation->setTextureAtlas(muzzleAtlas);
  muzzleLeftAnimation->setFrames(muzzleAnimationFrames);
  muzzleLeftAnimation->setRepeat(false);
  muzzleLeftAnimation->setAutoStop(true);

  Essengine::Animation* muzzleRightAnimation = _muzzleRightAnimationManager->add("MUZZLE");
  muzzleRightAnimation->setPlaybackRate(0.8f / 60.0f);
  muzzleRightAnimation->setTextureAtlas(muzzleAtlas);
  muzzleRightAnimation->setFrames(muzzleAnimationFrames);
  muzzleRightAnimation->setRepeat(false);
  muzzleRightAnimation->setAutoStop(true);
}

Player::~Player() { 
  delete _animationManager;
  delete _thrusterAnimationManager;
  delete _muzzleLeftAnimationManager;
  delete _muzzleRightAnimationManager;
}

bool Player::update(float deltaTime) {
  b2Vec2 velocity = _body->GetLinearVelocity();
  b2Vec2 force(0.0f, 0.0f), acceleration(0.0f, 0.0f), desiredVelocity(0.0f, 0.0f);
  float maxSpeed = b2Max(_maxVelocity.x, _maxVelocity.y);

  //Determine animations
  if (_direction.x < 0) {
    _animationManager->play("BANK_LEFT");
  } else if (_direction.x > 0) {
    _animationManager->play("BANK_RIGHT");
  } else {
    if (_animationManager->getCurrentAnimationName() != "IDLE") {
      Essengine::Animation* currentAnimation = _animationManager->getCurrent();
      if (!currentAnimation->isReversed()) {
        currentAnimation->setReverse(true);
      } else if (currentAnimation->getCurrentFrameNumber() == 0) {
        _animationManager->play("IDLE");
      }
    }
  }

  //Handle Physics
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

  //Update Projectile Spawners
  _projectileSpawnerLeftPosition = Utils::toVec2(_body->GetPosition()) + glm::vec2(-_width / 2 + 0.5f, _height / 2 + _projectileSpawnerLeft.getProjectileHeight() - 2.0f);
  _projectileSpawnerRightPosition = _projectileSpawnerLeftPosition + glm::vec2(_width - 1.0f, 0.0f);

  int projectilesSpawnedLeft = _projectileSpawnerLeft.update(deltaTime, _isFiring, _projectileSpawnerLeftPosition, glm::vec2(0.0f, 50.0f));
  int projectilesSpawnedRight = _projectileSpawnerRight.update(deltaTime, _isFiring, _projectileSpawnerRightPosition, glm::vec2(0.0f, 50.0f));

  if (projectilesSpawnedLeft > 0) {
    _muzzleLeftAnimationManager->play("MUZZLE");
    _muzzleLeftAnimationManager->getCurrent()->reset();
    _muzzleRightAnimationManager->play("MUZZLE");
    _muzzleRightAnimationManager->getCurrent()->reset();
  }

  //Update Animations
  _animationManager->update(deltaTime);
  _thrusterAnimationManager->update(deltaTime);
  _muzzleLeftAnimationManager->update(deltaTime);
  _muzzleRightAnimationManager->update(deltaTime);

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

    spriteBatch->draw(glm::vec4(screenPosition.x - width / 2, screenPosition.y - _height / 2, width, _height), textureAtlas->getUV(currentAnimationFrame), textureAtlas->getTextureId(), _color, 2);

    Essengine::TextureAtlas* thrusterTextureAtlas = _thrusterAnimationManager->getCurrent()->getTextureAtlas();
    std::string thrusterCurrentAnimationFrame = _thrusterAnimationManager->getCurrent()->getCurrentFrame();
    
    spriteBatch->draw(glm::vec4(screenPosition.x - _thrusterWidth / 2, screenPosition.y - _height + 0.53f, _thrusterWidth, _thrusterHeight), thrusterTextureAtlas->getUV(thrusterCurrentAnimationFrame), thrusterTextureAtlas->getTextureId(), _color, 1);
  
    if (_muzzleLeftAnimationManager->isPlaying()) {
      Essengine::TextureAtlas* muzzleLeftTextureAtlas = _muzzleLeftAnimationManager->getCurrent()->getTextureAtlas();
      std::string muzzleLeftCurrentAnimationFrame = _muzzleLeftAnimationManager->getCurrent()->getCurrentFrame();

      spriteBatch->draw(glm::vec4(_projectileSpawnerLeftPosition.x - 0.35f, _projectileSpawnerLeftPosition.y + 0.05f, 0.9f, 0.9f), muzzleLeftTextureAtlas->getUV(muzzleLeftCurrentAnimationFrame), muzzleLeftTextureAtlas->getTextureId(), _color, 1);

      Essengine::TextureAtlas* muzzleRightTextureAtlas = _muzzleRightAnimationManager->getCurrent()->getTextureAtlas();
      std::string muzzleRightCurrentAnimationFrame = _muzzleRightAnimationManager->getCurrent()->getCurrentFrame();

      spriteBatch->draw(glm::vec4(_projectileSpawnerRightPosition.x - 0.40f, _projectileSpawnerRightPosition.y + 0.05f, 0.9f, 0.9f), muzzleRightTextureAtlas->getUV(muzzleRightCurrentAnimationFrame), muzzleRightTextureAtlas->getTextureId(), _color, 1);
    }
  }
}

void Player::contact(Entity* e) {
  if(e->getType() == ET_ENTITY) {
    applyDamage(e->getCollisionDamage());
    _game->getEntityManager()->deleteEntity(e->getId(), true);
  }
}

