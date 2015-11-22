#pragma once
#include <map>
#include "GLTexture.h"

namespace Essengine {

  class TextureCache {
    public:
      TextureCache();
      ~TextureCache();

      GLTexture getTexture(std::string texturePath);

    private:
      std::map<std::string, Essengine::GLTexture> _textures;

  };

}