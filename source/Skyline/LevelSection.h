#pragma once
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

  private:
    unsigned int _backgroundId;
    float _height;
    float _width;

};

