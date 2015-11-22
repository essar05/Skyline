#include "IOManager.h"
#include <glew.h>
#include <SDL_image.h>
#include "Errors\RuntimeException.cpp"

namespace Essengine {

  GLTexture IOManager::loadTextureFromImage(std::string filePath) {
    GLTexture texture = {};

    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
      throw ERuntimeException("Error loading image: " + filePath);
    }
    
    if (surface->format->BytesPerPixel == 3) { // RGB 24bit
      texture._mode = GL_RGB;
    } else if (surface->format->BytesPerPixel == 4) { // RGBA 32bit
      texture._mode = GL_RGBA;
    } else {
      SDL_FreeSurface(surface);
      throw ERuntimeException("Unknown image format: " + filePath);
    }
    
    texture._width = surface->w;
    texture._height = surface->h;
    
    glGenTextures(1, &(texture._id));

    glBindTexture(GL_TEXTURE_2D, texture._id);
    glTexImage2D(GL_TEXTURE_2D, 0, texture._mode, texture._width, texture._height, 0, texture._mode, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(surface);
    
    return texture;
  }

}