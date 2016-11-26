#include "TextureCache.h"
#include "IOManager.h"

namespace Essengine {

  TextureCache::TextureCache() {}

  TextureCache::~TextureCache() {
    auto it = _atlases.begin();
    while (it != _atlases.end()) {
      delete it->second;
      it++;
    }
  }

  GLTexture TextureCache::getTexture(std::string texturePath) {
    auto it = _textures.find(texturePath);
    
    if(it == _textures.end()) {
      GLTexture texture = Essengine::IOManager::loadTextureFromImage(texturePath);

      _textures.insert(make_pair(texturePath, texture));

      return texture;
    }

    return it->second;
  }

  TextureAtlas* TextureCache::getAtlas(std::string texturePath, std::string metadataPath) {
    std::string key = texturePath + "_" + metadataPath;
    auto it = _atlases.find(key);

    if (it == _atlases.end()) {
      TextureAtlas* atlas = new TextureAtlas(this, texturePath, metadataPath);

      _atlases.insert(std::make_pair(key, atlas));

      return atlas;
    }

    return it->second;
  }

}