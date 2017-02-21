#include "FBORenderer.h"
#include <Vertex.h>
#include <vector>
#include <glm\glm.hpp>

FBORenderer::FBORenderer() {
}

FBORenderer::~FBORenderer() {
  delete _shader;
}

void FBORenderer::initShader() {
  _shader = new Ess2D::Shader(false);

  _shader->loadShader(Ess2D::ShaderType::VERTEX, "Shaders/FBOV.shader");
  _shader->loadShader(Ess2D::ShaderType::FRAGMENT, "Shaders/FBOF.shader");
  _shader->compileShaders();

  _shader->addAttribute("vertexPosition");
  _shader->addAttribute("vertexColor");
  _shader->addAttribute("vertexUV");
  _shader->linkShaders();

  initVertexAttributeObject();
}

void FBORenderer::initVertexAttributeObject() {
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Ess2D::Vertex), (void*) offsetof(Ess2D::Vertex, position));
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Ess2D::Vertex), (void*) offsetof(Ess2D::Vertex, color));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Ess2D::Vertex), (void*) offsetof(Ess2D::Vertex, uv));

  glBindVertexArray(0);
}

void FBORenderer::drawFullscreenQuad(GLuint textureId) {
  glActiveTexture(GL_TEXTURE0);
  GLint textureLocation = _shader->getUniformLocation("textureSampler");
  glUniform1i(textureLocation, 0);

  Ess2D::Glyph glyph(glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), glm::vec4(0.0f, 1.0f, 1.0f, -1.0f), textureId, Ess2D::ColorRGBA8(225, 255, 255, 255), 0.0f);
  std::vector<Ess2D::Vertex> vertices;
  vertices.resize(6);
  vertices[0] = glyph.topLeft;
  vertices[1] = glyph.bottomLeft;
  vertices[2] = glyph.bottomRight;
  vertices[3] = glyph.bottomRight;
  vertices[4] = glyph.topRight;
  vertices[5] = glyph.topLeft;

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Ess2D::Vertex), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(_vao);

  glBindTexture(GL_TEXTURE_2D, glyph.textureId);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
}

void FBORenderer::render(Ess2D::FrameBufferObject* fbo) {
  _shader->use();

  drawFullscreenQuad(fbo->getColorTextureId());

  _shader->unuse();
}