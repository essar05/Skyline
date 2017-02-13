#pragma once
#include "Texture2D.h"
#include <string>

namespace Ess2D {

  class IOManager
  {
    public:
      static Texture2D loadTextureFromImage(std::string filePath);
  };

}