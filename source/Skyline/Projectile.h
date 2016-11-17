#pragma once

#include "Entity.h"
#include <string>

class Projectile : public Entity {
  public:
    Projectile();
    Projectile(int textureId, float width, float height, glm::vec2 position);
    Projectile(int textureId, float width, float height, glm::vec2 position, int source, float _damage);
    ~Projectile();

    bool update(float deltaTime) override;
    int getSource() { return _source; }
    float getDamage() { return _damage; }
    int getType() override { return ET_PROJECTILE; }
    void contact(Entity* e) override;

  protected:
    void createFixture();

  private:
    int _source; //1 -> player, 2 -> enemies;
    float _damage;

};

