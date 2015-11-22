#ifndef MODEL_H
#define MODEL_H

#include "GLW.h"
#include "Model.h"

namespace Essengine {
  class Model
  {
    public:
      Model();
      virtual ~Model();
      int getHeight();
      int getWidth();
      GLfloat* getUV();
      GLfloat* getVertices();
    protected:
      int height;
      int width;
      GLfloat* uv;
      GLfloat* vertices;
    private:
  };
};

#endif // MODEL_H
