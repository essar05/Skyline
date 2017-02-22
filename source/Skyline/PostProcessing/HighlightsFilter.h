#pragma once
#include <Shader.h>
#include "../FBORenderer.h"
#include <GL/glew.h>
#include "../Game.h"

class HighlightsFilter : public FBORenderer {
  public:
    HighlightsFilter();
    HighlightsFilter(float downScaling);
    ~HighlightsFilter();

    virtual void initShader();
    virtual void render(Ess2D::FrameBufferObject* fbo);
    Ess2D::FrameBufferObject* getResult();

  protected:
    virtual void initVertexAttributeObject();

  private:
    Ess2D::FrameBufferObject* _targetFBO;
    Game* _game;

};
