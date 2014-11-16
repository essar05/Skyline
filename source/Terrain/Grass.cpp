#include "Grass.h"

using namespace Terrain;

Grass::Grass() {
  this->width = 128;
  this->height = 64;
  this->vertices = new GLfloat[2*4] {
     (float) -(this->width / 2) , (float) -(this->height / 2),
     (float) (this->width / 2), (float) -(this->height / 2),
     (float) (this->width / 2),  (float) (this->height / 2),
     (float) -(this->width / 2),  (float) (this->height / 2)
  };
  this->uv = new GLfloat[2*4]{
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
  };
}

Grass::~Grass()
{
  //dtor
}
