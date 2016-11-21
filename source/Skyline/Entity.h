#pragma once

#include <Vertex.h>
#include <glm\glm.hpp>
#include <Box2D.h>
#include "EntityType.h"

class Game;

class Entity {
  public:
    Entity();
    Entity(int textureId, float width, float height, glm::vec2 position);
    Entity(int textureId, float width, float height, glm::vec2 position, bool scaleToWorld);
    ~Entity();

    void draw();
    virtual bool update(float deltaTime);

    void setPosition(const glm::vec2& position);
    void setPreviousPosition(const glm::vec2& position);
    void setDirection(const glm::vec2& direction);
    void setVelocity(const glm::vec2& velocity);
    void setColor(const Essengine::ColorRGBA8& color) { _color = color; }
    void setId(unsigned int id) { _id = id; }

    unsigned int getId() { return _id; }
    virtual int getType() { return ET_ENTITY; }
    glm::vec2 getPosition();
    glm::vec2 getPreviousPosition() { return _previousPosition; }
    glm::vec2 getDirection();
    float getWidth();
    float getHeight();
    float getCollisionDamage() { return _collisionDamage; }
    float getHealth() { return _health; }
    b2Body* getBody() { return _body;  }

    void applyDamage(float damage);
    bool collidesWith(Entity* anotherEntity);
    bool collidesWith(float width, float height, const glm::vec2& position);
    virtual void contact(Entity* entity);
    virtual void createB2Data();
    bool inViewport();
    float isSpawned();
    void resetSmoothStates();
    void smoothStates(float timestepAccumulatorRatio);
    void spawn();
    
  protected:
    Game* _game;

    virtual void createBody();
    virtual void createFixtures();

    unsigned int _id = NULL;
    float _health = 100.0f;
    float _width;
    float _height;
    glm::vec2 _position;
    glm::vec2 _previousPosition;
    glm::vec2 _direction = glm::vec2(0.0f);
    glm::vec2 _velocity = glm::vec2(0.0f);
    
    b2Body* _body;
    
    Essengine::ColorRGBA8 _color;

    float _collisionDamage = 0.0f;

    float _isAlive = false;
    float _isSpawned = false;
    
    int _textureId;

};