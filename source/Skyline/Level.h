#pragma once

#include <unordered_map>
#include "LevelSection.h"
#include <SpriteBatch.h>
#include <TextureCache.h>
#include "EntityManager.h"
#include <rapidjson.h>
#include <document.h>
#include <fstream>
#include <streambuf>
#include <Box2D.h>
#include "b2GLDebugDraw.h"
#include "Player.h"
#include "Game.h"
#include "ContactListener.h"

class Player;

class Level {
  public:
    Level();
    Level(float height, float width);
    ~Level();

    void addSection(LevelSection* section);
    void draw();
    void load(std::string levelName);
    void update(float deltaTime);
    void smoothStates();
    void resetSmoothStates();

    void setBackground(Ess2D::Texture2D* texture);

    b2World* getWorld();
    Player* getPlayer();

  private:
    Game* _game;
    Player* _player;

    float _height;
    float _width;

    std::map<float, LevelSection*> _sections;
    std::map<float, LevelSection*>::iterator _renderStartIt;

    b2World* _world;
    b2GLDebugDraw _glDebugDrawInstance;
    ContactListener _contactListener;
    unsigned int _backgroundId;
    float _backgroundWidth;
    float _backgroundHeight;

};