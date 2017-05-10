#pragma once
#include "Entity.h"
class Powerup: public Entity {
  public:
    Powerup();
    Powerup(std::string type, float width, float height, glm::vec2 position);
    ~Powerup();

    bool update(float deltaTime) override;
    int getType() override { return ET_POWERUP; }
    std::string getPowerupType() { return _powerupType; }

    void draw();

  protected:
    virtual void createFixtures() override;
    int _depth = 9000;
    std::string _powerupType;

  private:
    
};
