#ifndef TEXTURE_H
#define TEXTURE_H

namespace Essengine {
  class Texture {
    public:
      Texture(const char* aFilePath);
      virtual ~Texture();
      GLuint GetId();
      int GetHeight();
      int GetWidth();
      int GetMode();
    protected:
      void Load(const char* aFilePath);
    private:
      GLuint id;
      int height;
      int width;
      int mode;
  };
};

#endif // TEXTURE_H
