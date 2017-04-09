#pragma once

#include <unordered_map>
#include "LevelSection.h"
#include "LevelDecoration.h"
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
    void addDecoration(LevelDecoration* decoration, int layer);
    void draw();
    void load(std::string levelName);
    void update(float deltaTime);
    void smoothStates();
    void resetSmoothStates();

    LevelSection* getSection(int index);

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

    std::map<int, std::vector<LevelDecoration*>> _decorations;
    std::vector<int> _decorationLayers;

    b2World* _world;
    b2GLDebugDraw _glDebugDrawInstance;
    ContactListener _contactListener;
    unsigned int _backgroundId;
    float _backgroundWidth;
    float _backgroundHeight;

    static bool compareDecorationYPosition(LevelDecoration* a, LevelDecoration* b);
    static bool compareDecorationLayerDesc(int y1, int y2);

};