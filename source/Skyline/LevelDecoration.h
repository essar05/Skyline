#pragma once
#include <string>
#include <Texture2D.h>

class LevelDecoration {

  public:
    LevelDecoration(GLuint textureId, float x, float y, float width, float height, float scale, float displacement);
    ~LevelDecoration();

    void setScale(float scale);
   
    float getX();
    float getY();
    float getWidth();
    float getHeight();
    float getDisplacement();
    GLuint getTextureId();

  private:
    GLuint _textureId;
    float _scale = 1.0f;
    int _layer = 0;
    float _x;
    float _y;
    float _width;
    float _height;
    float _displacement;

};

