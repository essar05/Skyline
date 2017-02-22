#pragma once
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <vector>
#include "Vertex.h"

namespace Ess2D {

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