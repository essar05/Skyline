#pragma once
#include "GLTexture.h"
#include <string>

namespace Essengine {

  class IOManager
  {
    public:
      static GLTexture loadTextureFromImage(std::string filePath);
  };

}