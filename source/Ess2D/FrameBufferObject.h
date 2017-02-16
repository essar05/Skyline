#pragma once
#include <glew.h>
#include "Window.h"

namespace Ess2D {

  enum class DepthBufferType {
    NONE = 0,
    RENDER_BUFFER = 1,
    TEXTURE = 2
  };

  class FrameBufferObject {
    public:
      FrameBufferObject(Window* window, GLsizei width, GLsizei height, DepthBufferType depthBufferType);
      ~FrameBufferObject();

      void bind();
      void unbind();
      void bindToRead();

      GLuint getColorTextureId();
      GLuint getDepthTextureId();

    private:
      void createFrameBuffer();
      void createColorTexture();
      void createDepthTexture();
      void createDepthBuffer();

      Window* _window;
      GLsizei _width;
      GLsizei _height;

      GLuint _id;

      GLuint _colorTextureId;
      GLuint _depthTextureId;

      GLuint _colorBufferId;
      GLuint _depthBufferId;

  };

}