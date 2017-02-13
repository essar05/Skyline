#pragma once
#include <map>
#include "Texture2D.h"
#include "TextureAtlas.h"

namespace Ess2D {

  class TextureAtlas;

  class TextureCache {
    public:
      TextureCache();
      ~TextureCache();

      Texture2D getTexture(std::string texturePath);
      TextureAtlas* getAtlas(std::string texturePath, std::string metadataPath);

    private:
      std::map<std::string, Ess2D::Texture2D> _textures;
      std::map<std::string, Ess2D::TextureAtlas*> _atlases;

  };

}