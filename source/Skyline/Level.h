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

class Level {
  public:
    Level();
    Level(float height, float width);
    ~Level();

    void addSection(LevelSection* section);
    void draw();
    void load(std::string levelName);
    void update(float deltaTime);

    void addActiveObject(unsigned int k);
    void discardActiveObject();
    std::vector<unsigned int> getActiveObjects();

    b2World* getWorld();

  private:
    Game* _game;

    float _height;
    float _width;
    std::vector<unsigned int> _activeObjects;
    std::unordered_map<float, LevelSection*> _sections;
    std::unordered_map<float, LevelSection*>::iterator _renderStartIt;

    b2World* _world;
    b2GLDebugDraw _glDebugDrawInstance;

};