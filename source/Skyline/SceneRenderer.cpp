#include "SceneRenderer.h"
#include "Game.h"

SceneRenderer::SceneRenderer() {
  _shader = new Ess2D::Shader(true);

  _shader->loadShader(Ess2D::ShaderType::VERTEX, "Shaders/Vertex.shader");
  _shader->loadShader(Ess2D::ShaderType::FRAGMENT, "Shaders/Fragment.shader");
  _shader->compileShaders();

  _shader->addAttribute("vertexPosition");
  _shader->addAttribute("vertexColor");
  _shader->addAttribute("vertexUV");
  _shader->linkShaders();
}

SceneRenderer::~SceneRenderer() {
  delete _shader;
}

void SceneRenderer::render() {
  Game* game = Game::GetInstance();

  _shader->use();

  glActiveTexture(GL_TEXTURE0);
  GLint textureLocation = _shader->getUniformLocation("textureSampler");
  glUniform1i(textureLocation, 0);

  GLint useTextureLocation = _shader->getUniformLocation("useTexture");
  glUniform1i(useTextureLocation, 1);

  GLint pLocation = _shader->getUniformLocation("P");
  glm::mat4 cameraMatrix = game->getGameplayScreen()->getMainCamera()->getCameraMatrix();

  glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

  game->getGameplayScreen()->getSpriteBatch()->begin(Ess2D::GlyphSortType::BACK_TO_FRONT);

  game->getGameplayScreen()->getLevel()->draw();
  game->getGameplayScreen()->getProjectileManager()->draw();

  game->getGameplayScreen()->getSpriteBatch()->end();
  game->getGameplayScreen()->getSpriteBatch()->render();

  glUniform1i(useTextureLocation, 0);
  game->getGameplayScreen()->getLevel()->getWorld()->DrawDebugData();

  _shader->unuse();
}