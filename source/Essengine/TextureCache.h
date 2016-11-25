#pragma once
#include <map>
#include "GLTexture.h"
#include "TextureAtlas.h"

namespace Essengine {

  class TextureAtlas;

  class TextureCache {
    public:
      TextureCache();
      ~TextureCache();

      GLTexture getTexture(std::string texturePath);
      TextureAtlas* getAtlas(std::string texturePath, std::string metadataPath);

    private:
      std::map<std::string, Essengine::GLTexture> _textures;
      std::map<std::string, Essengine::TextureAtlas*> _atlases;

  };

}