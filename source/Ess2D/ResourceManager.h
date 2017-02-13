#pragma once
#include "TextureCache.h"
#include <string>

namespace Ess2D {

  class ResourceManager {
    public:
      static Texture2D getTexture(std::string texturePath);

    private:
      static TextureCache _textureCache;
  };

}