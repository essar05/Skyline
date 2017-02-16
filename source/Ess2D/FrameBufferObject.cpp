#include "FrameBufferObject.h"

namespace Ess2D {

  FrameBufferObject::FrameBufferObject(Window* window, GLsizei width, GLsizei height, DepthBufferType depthBufferType) {
    _width = width;
    _height = height;
    _window = window;

    createFrameBuffer();
    createColorTexture();
    if (depthBufferType == DepthBufferType::RENDER_BUFFER) {
      createDepthBuffer();
    } else if(depthBufferType == DepthBufferType::TEXTURE) {
      createDepthTexture();
    }
    
    unbind();
  }

  FrameBufferObject::~FrameBufferObject() {
    glDeleteFramebuffers(1, &_id);
    glDeleteTextures(1, &_colorTextureId);
    glDeleteTextures(1, &_depthTextureId);
    glDeleteRenderbuffers(1, &_depthBufferId);
  }

  void FrameBufferObject::bind() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);
    glViewport(0, 0, _width, _height);
  }

  void FrameBufferObject::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, (GLsizei) _window->GetWidth(), (GLsizei) _window->GetHeight());
  }

  void FrameBufferObject::bindToRead() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
  }

  GLuint FrameBufferObject::getColorTextureId() {
    return _colorTextureId;
  }

  GLuint FrameBufferObject::getDepthTextureId() {
    return _depthTextureId;
  }

  void FrameBufferObject::createFrameBuffer() {
    glGenFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
  }

  void FrameBufferObject::createColorTexture() {
    glGenTextures(1, &_colorTextureId);
    glBindTexture(GL_TEXTURE_2D, _colorTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTextureId, 0);
  }

  void FrameBufferObject::createDepthTexture() {
    glGenTextures(1, &_depthTextureId);
    glBindTexture(GL_TEXTURE_2D, _depthTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTextureId, 0);
  }

  void FrameBufferObject::createDepthBuffer() {
    glGenRenderbuffers(1, &_depthBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBufferId);
  }

}