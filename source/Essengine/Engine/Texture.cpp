#include "SDLW.h"
#include "GLW.h"
#include "Texture.h"

namespace Essengine {

  Texture::Texture(const char* aFilePath) {
    this->Load(aFilePath);
  }

  Texture::~Texture() {}

  GLuint Texture::GetId() {
    return this->id;
  }

  int Texture::GetHeight() {
    return this->height;
  }

  int Texture::GetWidth() {
    return this->height;
  }

  int Texture::GetMode() {
    return this->mode;
  }

  void Texture::Load(const char* aFilePath) {

    SDL_Surface* theSurface = SDLW::ImageLoad(aFilePath);
    if (theSurface->format->BytesPerPixel == 3) { // RGB 24bit
      this->mode = GL_RGB;
    }
    else if (theSurface->format->BytesPerPixel == 4) { // RGBA 32bit
      this->mode = GL_RGBA;
    }
    else {
      SDL_FreeSurface(theSurface);
      char* theError = new char[200];
      sprintf_s(theError, sizeof(theError) + 30, "Unknown image format: %s\n", aFilePath);
      throw ERuntimeException(theError);
    }

    this->width = theSurface->w;
    this->height = theSurface->h;

    GLW::GenTextures(1, &this->id);
    GLW::BindTexture(GL_TEXTURE_2D, this->id);
    GLW::TexImage2D(GL_TEXTURE_2D, 0, this->mode, this->width, this->height, 0, this->mode, GL_UNSIGNED_BYTE, theSurface->pixels);

    GLW::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLW::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDLW::FreeSurface(theSurface);
  }
}