#include "SpriteBatch.h"
#include <algorithm>

namespace Essengine {

  SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) {}

  SpriteBatch::~SpriteBatch() {}

  void SpriteBatch::init() {
    createVertexArray();
  }

  void SpriteBatch::begin(GlyphSortType sortType) {
    _sortType = sortType;
  }

  void SpriteBatch::end() {
    sortGlyphs();
    createRenderBatches();
  }

  //pass by reference for optimization and const to make sure they're not changed;
  void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint textureId, const ColorRGBA8& color, float zDepth) {
    Glyph* glyph = new Glyph;

    glyph->textureId = textureId;
    glyph->zDepth = zDepth;

    glyph->topLeft.color = color;
    glyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
    glyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    glyph->bottomLeft.color = color;
    glyph->bottomLeft.setPosition(destRect.x, destRect.y);
    glyph->bottomLeft.setUV(uvRect.x, uvRect.y);

    glyph->bottomRight.color = color;
    glyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
    glyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    glyph->topRight.color = color;
    glyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
    glyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

    _glyphs.push_back(glyph);
  }

  void SpriteBatch::render() {
    glBindVertexArray(_vao);

    for(unsigned int i = 0; i < _renderBatches.size(); i++) {
      glBindTexture(GL_TEXTURE_2D, _renderBatches[i].textureId);

      glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
    }
  
    glBindVertexArray(0);

    _renderBatches.clear();
    for(int i = 0; i < _glyphs.size(); i++) {
      delete _glyphs[i];
    }
    _glyphs.clear();
  }

  void SpriteBatch::createVertexArray() {
    if(_vao == 0) {
      glGenVertexArrays(1, &_vao);
    }
    glBindVertexArray(_vao);

    if(_vbo == 0) {
      glGenBuffers(1, &_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    //enabling the attribute array. not sure if need to do this since we do that in GLprogram::use ??
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //position attribute pointer.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    //color attribute pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*) offsetof(Vertex, color));
    //texture uv coordinates pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));

    glBindVertexArray(0);

  }

  void SpriteBatch::createRenderBatches() {
    std::vector<Vertex> vertices;
    vertices.resize(_glyphs.size() * 6);

    if(_glyphs.empty()) {
      return;
    }

    int k = 0;
    int offset = 0;

    //setup the first batch just to avoid more ifs inside a for. isntead we'll start the for from the 1 index.
    _renderBatches.emplace_back(offset, 6, _glyphs[0]->textureId);
    
    //add the vertices of the first glyph in our vector
    vertices[k++] = _glyphs[0]->topLeft;
    vertices[k++] = _glyphs[0]->bottomLeft;
    vertices[k++] = _glyphs[0]->bottomRight;
    vertices[k++] = _glyphs[0]->bottomRight;
    vertices[k++] = _glyphs[0]->topRight;
    vertices[k++] = _glyphs[0]->topLeft;
    offset += 6;

    for(int i = 1; i < _glyphs.size(); i++) {
      //only create a new batch if the texture of the glyph is different.
      if(_glyphs[i]->textureId != _glyphs[i - 1]->textureId) {
        _renderBatches.emplace_back(offset, 6, _glyphs[i]->textureId);
      } else {
        _renderBatches.back().numVertices += 6;
      }
      
      //add the vertices of glyph i in our vector
      vertices[k++] = _glyphs[i]->topLeft;
      vertices[k++] = _glyphs[i]->bottomLeft;
      vertices[k++] = _glyphs[i]->bottomRight;
      vertices[k++] = _glyphs[i]->bottomRight;
      vertices[k++] = _glyphs[i]->topRight;
      vertices[k++] = _glyphs[i]->topLeft;
      offset += 6;
    }

    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    //clear the buffer. it's faster than just uploadingh directly.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    //upload the vertices data to the vertex buffer object.
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void SpriteBatch::sortGlyphs() {
    switch(_sortType) {
      case GlyphSortType::BACK_TO_FRONT:
        std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
        break;
      case GlyphSortType::FRONT_TO_BACK:
        std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
        break;
      case GlyphSortType::TEXTURE:
        std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
        break;
    }
  }

  bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
    return (a->zDepth < b->zDepth);
  }

  bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
    return (a->zDepth > b->zDepth);
  }

  bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
    return (a->textureId < b->textureId);
  }

}