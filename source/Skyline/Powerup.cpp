#include "Powerup.h"
#include "Game.h"

Powerup::Powerup() {
}

Powerup::Powerup(std::string type, float width, float height, glm::vec2 position) :
  Entity(0, glm::vec4(), width, height, position, 0.0f, false) {
  
  _powerupType = type;

  Ess2D::TextureAtlas * powerupsAtlas = _game->getGameplayScreen()->getTextureCache()->getAtlas("Textures/powerups.png", "Textures/powerups.json");
  _textureId = powerupsAtlas->getTextureId();

  if(_powerupType == "hp") {  
    _uv = powerupsAtlas->getUV("powerup_life");
  } else if(_powerupType == "score") {
    _uv = powerupsAtlas->getUV("powerup_score"); 
  } else if(_powerupType == "dmg") {
    _uv = powerupsAtlas->getUV("powerup_bad");
  }
}

Powerup::~Powerup() {
}

bool Powerup::update(float deltaTime) {
  if(_body == nullptr) return false;

  //Entity::update(deltaTime);

  //_body->ApplyTorque(50.0f, true);
  _body->SetAngularVelocity(3.0f);

  if(!this->inViewport()) {
    return false;
  }
  return true;
}

void Powerup::draw() {
  if(_isSpawned && !isDead() && _body != nullptr) {
    b2Vec2 bodyPosition = this->_body->GetPosition();
    glm::vec2 screenPosition = _position;

    Ess2D::SpriteBatch* spriteBatch = _game->getGameplayScreen()->getSpriteBatch();
    spriteBatch->draw(glm::vec4(screenPosition.x - _width / 2, screenPosition.y - _height / 2, _width, _height), _uv, _textureId, _color, (float)_depth, _body->GetAngle());
  }
}

void Powerup::createFixtures() {
  b2CircleShape circleShape;
  circleShape.m_p.Set(0, _height / 25.0f);
  circleShape.m_radius = _width / 2;

  b2FixtureDef circleFixtureDef;
  circleFixtureDef.shape = &circleShape;
  circleFixtureDef.density = 1;
  circleFixtureDef.isSensor = 1;

  _body->CreateFixture(&circleFixtureDef);
}