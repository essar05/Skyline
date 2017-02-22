#pragma once
#include <GL/glew.h>
#include <glm\glm.hpp>

namespace Ess2D {

  struct Position {
    float x;
    float y;
  };

  struct ColorRGBA8 {
    ColorRGBA8() : r(0), g(0), b(0), a(255) {}
    ColorRGBA8(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : r(r), g(g), b(b), a(a) {}
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
  };

  struct UV {
    float u;
    float v;
  };

  struct Vertex {
    Position position;
    ColorRGBA8 color;
    UV uv;

    void setPosition(float x, float y) {
      position.x = x;
      position.y = y;
    }

    void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
      color.r = r;
      color.g = g;
      color.b = b;
      color.a = a;
    }

    void setUV(float u, float v) {
      uv.u = u;
      uv.v = v;
    }

  };

  enum class GlyphSortType {
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
  };

  class Glyph {
    public:
    Glyph() {}
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint TextureId, const ColorRGBA8& color, float ZDepth) : textureId(TextureId), zDepth(ZDepth) {
      topLeft.color = color;
      topLeft.setPosition(destRect.x, destRect.y + destRect.w);
      topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

      bottomLeft.color = color;
      bottomLeft.setPosition(destRect.x, destRect.y);
      bottomLeft.setUV(uvRect.x, uvRect.y);

      bottomRight.color = color;
      bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
      bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

      topRight.color = color;
      topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
      topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
    }

    GLuint textureId;
    float zDepth;

    Vertex topLeft;
    Vertex bottomLeft;
    Vertex topRight;
    Vertex bottomRight;
  };

}

