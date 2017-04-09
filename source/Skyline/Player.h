#pragma once
#include "Entity.h"
#include "ProjectileSpawner.h"
#include "Game.h"
#include <AnimationManager.h>

class Player : public Entity {
  public:
    Player();
    Player(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position);
    ~Player();

    void setIsFiring(bool isFiring) { _isFiring = isFiring; }
    void setDefaultVelocity(glm::vec2 velocity) { _defaultVelocity = velocity; }
    void draw();
    bool update(float deltaTime) override;
    int getType() override { return ET_PLAYER; }
    void contact(Entity* e) override;
    void correctProjectileSpawnersPosition(const std::string& currentPlayerFrame);

  private:

    glm::vec2 _defaultVelocity = glm::vec2(0.0f, 10.0f);
    glm::vec2 _maxVelocity = glm::vec2(20.0f, 20.0f);
    
    float _fireRate = 8.0f; //
    bool _isFiring = false;
    ProjectileSpawner _projectileSpawnerLeft;
    ProjectileSpawner _projectileSpawnerRight;
    Ess2D::AnimationManager* _animationManager;
    Ess2D::AnimationManager* _thrusterAnimationManager;
    Ess2D::AnimationManager* _muzzleLeftAnimationManager;
    Ess2D::AnimationManager* _muzzleRightAnimationManager;

    glm::vec2 _projectileSpawnerLeftPosition;
    glm::vec2 _projectileSpawnerRightPosition;

    float _horizontalScaleFactor = 1.0f;

    float _thrusterHeight = 30.0f;
    float _thrusterWidth = 30.0f;

};

