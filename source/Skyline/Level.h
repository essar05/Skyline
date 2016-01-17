#pragma once

#include <unordered_map>
#include "LevelSection.h"
#include <SpriteBatch.h>

class Level {
  public:
    Level();
    Level(float height, float width);
    ~Level();
    void addSection(LevelSection* section);
    void moveUp();
    void draw();

  private:
    float _height;
    float _width;
    std::unordered_map<float, LevelSection*> _sections;
    std::unordered_map<float, LevelSection*>::iterator _renderStartIt;
};

