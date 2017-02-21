#pragma once
#include <Shader.h>
#include "../FBORenderer.h"
#include <glew.h>
#include "../Game.h"

enum class CombineFilterBlendMode {
  ADD = 1,
  SCREEN = 2,
  LIGHTEN = 3
};

class CombineFilter : public FBORenderer {
  public:
    CombineFilter();
    CombineFilter(int downScaling);
    ~CombineFilter();

    virtual void initShader();
    virtual void render(Ess2D::FrameBufferObject* fbo);
    void render(Ess2D::FrameBufferObject* baseFBO, Ess2D::FrameBufferObject* highlightsFBO);
    Ess2D::FrameBufferObject* getResult();

    void setBlendMode(CombineFilterBlendMode blendMode);
    void setBloomIntensity(float bloomIntensity);

  protected:
    virtual void initVertexAttributeObject();

  private:
    Ess2D::FrameBufferObject* _targetFBO;
    Game* _game;
    CombineFilterBlendMode _blendMode = CombineFilterBlendMode::LIGHTEN;
    float _bloomIntensity = 1.0f;
};
