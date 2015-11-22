#pragma once

#include <glew.h>

namespace Essengine {

  class GLTexture
  {
    public:
      //methods
      GLTexture();
      ~GLTexture();

      //properties
      GLuint _id;

      int _height;
      int _width;
      
      int _mode;
  };

}