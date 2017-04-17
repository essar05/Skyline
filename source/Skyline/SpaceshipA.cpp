#include "SpaceshipA.h"
#include "Utils.h"

SpaceshipA::SpaceshipA() : SpaceshipA(0, glm::vec4(0.0f), 0.0f, 0.0f, glm::vec2(0.0f, 0.0f), 0.0f) { }

SpaceshipA::SpaceshipA(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position, float angle) : Entity(textureId, uv, width, height, position, angle) {
  initAnimations();
  initThruster();
  initProjectileSpawners();
}

SpaceshipA::~SpaceshipA() {
  delete _animationManager;
  delete _thrusterAnimationManager;
  delete _muzzleLeftAnimationManager;
  delete _muzzleRightAnimationManager;
}

bool SpaceshipA::update(float deltaTime) {
  Entity::update(deltaTime);

  //Determine animations
  if (_direction.x < 0) {
    _animationManager->play("BANK_LEFT");
  } else if (_direction.x > 0) {
    _animationManager->play("BANK_RIGHT");
  } else {
    if (_animationManager->getCurrentAnimationName() != "IDLE") {
      Ess2D::Animation* currentAnimation = _animationManager->getCurrent();
      if (!currentAnimation->isReversed()) {
        currentAnimation->setReverse(true);
      } else if (currentAnimation->getCurrentFrameNumber() == 0) {
        _animationManager->play("IDLE");
      }
    }
  }

  //Update Projectile Spawners
  _projectileSpawnerLeftPosition = glm::vec2(-0.75f, 0.75f);
  _projectileSpawnerRightPosition = glm::vec2(0.84f, 0.75f);

  _projectileSpawnerLeftPosition = Utils::rotatePoint(_projectileSpawnerLeftPosition, glm::vec2(0.0f, 0.0f), _body->GetAngle());
  _projectileSpawnerRightPosition = Utils::rotatePoint(_projectileSpawnerRightPosition, glm::vec2(0.0f, 0.0f), _body->GetAngle());

  glm::vec2 projectileVelocity = glm::vec2(0.0f, 40.0f);
  projectileVelocity = Utils::rotatePoint(projectileVelocity, glm::vec2(0.0f, 0.0f), _body->GetAngle());

  glm::vec2 positionCorrection = Utils::rotatePoint(glm::vec2(0.0f, 0.5f), glm::vec2(0.0f, 0.0f), _body->GetAngle());

  //correctProjectileSpawnersPosition(_animationManager->getCurrent()->getCurrentFrame());

  int projectilesSpawnedLeft = _projectileSpawnerLeft.update(deltaTime, _isFiring, Utils::toVec2(_body->GetPosition()) + _projectileSpawnerLeftPosition + positionCorrection, projectileVelocity, _body->GetAngle());
  int projectilesSpawnedRight = _projectileSpawnerRight.update(deltaTime, _isFiring, Utils::toVec2(_body->GetPosition()) + _projectileSpawnerRightPosition + positionCorrection, projectileVelocity, _body->GetAngle());

  if(projectilesSpawnedLeft > 0) {
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

void SpaceshipA::draw() {
  if (!_isSpawned) return;

  b2Vec2 bodyPosition = this->_body->GetPosition();
  glm::vec2 screenPosition = _position;

  //SPACESHIP

  Ess2D::SpriteBatch* spriteBatch = _game->getGameplayScreen()->getSpriteBatch();
  Ess2D::TextureAtlas* textureAtlas = _animationManager->getCurrent()->getTextureAtlas();
  std::string currentAnimationFrame = _animationManager->getCurrent()->getCurrentFrame();

  float width = textureAtlas->getSize(currentAnimationFrame).x * _horizontalScaleFactor;

  spriteBatch->draw(glm::vec4(screenPosition.x - width / 2, screenPosition.y - _height / 2, width, _height), textureAtlas->getUV(currentAnimationFrame), textureAtlas->getTextureId(), _color, (float)_depth, _body->GetAngle());

  //THRUSTER
  glm::vec2 thrusterPosition = glm::vec2(screenPosition.x, screenPosition.y - _height / 2 - _thrusterHeight / 2 + 0.5f);
  thrusterPosition = Utils::rotatePoint(thrusterPosition, screenPosition, _body->GetAngle());

  Ess2D::TextureAtlas* thrusterTextureAtlas = _thrusterAnimationManager->getCurrent()->getTextureAtlas();
  std::string thrusterCurrentAnimationFrame = _thrusterAnimationManager->getCurrent()->getCurrentFrame();

  spriteBatch->draw(glm::vec4(thrusterPosition.x - _thrusterWidth / 2, thrusterPosition.y - _thrusterHeight / 2, _thrusterWidth, _thrusterHeight), thrusterTextureAtlas->getUV(thrusterCurrentAnimationFrame), thrusterTextureAtlas->getTextureId(), _color, (float)_depth - 1, _body->GetAngle());

  if(_muzzleLeftAnimationManager->isPlaying()) {
    Ess2D::TextureAtlas* muzzleLeftTextureAtlas = _muzzleLeftAnimationManager->getCurrent()->getTextureAtlas();
    std::string muzzleLeftCurrentAnimationFrame = _muzzleLeftAnimationManager->getCurrent()->getCurrentFrame();

    glm::vec2 leftMuzzlePosition = screenPosition + _projectileSpawnerLeftPosition;
    spriteBatch->draw(glm::vec4(leftMuzzlePosition.x - _muzzleSize.x / 2, leftMuzzlePosition.y - _muzzleSize.y / 2, _muzzleSize.x, _muzzleSize.y), muzzleLeftTextureAtlas->getUV(muzzleLeftCurrentAnimationFrame), muzzleLeftTextureAtlas->getTextureId(), _color, (float)_depth - 1, _body->GetAngle());

    Ess2D::TextureAtlas* muzzleRightTextureAtlas = _muzzleRightAnimationManager->getCurrent()->getTextureAtlas();
    std::string muzzleRightCurrentAnimationFrame = _muzzleRightAnimationManager->getCurrent()->getCurrentFrame();

    glm::vec2 rightMuzzlePosition = screenPosition + _projectileSpawnerRightPosition;
    spriteBatch->draw(glm::vec4(rightMuzzlePosition.x - _muzzleSize.x / 2, rightMuzzlePosition.y - _muzzleSize.y / 2, _muzzleSize.x, _muzzleSize.y), muzzleRightTextureAtlas->getUV(muzzleRightCurrentAnimationFrame), muzzleRightTextureAtlas->getTextureId(), _color, (float)_depth - 1, _body->GetAngle());
  }
}

void SpaceshipA::initAnimations() {
  Ess2D::TextureAtlas * spaceshipAAtlas = _game->getGameplayScreen()->getTextureCache()->getAtlas("Textures/spaceshipA.png", "Textures/spaceshipA.json");

  _animationManager = new Ess2D::AnimationManager();

  Ess2D::Animation* idleAnimation = _animationManager->add("IDLE");
  idleAnimation->setPlaybackRate(10.0f / 60.0f);
  idleAnimation->setTextureAtlas(spaceshipAAtlas);
  idleAnimation->setFrames(std::vector<std::string> {"spaceship_1_right_1"});

  Ess2D::Animation* bankLeftAnimation = _animationManager->add("BANK_LEFT");
  bankLeftAnimation->setPlaybackRate(2.5f / 60.0f);
  bankLeftAnimation->setTextureAtlas(spaceshipAAtlas);
  bankLeftAnimation->setRepeat(false);
  bankLeftAnimation->setFrames(std::vector<std::string> {"spaceship_1_left_1", "spaceship_1_left_2", "spaceship_1_left_3", "spaceship_1_left_4", "spaceship_1_left_5", "spaceship_1_left_6", "spaceship_1_left_7", "spaceship_1_left_8", "spaceship_1_left_9", "spaceship_1_left_10"});

  Ess2D::Animation* bankRightAnimation = _animationManager->add("BANK_RIGHT");
  bankRightAnimation->setPlaybackRate(2.5f / 60.f);
  bankRightAnimation->setTextureAtlas(spaceshipAAtlas);
  bankRightAnimation->setRepeat(false);
  bankRightAnimation->setFrames(std::vector<std::string> {"spaceship_1_right_1", "spaceship_1_right_2", "spaceship_1_right_3", "spaceship_1_right_4", "spaceship_1_right_5", "spaceship_1_right_6", "spaceship_1_right_7", "spaceship_1_right_8", "spaceship_1_right_9", "spaceship_1_right_10"});

  _animationManager->play("IDLE");

  glm::vec2 frameSize = idleAnimation->getTextureAtlas()->getSize(idleAnimation->getCurrentFrame());

  _horizontalScaleFactor = _width / frameSize.x;
}

void SpaceshipA::initThruster() {
  Ess2D::TextureAtlas * thrusterAtlas = _game->getGameplayScreen()->getTextureCache()->getAtlas("Textures/thruster_red.png", "Textures/thruster_red.json");

  _thrusterAnimationManager = new Ess2D::AnimationManager();

  Ess2D::Animation* thrusterIdleAnim = _thrusterAnimationManager->add("IDLE");
  thrusterIdleAnim->setPlaybackRate(2.5f / 60.0f);
  thrusterIdleAnim->setTextureAtlas(thrusterAtlas);
  thrusterIdleAnim->setRepeat(true);
  std::vector<std::string> thrusterAnimationFrames;
  for(int i = 0; i <= 10; i++) {
    thrusterAnimationFrames.push_back("thurstflame_red_" + std::to_string(i));
  }

  thrusterIdleAnim->setFrames(thrusterAnimationFrames);

  _thrusterAnimationManager->play("IDLE");

  _thrusterWidth = _game->getGameplayScreen()->getMainCamera()->getWorldScalar(_thrusterWidth);
  _thrusterHeight = _game->getGameplayScreen()->getMainCamera()->getWorldScalar(_thrusterHeight);
}

void SpaceshipA::initProjectileSpawners() {
  _projectileSpawnerLeft = ProjectileSpawner(_fireRate, glm::vec2(0.4f, 0.7f), 40.0f);
  _projectileSpawnerLeft.setSource(this->getType());
  _projectileSpawnerRight = ProjectileSpawner(_fireRate, glm::vec2(0.4f, 0.7f), 40.0f);
  _projectileSpawnerRight.setSource(this->getType());

  Ess2D::TextureAtlas * muzzleAtlas = _game->getGameplayScreen()->getTextureCache()->getAtlas("Textures/muzzle.png", "Textures/muzzle.json");

  _muzzleLeftAnimationManager = new Ess2D::AnimationManager();
  _muzzleRightAnimationManager = new Ess2D::AnimationManager();

  std::vector<std::string> muzzleAnimationFrames;
  for(int i = 0; i <= 16; i++) {
    muzzleAnimationFrames.push_back("muzzle_1_" + std::to_string(i));
  }

  Ess2D::Animation* muzzleLeftAnimation = _muzzleLeftAnimationManager->add("MUZZLE");
  muzzleLeftAnimation->setPlaybackRate(0.8f / 60.0f);
  muzzleLeftAnimation->setTextureAtlas(muzzleAtlas);
  muzzleLeftAnimation->setFrames(muzzleAnimationFrames);
  muzzleLeftAnimation->setRepeat(false);
  muzzleLeftAnimation->setAutoStop(true);

  Ess2D::Animation* muzzleRightAnimation = _muzzleRightAnimationManager->add("MUZZLE");
  muzzleRightAnimation->setPlaybackRate(0.8f / 60.0f);
  muzzleRightAnimation->setTextureAtlas(muzzleAtlas);
  muzzleRightAnimation->setFrames(muzzleAnimationFrames);
  muzzleRightAnimation->setRepeat(false);
  muzzleRightAnimation->setAutoStop(true);
}
