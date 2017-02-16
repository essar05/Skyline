#pragma once
#include <Renderer.h>
#include <Shader.h>

class FBORenderer {
  public:
    FBORenderer();
    ~FBORenderer();

    void render(float imageWidth, float imageHeight, GLuint textureId);
    
  private:
    Ess2D::Shader* _shader;
};

