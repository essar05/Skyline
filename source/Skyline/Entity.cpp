#define NOMINMAX

#include "Entity.h"
#include "Game.h"
#include "Utils.h"
#include <iostream>

Entity::Entity() {}

Entity::Entity(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position) : Entity(textureId, uv, width, height, position, 0.0f) { }

Entity::Entity(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position, float angle) : Entity(textureId, uv, width, height, position, angle, true) { }

Entity::Entity(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position, float angle, bool scaleToWorld) : _textureId(textureId), _uv(uv), _width(width), _height(height), _position(position), _angle(angle) {
  _game = Game::GetInstance();

  _color = Ess2D::ColorRGBA8(225, 255, 255, 255);

  if(scaleToWorld) {
    _width = _game->getGameplayScreen()->getMainCamera()->getWorldScalar(_width);
    _height = _game->getGameplayScreen()->getMainCamera()->getWorldScalar(_height);
    _position = _game->getGameplayScreen()->getMainCamera()->getWorldCoordinates(_position);
  }

  _previousPosition = _position;
}

Entity::~Entity() {
  if(_body != nullptr) {
    _body->GetWorld()->DestroyBody( _body );
  }
}

void Entity::setPosition(const glm::vec2& position) {
  _position = position;
}

void Entity::setPreviousPosition(const glm::vec2& position) {
  _previousPosition = position;
}

void Entity::setDirection(const glm::vec2& direction) {
  _direction = direction;
}

void Entity::setVelocity(const glm::vec2& velocity) {
  _velocity = velocity;
}

void Entity::setAngle(float angle) {
  _body->SetTransform(_body->GetPosition(), glm::radians(angle));
}

bool Entity::update(float deltaTime) {
  b2Vec2 velocity = _body->GetLinearVelocity();
  b2Vec2 force(0.0f, 0.0f), acceleration(0.0f, 0.0f);

  acceleration.x = _velocity.x - velocity.x;
  acceleration.y = _velocity.y - velocity.y;
  
  force = _body->GetMass() * acceleration;

  //_body->ApplyLinearImpulse(force, _body->GetWorldCenter(), true);
  _body->SetLinearVelocity(Utils::toB2Vec2(_velocity));

  return true;
}

void Entity::applyDamage(float damage) {
  _health -= damage;
}

bool Entity::collidesWith(Entity* anotherEntity) {
  b2Vec2 bPosition = anotherEntity->getBody()->GetPosition();
  bool result = collidesWith(anotherEntity->_width, anotherEntity->_height, glm::vec2(bPosition.x, bPosition.y));

  if(result) {
    anotherEntity->setColor(Ess2D::ColorRGBA8(235, 0, 0, 255));
  } else {
    anotherEntity->setColor(Ess2D::ColorRGBA8(255, 255, 255, 255));
  }

  return result;
}

bool Entity::inViewport() {
  glm::vec2 viewportSize = _game->getGameplayScreen()->getMainCamera()->getWorldViewportSize();
  glm::vec2 cameraPosition = _game->getGameplayScreen()->getMainCamera()->getPosition() / _game->getGameplayScreen()->getMainCamera()->getZoom();

  return collidesWith(viewportSize.x, viewportSize.y, cameraPosition);
}

bool Entity::collidesWith(float bWidth, float bHeight, const glm::vec2& bPosition) {
  glm::vec2 position = Utils::toVec2(_body->GetPosition());
  return (position.x - _width / 2 < bPosition.x + bWidth / 2 &&
          position.x + _width / 2 > bPosition.x - bWidth / 2 &&
          position.y - _height / 2 < bPosition.y + bHeight / 2 &&
          position.y + _height / 2 > bPosition.y - bHeight / 2);
}

void Entity::draw() {
  if(_isSpawned) {
    b2Vec2 bodyPosition = this->_body->GetPosition();
    glm::vec2 screenPosition = _position;

    Ess2D::SpriteBatch* spriteBatch = _game->getGameplayScreen()->getSpriteBatch();
    spriteBatch->draw(glm::vec4(screenPosition.x - _width / 2, screenPosition.y - _height / 2, _width, _height), _uv, _textureId, _color, (float) _depth, _body->GetAngle());
  }
}

void Entity::contact(Entity* entity) {}

void Entity::smoothStates(float timestepAccumulatorRatio) {
  float oneMinusRatio = 1.0f - timestepAccumulatorRatio;

  this->setPosition(
    timestepAccumulatorRatio * Utils::toVec2(_body->GetPosition()) +
    oneMinusRatio * _previousPosition
  );
}

void Entity::resetSmoothStates() {
  glm::vec2 position = Utils::toVec2(_body->GetPosition());
  _position = position;
  _previousPosition = position;
}

void Entity::createB2Data() {
  createBody();
  createFixtures();
  _body->SetUserData(this);
}

void Entity::createBody() {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(_position.x, _position.y);
  bodyDef.angle = glm::radians(_angle);
  bodyDef.fixedRotation = true;
  _body = _game->getGameplayScreen()->getLevel()->getWorld()->CreateBody(&bodyDef);
}

void Entity::createFixtures() {
  b2Vec2 vertices[3];
  vertices[0].Set(-_width / 2, 0.0f);
  vertices[1].Set(_width / 2, 0.0f);
  vertices[2].Set(0.0f, _height / 2);
  int32 count = 3;

  b2PolygonShape boxShape;
  boxShape.Set(vertices, count);

  //boxShape.SetAsBox(_width / 2, _height / 2);

  b2FixtureDef boxFixtureDef;
  boxFixtureDef.shape = &boxShape;
  boxFixtureDef.density = 1;
  boxFixtureDef.isSensor = 1;

  _body->CreateFixture(&boxFixtureDef);

  b2PolygonShape boxShape2;
  boxShape2.SetAsBox(_width / 7, _height / 4, b2Vec2(0.0f, -_height / 4), 0);

  b2FixtureDef boxFixtureDef2;
  boxFixtureDef2.shape = &boxShape2;
  boxFixtureDef2.density = 1;
  boxFixtureDef2.isSensor = 1;
  _body->CreateFixture(&boxFixtureDef2);
}

void Entity::die() {
}

void Entity::cleanupBody() {
  _body = nullptr;
}

void Entity::spawn() {
  _isSpawned = true;
}

float Entity::isSpawned() {
  return _isSpawned;
}

glm::vec2 Entity::getPosition() {
  return Utils::toVec2(this->_body->GetPosition());
}

glm::vec2 Entity::getDirection() {
  return _direction;
}

float Entity::getWidth() {
  return _width;
}

float Entity::getHeight() {
  return _height;
}

