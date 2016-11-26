#include "TextureAtlas.h"
#include <rapidjson.h>
#include <fstream>
#include <document.h>
#include "Errors\RuntimeException.cpp"

namespace Essengine {

  TextureAtlas::TextureAtlas(TextureCache* textureCache, std::string texturePath, std::string metadataPath) {
    GLTexture texture = textureCache->getTexture(texturePath);
    this->_textureWidth = texture._width;
    this->_textureHeight = texture._height;
    this->_textureId = texture._id;

    this->loadMetadata(metadataPath);
  }

  TextureAtlas::~TextureAtlas() {
    auto it = _frames.begin();
    while (it != _frames.end()) {
      delete it->second;
      it++;
    }
  }

  glm::vec4 TextureAtlas::getUV(std::string frameName) {
    auto it = _frames.find(frameName);
    if (it != _frames.end()) {
      return it->second->_uv;
    }

    return glm::vec4(0.0f);
  }

  void TextureAtlas::loadMetadata(std::string metadataPath) {
    std::ifstream fileStream(metadataPath);
    std::string jsonData;

    if (!fileStream.is_open()) {
      throw Essengine::ERuntimeException("Failed to open " + metadataPath);
    }

    fileStream.seekg(0, std::ios::end);
    jsonData.reserve((unsigned int) fileStream.tellg());
    fileStream.seekg(0, std::ios::beg);

    jsonData.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    rapidjson::Document document;
    document.Parse(jsonData.c_str());

    if (!document.IsArray()) {
      throw Essengine::ERuntimeException("Texture atlas metadata file invalid.");
    }

    for (rapidjson::SizeType i = 0; i < document.Size(); i++) {
      std::string frameName = document[i]["name"].GetString();
      
      TextureAtlasFrame* frame = new TextureAtlasFrame();
      frame->_height = document[i]["height"].GetInt();
      frame->_width = document[i]["width"].GetInt();
      frame->_x = document[i]["x"].GetInt();
      frame->_y = document[i]["y"].GetInt();
      frame->_name = frameName;
      frame->_uv = this->generateFrameUV(frame);

      _frames.insert(std::pair<std::string, TextureAtlasFrame*>(frameName, frame));
    }

  }

  glm::vec4 TextureAtlas::generateFrameUV(TextureAtlasFrame* frame) {
    glm::vec4 uv;
    uv.x = (float) frame->_x / _textureWidth;
    uv.y = 1.0f - (float) (frame->_y + frame->_height ) / _textureHeight + uv.w;
    uv.z = (float) frame->_width / _textureWidth;
    uv.w = (float) frame->_height / _textureHeight;
    
    return uv;
  }

}