#pragma once
#include <vector>

class LevelSection {
  public:
    LevelSection(float height, float width);
    ~LevelSection();

    void setBackground(unsigned int backgroundId) {
      _backgroundId = backgroundId;
    }

    unsigned int getBackground() {
      return _backgroundId;
    }

    float getHeight() {
      return _height;
    }
    float getWidth() {
      return _width;
    }
    void draw(float x, float y);
    void addObject(unsigned int objectId);
    static bool compareObjectYPosition(unsigned int a, unsigned int b);

  private:
    unsigned int _backgroundId;
    float _height;
    float _width;

    std::vector<unsigned int> _objects;
    unsigned int _renderStartIt = 0;

};

