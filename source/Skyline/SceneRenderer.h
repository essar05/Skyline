#pragma once
#include <Shader.h>

class SceneRenderer {
  public:
    SceneRenderer();
    ~SceneRenderer();

    void render();
  private:
    Ess2D::Shader* _shader;

};

