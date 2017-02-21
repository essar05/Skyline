#include "CombineFilter.h"

CombineFilter::CombineFilter() : CombineFilter(1) {}

CombineFilter::CombineFilter(int downScaling) {
  _game = Game::GetInstance();
  _targetFBO = new Ess2D::FrameBufferObject(_game->getWindow(), (GLsizei) (_game->getWidth() / downScaling), (GLsizei) (_game->getHeight() / downScaling), Ess2D::DepthBufferType::RENDER_BUFFER);
}

CombineFilter::~CombineFilter() {}

void CombineFilter::initShader() {
  _shader = new Ess2D::Shader(false);

  _shader->loadShader(Ess2D::ShaderType::VERTEX, "Shaders/FilterV.shader");
  _shader->loadShader(Ess2D::ShaderType::FRAGMENT, "Shaders/CombineFilterF.shader");
  _shader->compileShaders();

  _shader->addAttribute("vertexPosition");
  _shader->linkShaders();

  initVertexAttributeObject();
}

void CombineFilter::initVertexAttributeObject() {
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

void CombineFilter::render(Ess2D::FrameBufferObject* fbo) {}

void CombineFilter::render(Ess2D::FrameBufferObject* baseFBO, Ess2D::FrameBufferObject* highlightsFBO) {
  _targetFBO->bind();
  _shader->use();

  glActiveTexture(GL_TEXTURE0);
  GLint baseTextureLocation = _shader->getUniformLocation("baseTexture");
  glUniform1i(baseTextureLocation, 0);

  glActiveTexture(GL_TEXTURE1);
  GLint highlightsTextureLocation = _shader->getUniformLocation("highlightsTexture");
  glUniform1i(highlightsTextureLocation, 1);

  GLint blendModeLocation = _shader->getUniformLocation("blendMode");
  glUniform1i(blendModeLocation, (GLint) _blendMode);

  GLint highlightsIntensityLocation = _shader->getUniformLocation("highlightsIntensity");
  glUniform1f(highlightsIntensityLocation, _bloomIntensity);

  glBindVertexArray(_vao);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, baseFBO->getColorTextureId());

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, highlightsFBO->getColorTextureId());

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);

  _shader->unuse();
  _targetFBO->unbind();
}

void CombineFilter::setBlendMode(CombineFilterBlendMode blendMode) {
  _blendMode = blendMode;
}

void CombineFilter::setBloomIntensity(float bloomIntensity) {
  _bloomIntensity = bloomIntensity;
}

Ess2D::FrameBufferObject* CombineFilter::getResult() {
  return _targetFBO;
}
