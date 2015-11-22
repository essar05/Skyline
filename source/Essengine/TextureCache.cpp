#include "TextureCache.h"
#include "IOManager.h"

namespace Essengine {

  TextureCache::TextureCache() {
  }


  TextureCache::~TextureCache() {
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

}