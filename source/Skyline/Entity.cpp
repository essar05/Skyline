#define NOMINMAX

#include "Entity.h"
#include "Game.h"
#include <algorithm>

Entity::Entity() {
}

Entity::Entity(int textureId, float width, float height, glm::vec2 position) : Entity(textureId, width, height, position, true) { }

Entity::Entity(int textureId, float width, float height, glm::vec2 position, bool scaleToWorld) : _textureId(textureId), _width(width), _height(height), _position(position) {
  _game = Game::GetInstance();

  _color = Essengine::ColorRGBA8(225, 255, 255, 255);

  if(scaleToWorld) {
    _width = _game->getMainCamera()->getWorldScalar(_width);
    _height = _game->getMainCamera()->getWorldScalar(_height);
    _position = _game->getMainCamera()->getWorldCoordinates(_position);
  }

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(_position.x, _position.y);
  bodyDef.angle = 0;
  bodyDef.fixedRotation = true;
  _body = _game->getLevel()->getWorld()->CreateBody(&bodyDef);

  b2PolygonShape boxShape;
  boxShape.SetAsBox(_width / 2, _height / 2);

  b2FixtureDef boxFixtureDef;
  boxFixtureDef.shape = &boxShape;
  boxFixtureDef.density = 1;

  _body->CreateFixture(&boxFixtureDef);
}

Entity::~Entity() {
  //_game->getLevel()->getWorld()->DestroyBody(_body);
}

void Entity::setDirection(const glm::vec2& direction) {
  _direction = direction;
}

void Entity::setAcceleration(const glm::vec2& acceleration) {
  _acceleration = acceleration;
}

void Entity::setVelocity(const glm::vec2& velocity) {
  _velocity = velocity;
}

void Entity::setBaseVelocity(const glm::vec2& velocity) {
  _baseVelocity = velocity;
}

glm::vec2 Entity::getBaseVelocity() {
  return _baseVelocity;
}

void Entity::setBaseDirection(const glm::vec2& direction) {
  _baseDirection = direction;
}

glm::vec2 Entity::getBaseDirection() {
  return _baseDirection;
}

void Entity::update(float deltaTime) {
  _velocity += _acceleration * deltaTime;

  _position += _velocity * deltaTime;
}

void Entity::applyDamage(float damage) {
  _health -= damage;
}

bool Entity::collidesWith(Entity* anotherEntity) {
  b2Vec2 bPosition = anotherEntity->getBody()->GetPosition();
  bool result = collidesWith(anotherEntity->_width, anotherEntity->_height, glm::vec2(bPosition.x, bPosition.y));

  if(result) {
    anotherEntity->setColor(Essengine::ColorRGBA8(235, 0, 0, 255));
  } else {
    anotherEntity->setColor(Essengine::ColorRGBA8(255, 255, 255, 255));
  }

  return result;
}

bool Entity::inViewport() {
  glm::vec2 viewportSize = _game->getMainCamera()->getWorldViewportSize();
  glm::vec2 cameraPosition = _game->getMainCamera()->getPosition();
  return collidesWith(viewportSize.x, viewportSize.y, cameraPosition);
}

bool Entity::collidesWith(float bWidth, float bHeight, const glm::vec2& bPosition) {
  return (_position.x < bPosition.x + bWidth &&
          _position.x + _width > bPosition.x &&
          _position.y < bPosition.y + bHeight &&
          _position.y + _height > bPosition.y);
}

void Entity::draw() {
  if(_isSpawned) {
    b2Vec2 bodyPosition = this->_body->GetPosition();
    glm::vec2 screenPosition = glm::vec2(bodyPosition.x, bodyPosition.y);
    //float width = this->_body->GetFixtureList()->GetShape()->;

    Essengine::SpriteBatch* spriteBatch = _game->getSpriteBatch();
    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    spriteBatch->draw(glm::vec4(screenPosition.x - _width / 2, screenPosition.y - _height / 2, _width, _height), uv, _textureId, _color, 0);
  }
}

void Entity::spawn() {
  _isSpawned = true;
}

float Entity::isSpawned() {
  return _isSpawned;
}

glm::vec2 Entity::getPosition() {
  return _position;
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