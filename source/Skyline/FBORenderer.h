#pragma once
#include <Shader.h>
#include <FrameBufferObject.h>

class FBORenderer {
  public:
    FBORenderer();
    ~FBORenderer();

    virtual void render(Ess2D::FrameBufferObject* fbo);
    virtual void initShader();
    void drawFullscreenQuad(GLuint textureId);
    
  protected:
    virtual void initVertexAttributeObject();

    Ess2D::Shader* _shader;
    GLuint _vao;
    GLuint _vbo;

};

