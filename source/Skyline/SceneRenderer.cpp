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
  game->getGameplayScreen()->getParticleManager()->draw();
  game->getGameplayScreen()->getPowerupManager()->draw();

  if(game->getGameplayScreen()->isPaused()) {
    Ess2D::Camera2D* camera = game->getGameplayScreen()->getMainCamera();

    float cameraPosX = camera->getPosition().x - camera->getWorldScalar(game->getWidth()) / 2;
    float cameraPosY = camera->getPosition().y - camera->getWorldScalar(game->getHeight()) / 2;

    game->getGameplayScreen()->getSpriteBatch()->draw(glm::vec4(cameraPosX, cameraPosY, camera->getWorldScalar(game->getWidth()), camera->getWorldScalar(game->getHeight())), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0, Ess2D::ColorRGBA8(0, 0, 0, 180), 15000, 0.0f);
  }

  game->getGameplayScreen()->getSpriteBatch()->end();
  game->getGameplayScreen()->getSpriteBatch()->render();

  glUniform1i(useTextureLocation, 0);
  game->getGameplayScreen()->getLevel()->getWorld()->DrawDebugData();

  _shader->unuse();
}