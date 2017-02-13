#pragma once
#include <glew.h>
#include <glm\glm.hpp>
#include <vector>
#include "Vertex.h"

namespace Ess2D {

  enum class GlyphSortType {
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
  };

  class Glyph {
    public:
      Glyph() {}
      Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint TextureId, const ColorRGBA8& color, float ZDepth) : textureId(TextureId), zDepth (ZDepth) {
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

  class RenderBatch {
    public:
      RenderBatch(GLuint offset, GLuint numVertices, GLuint textureId) {
        this->offset = offset;
        this->numVertices = numVertices;
        this->textureId = textureId;
      }
      GLuint offset;
      GLuint numVertices;
      GLuint textureId;
  };

  class SpriteBatch {
    public:
      SpriteBatch();
      ~SpriteBatch();
    
      void init();

      void begin(GlyphSortType sortType = GlyphSortType::TEXTURE); 
      void end();

      //pass by reference for optimization and const to make sure they're not changed;
      void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint textureId, const ColorRGBA8& color, float zDepth);

      void render();

    private:
      void createVertexArray();
      void sortGlyphs();
      void createRenderBatches();
      static bool compareBackToFront(Glyph* a, Glyph* b);
      static bool compareFrontToBack(Glyph* a, Glyph* b);
      static bool compareTexture(Glyph* a, Glyph* b);

      GLuint _vbo;
      GLuint _vao;

      GlyphSortType _sortType;

      std::vector<Glyph> _glyphs; //actual glyphs
      std::vector<Glyph*> _glyphPointers; //for sorting
      std::vector<RenderBatch> _renderBatches;


  };

}