#ifndef RENDERER_H
#define RENDERER_H

#include "SDLW.h"
#include "GLW.h"
#include "Texture.h"
#include "Model.h"
#include "Grass.h"
#include "Game.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace Engine;
using namespace Terrain;

typedef map<int, Model*> modelMap;
typedef map<int, GLuint> buffersMap;

struct character_info {
  float ax; // advance.x
  float ay; // advance.y

  float bw; // bitmap.width;
  float bh; // bitmap.rows;

  float bl; // bitmap_left;
  float bt; // bitmap_top;

  float tx; // x offset of glyph in texture coordinates
};

namespace Engine {
  class Game;
  class Renderer {
    public:
      Renderer(Game* aGameInstance);
      virtual ~Renderer();
      void Render();
      void SwapWindow();
	  GLuint GetWidth();
	  GLuint GetHeight();
    void RenderText(char aChar, float x, float y);
    void RenderString(const char* aString, float x, float y);

    protected:
    private:
      Game* game;
      Texture* textureAtlas;
      modelMap* models;
	    buffersMap* buffers;
      GLuint* UBO;
      GLuint* VBO;
      bool renderQuad = true;
      GLint vertexPos2DLocation = -1;
      SDL_GLContext context;

      GLuint mainProgramId = 0;
      GLuint fontProgramId = 0;

      SDL_Window* window = NULL;
      GLuint textureId;
      GLuint matrixId;
      glm::mat4 MVP;
      GLuint screenWidth;
      GLuint screenHeight;
      
      GLuint fontColorLocation = 0;
      GLuint fontTextureLocation = 0;
      GLuint fontMVPLocation = 0;
      FT_Library ft;
      GLuint fontVBO;
      FT_GlyphSlot glyph;
      GLuint fontTextureId;
      FT_Face face;
      character_info chars[128];
      GLuint fontAtlasWidth = 0;
      GLuint fontAtlasHeight = 0;
  };
};

#endif // RENDERER_H
