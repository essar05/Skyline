#include "FBORenderer.h"
#include "Game.h"

FBORenderer::FBORenderer() {
  _shader = new Ess2D::Shader(false);

  _shader->loadShader(Ess2D::ShaderType::VERTEX, "Shaders/Vertex.shader");
  _shader->loadShader(Ess2D::ShaderType::FRAGMENT, "Shaders/Fragment.shader");
  _shader->compileShaders();

  _shader->addAttribute("vertexPosition");
  _shader->addAttribute("vertexColor");
  _shader->addAttribute("vertexUV");
  _shader->linkShaders();
}

FBORenderer::~FBORenderer() {
  delete _shader;
}

void FBORenderer::render(float imageWidth, float imageHeight, GLuint textureId) {
  Game* game = Game::GetInstance();

  _shader->use();

  glActiveTexture(GL_TEXTURE0);
  GLint textureLocation = _shader->getUniformLocation("textureSampler");
  glUniform1i(textureLocation, 0);

  GLint useTextureLocation = _shader->getUniformLocation("useTexture");
  glUniform1i(useTextureLocation, 1);

  GLint pLocation = _shader->getUniformLocation("P");
  glm::mat4 cameraMatrix = glm::ortho(0.0f, game->getWidth(), 0.0f, game->getHeight());

  glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

  game->getSpriteBatch()->begin(Ess2D::GlyphSortType::BACK_TO_FRONT);

  //UV Coords flipped, cause FBOs are flipped.
  glm::vec4 uv(0.0f, 1.0f, 1.0f, -1.0f);
  game->getSpriteBatch()->draw(glm::vec4(0, 0, imageWidth, imageHeight), uv, textureId, Ess2D::ColorRGBA8(225, 255, 255, 255), 0.0f);

  game->getSpriteBatch()->end();
  game->getSpriteBatch()->render();

  _shader->unuse();
}