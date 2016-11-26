#pragma once
#include <string>
#include "TextureCache.h"
#include <glm\glm.hpp>
#include <unordered_map>

namespace Essengine {

  class TextureCache;

  class TextureAtlasFrame {
    public:
      std::string _name;
      int _x;
      int _y;
      int _width;
      int _height;
      glm::vec4 _uv;
  };

  class TextureAtlas {
    public:
      TextureAtlas(TextureCache* textureCache, std::string texturePath, std::string metadataPath);
      ~TextureAtlas();

      glm::vec4 getUV(std::string frameName);
      GLuint getTextureId() { return _textureId; }
      
    private:
      void loadMetadata(std::string metadataPath);
      glm::vec4 generateFrameUV(TextureAtlasFrame* frame);

      GLuint _textureId;
      unsigned int _textureWidth;
      unsigned int _textureHeight;
      
      std::unordered_map<std::string, TextureAtlasFrame*> _frames;

  };

};