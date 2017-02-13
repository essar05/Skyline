#include "Projectile.h"
#include "Game.h"

Projectile::Projectile() {
}

Projectile::Projectile(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position) : Entity(textureId, uv, width, height, position, false) {}
Projectile::Projectile(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position, int source, float damage) : Projectile(textureId, uv, width, height, position) {
  _source = source; 
  _damage = damage;

  Ess2D::TextureAtlas * _playerAtlas = _game->getTextureCache()->getAtlas("Textures/bullet.png", "Textures/bullet.json");

  _animationManager = new Ess2D::AnimationManager();

  Ess2D::Animation* idleAnimation = _animationManager->add("IDLE");
  idleAnimation->setPlaybackRate(10.0f / 60.0f);
  idleAnimation->setTextureAtlas(_playerAtlas);
  idleAnimation->setFrames(std::vector<std::string> {"bullet_red_0", "bullet_red_1", "bullet_red_2", "bullet_red_3", "bullet_red_4", "bullet_red_5", "bullet_red_6", "bullet_red_7"});
  _animationManager->play("IDLE");
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

void Projectile::draw() {
  if (_isSpawned) {
    b2Vec2 bodyPosition = this->_body->GetPosition();
    glm::vec2 screenPosition = _position;

    Ess2D::SpriteBatch* spriteBatch = _game->getSpriteBatch();
    Ess2D::TextureAtlas* textureAtlas = _animationManager->getCurrent()->getTextureAtlas();
    std::string currentAnimationFrame = _animationManager->getCurrent()->getCurrentFrame(); 
    spriteBatch->draw(glm::vec4(screenPosition.x - _width / 2, screenPosition.y - _height / 2, _width, _height), textureAtlas->getUV(currentAnimationFrame), textureAtlas->getTextureId(), _color, _depth);
  }
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
  boxFixtureDef.isSensor = 1;

  _body->CreateFixture(&boxFixtureDef);
  _body->SetBullet(true);
}
