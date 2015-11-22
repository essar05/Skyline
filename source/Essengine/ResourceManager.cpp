#include "ResourceManager.h"

namespace Essengine {

  TextureCache ResourceManager::_textureCache;

  GLTexture ResourceManager::getTexture(std::string texturePath) {
    return _textureCache.getTexture(texturePath);
  }

}