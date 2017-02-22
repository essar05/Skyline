#pragma once

#include <GL/glew.h>

namespace Ess2D {

  class Texture2D
  {
    public:
      //methods
      Texture2D();
      ~Texture2D();

      //properties
      GLuint _id;

      int _height;
      int _width;
      
      int _mode;
  };

}