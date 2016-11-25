#include "Projectile.h"
#include "Game.h"

Projectile::Projectile() {
}

Projectile::Projectile(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position) : Entity(textureId, uv, width, height, position, false) {}
Projectile::Projectile(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position, int source, float damage) : Projectile(textureId, uv, width, height, position) {
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

void Projectile::contact(Entity* e) {
  if(e->getType() == ET_ENTITY && _source == ET_PLAYER) { //todo change to ET_DAMAGE
    e->applyDamage(this->getDamage());
    _game->getProjectileManager()->deleteProjectile(this->getId(), true);
  }
}

void Projectile::createFixtures() {
  b2PolygonShape boxShape;
  boxShape.SetAsBox(_width / 2, _height / 2);

  b2FixtureDef boxFixtureDef;
  boxFixtureDef.shape = &boxShape;
  boxFixtureDef.density = 1;

  _body->CreateFixture(&boxFixtureDef);
  _body->SetBullet(true);
}
