#include "Projectile.h"


Projectile::Projectile() {
}

Projectile::Projectile(int textureId, float width, float height, glm::vec2 position) : Entity(textureId, width, height, position) {}
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
