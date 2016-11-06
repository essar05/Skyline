#include "Projectile.h"


Projectile::Projectile() {
}

Projectile::Projectile(int textureId, float width, float height, glm::vec2 position) : Entity(textureId, width, height, position, false) {}
Projectile::Projectile(int textureId, float width, float height, glm::vec2 position, int source, float damage) : Projectile(textureId, width, height, position) { 
  _source = source; 
  _damage = damage;
}

Projectile::~Projectile() {
}

bool Projectile::update(float deltaTime) {
  Entity::update(deltaTime);
  if(!this->inViewport()) {
    return false;
  }
  return true;
}

void Projectile::createFixture() {
  b2PolygonShape boxShape;
  boxShape.SetAsBox(_width / 2, _height / 2);

  b2FixtureDef boxFixtureDef;
  boxFixtureDef.shape = &boxShape;
  boxFixtureDef.density = 1;

  _body->CreateFixture(&boxFixtureDef);
}
