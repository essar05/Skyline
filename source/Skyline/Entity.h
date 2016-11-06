#pragma once

#include <Vertex.h>
#include <glm\glm.hpp>
#include <Box2D.h>

class Game;

class Entity {
  public:
    Entity();
    Entity(int textureId, float width, float height, glm::vec2 position);
    Entity(int textureId, float width, float height, glm::vec2 position, bool scaleToWorld);
    ~Entity();

    void setDirection(const glm::vec2& direction);
    void setVelocity(const glm::vec2& velocity);
    void setColor(const Essengine::ColorRGBA8& color) { _color = color; }

    glm::vec2 getPosition();
    glm::vec2 getDirection();
    float getWidth();
    float getHeight();
    float getCollisionDamage() { return _collisionDamage; }
    float getHealth() { return _health; }
    b2Body* getBody() { return _body;  }

    void draw();
  
    virtual bool update(float deltaTime);
    
    void applyDamage(float damage);

    bool collidesWith(Entity* anotherEntity);
    bool inViewport();
    bool collidesWith(float width, float height, const glm::vec2& position);

    void spawn();
    float isSpawned();

  protected:
    Game* _game;

    virtual void createBody();
    virtual void createFixture();

    float _health = 100.0f;
    float _width;
    float _height;
    glm::vec2 _position;
    glm::vec2 _direction = glm::vec2(0.0f);
    glm::vec2 _velocity = glm::vec2(0.0f);
    
    b2Body* _body;
    
    Essengine::ColorRGBA8 _color;

    float _collisionDamage = 0.0f;

    float _isAlive = false;
    float _isSpawned = false;
    
    int _textureId;

};

