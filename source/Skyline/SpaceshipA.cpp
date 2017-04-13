#include "SpaceshipA.h"

SpaceshipA::SpaceshipA() : SpaceshipA(0, glm::vec4(0.0f), 0.0f, 0.0f, glm::vec2(0.0f, 0.0f), 0.0f) { }

SpaceshipA::SpaceshipA(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position, float angle) : Entity(textureId, uv, width, height, position, angle) {
  initAnimations();
}

SpaceshipA::~SpaceshipA() {
  delete _animationManager;
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

  _animationManager->update(deltaTime);

  return true;
}

void SpaceshipA::draw() {
  if (!_isSpawned) return;

  b2Vec2 bodyPosition = this->_body->GetPosition();
  glm::vec2 screenPosition = _position;

  Ess2D::SpriteBatch* spriteBatch = _game->getGameplayScreen()->getSpriteBatch();
  Ess2D::TextureAtlas* textureAtlas = _animationManager->getCurrent()->getTextureAtlas();
  std::string currentAnimationFrame = _animationManager->getCurrent()->getCurrentFrame();

  float width = textureAtlas->getSize(currentAnimationFrame).x * _horizontalScaleFactor;

  spriteBatch->draw(glm::vec4(screenPosition.x - width / 2, screenPosition.y - _height / 2, width, _height), textureAtlas->getUV(currentAnimationFrame), textureAtlas->getTextureId(), _color, (float)_depth, _body->GetAngle());
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

}

void SpaceshipA::initProjectileSpawner() {

}
