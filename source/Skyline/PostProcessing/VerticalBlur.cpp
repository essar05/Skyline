#include "VerticalBlur.h"

VerticalBlur::VerticalBlur() : VerticalBlur(1) {}

VerticalBlur::VerticalBlur(int downScaling) {
  _game = Game::GetInstance();
  _targetFBO = new Ess2D::FrameBufferObject(_game->getWindow(), _game->getWidth() / downScaling, _game->getHeight() / downScaling, Ess2D::DepthBufferType::RENDER_BUFFER);
}

VerticalBlur::~VerticalBlur() {}

void VerticalBlur::initShader() {
  _shader = new Ess2D::Shader(false);

  _shader->loadShader(Ess2D::ShaderType::VERTEX, "Shaders/VerticalBlurV.shader");
  _shader->loadShader(Ess2D::ShaderType::FRAGMENT, "Shaders/BlurF.shader");
  _shader->compileShaders();

  _shader->addAttribute("vertexPosition");
  _shader->linkShaders();

  initVertexAttributeObject();
}

void VerticalBlur::initVertexAttributeObject() {
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  GLfloat vertices[] = {
    -1.0f, 1.0f,
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f,
    -1.0f, 1.0f
  };
  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
}

void VerticalBlur::render(Ess2D::FrameBufferObject* fbo) {
  _targetFBO->bind();
  _shader->use();

  glActiveTexture(GL_TEXTURE0);
  GLint textureLocation = _shader->getUniformLocation("textureSampler");
  glUniform1i(textureLocation, 0);

  GLint screenWidthLocation = _shader->getUniformLocation("screenWidth");
  glUniform1f(screenWidthLocation, _targetFBO->getWidth());

  glBindVertexArray(_vao);

  glBindTexture(GL_TEXTURE_2D, fbo->getColorTextureId());
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);

  _shader->unuse();
  _targetFBO->unbind();
}

Ess2D::FrameBufferObject* VerticalBlur::getResult() {
  return _targetFBO;
}
