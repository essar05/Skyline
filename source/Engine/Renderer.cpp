#include "Renderer.h"

Renderer::Renderer(Game* aGameInstance) {
  this->game = aGameInstance;

  SDLW::Init(SDL_INIT_VIDEO);

  /*SDL_DisplayMode* current = new SDL_DisplayMode;

  int should_be_zero = SDL_GetCurrentDisplayMode(0, current);

  if(should_be_zero != 0)
    // In case of error...
    SDL_Log("Could not get display mode for video display #%d: %s", 0, SDL_GetError());

  else
    // On success, print the current display mode.
    SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", 0, current->w, current->h, current->refresh_rate);
  */

  SDLW::SetGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDLW::SetGLAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDLW::SetGLAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  //fixed resolution for now.
  this->screenWidth = 640;
  this->screenHeight = 480;

  this->window = SDLW::CreateWindow_("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL);
  this->context = SDLW::GLCreateContext(this->window);

  SDLW::GLEWInit();
  SDLW::GLSetSwapInterval(1);

  //load up the shaders and store their name/id;
  this->mainProgramId = Shader::Load("Shaders/Vertex.shader", "Shaders/Fragment.shader");
  this->fontProgramId = Shader::Load("Shaders/FontVertex.shader", "Shaders/FontFragment.shader");

  GLW::Enable(GL_BLEND);
  GLW::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GLW::ClearColor(1.f, 1.f, 1.f, 1.f);

  //Initialize drawing stuff.
  this->textureAtlas = new Texture("Textures/grass_tile.png");
  this->models = new modelMap();
  this->buffers = new buffersMap();
  this->models->insert(std::pair<int, Model*>(1, new Grass()));
  this->VBO = new GLuint[this->models->size()];
  this->UBO = new GLuint[this->models->size()];

  int k = 0;
  for (modelMap::iterator it = this->models->begin(); it != this->models->end(); ++it) {
	this->buffers->insert(std::pair<int, int>(it->first, k));
    GLW::GenBuffers(1, &this->VBO[k]);
    GLW::BindBuffer(GL_ARRAY_BUFFER, this->VBO[k]);
    GLW::BufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), this->models->at(1)->getVertices(), GL_STATIC_DRAW);

    GLW::GenBuffers(1, &this->UBO[k]);
    GLW::BindBuffer(GL_ARRAY_BUFFER, this->UBO[k]);
    GLW::BufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), this->models->at(1)->getUV(), GL_STATIC_DRAW);
  }

  //get the location of MVP from our main shader.
  this->matrixId = glGetUniformLocation(this->mainProgramId, "MVP");

  glm::mat4 Projection = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f);
  this->MVP = (Projection) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

  //texture sampler location from main fragmnet shader
  this->textureId = glGetUniformLocation(this->mainProgramId, "textureSampler");

  //at the momemnt it's not really used, but used to calculate the I and J to start from when drawing.
  for (int k = 0; k < this->game->GetMap()->GetLayersCount(); k++) {
	  for (int i = 0; i < this->game->GetMap()->GetOffsetHeight(); i++) {
		  for (int j = 0; j < this->game->GetMap()->GetOffsetWidth(); j++) {
			  int theOffsetI = this->game->GetMap()->GetOffsetI();
			  int theOffsetJ = this->game->GetMap()->GetOffsetJ();

			  printf("\ni: %d", i + theOffsetI);
			  printf("j: %d", j + theOffsetJ);
		  }
	  }
  }
  
  //Temporary font init code (will be moved)

  if (FT_Init_FreeType(&this->ft)) {
	  fprintf(stderr, "Could not init freetype library\n");
  }
  

  if (FT_New_Face(ft, "Candara.ttf", 0, &this->face)) {
	  fprintf(stderr, "Could not open font\n");
  }

  FT_Set_Pixel_Sizes(this->face, 0, 48);
  
  //Load up the ASCII characters 32 - 127 (95 chars)
  
  this->glyph = this->face->glyph;

  int w = 0;
  int h = 0;
  
  for (int i = 32; i < 128; i++) {
    if (FT_Load_Char(this->face, i, FT_LOAD_RENDER)) {
      fprintf(stderr, "Could not load character %c\n", i);
      continue;
    }

    w += this->glyph->bitmap.width;
    if (i == 65) {
      printf("%f \n", (float)w);
    }
    h = (h < this->glyph->bitmap.rows) ? this->glyph->bitmap.rows : h;
  }
  
  this->fontAtlasWidth = w;
  this->fontAtlasHeight = h;

  this->fontTextureLocation = glGetUniformLocation(this->fontProgramId, "textureSampler");
  this->fontColorLocation = glGetUniformLocation(this->fontProgramId, "fontColor");
  this->fontMVPLocation = glGetUniformLocation(this->fontProgramId, "MVP");

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &this->fontTextureId);
  glBindTexture(GL_TEXTURE_2D, this->fontTextureId);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

  int x = 0;

  for (int i = 32; i < 128; i++) {
    if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
      continue;
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, this->glyph->bitmap.width, this->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, this->glyph->bitmap.buffer);

    this->chars[i - 32].ax = this->glyph->advance.x >> 6;
    this->chars[i - 32].ay = this->glyph->advance.y >> 6;

    this->chars[i - 32].bw = this->glyph->bitmap.width;
    this->chars[i - 32].bh = this->glyph->bitmap.rows;

    this->chars[i - 32].bl = this->glyph->bitmap_left;
    this->chars[i - 32].bt = this->glyph->bitmap_top;

    this->chars[i - 32].tx = (float) x / w;

    x += this->glyph->bitmap.width;
  }

  glGenBuffers(1, &this->fontVBO);
}

Renderer::~Renderer() {
  GLW::DeleteProgram(this->mainProgramId);
  GLW::DeleteProgram(this->fontProgramId);
  SDLW::DestroyWindow_(this->window);
  SDLW::Quit();
}

void Renderer::Render() {
	

  GLW::Clear(GL_COLOR_BUFFER_BIT);

  if(!this->renderQuad) {
    return;
  }

  GLW::UseProgram(this->mainProgramId);

  glm::mat4 Projection = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f);
  this->MVP = (Projection)* glm::translate(glm::mat4(1.0f), glm::vec3(this->game->GetViewportOffsetX(), this->game->GetViewportOffsetY(), 0.0f));

  glUniformMatrix4fv(this->matrixId, 1, GL_FALSE, &this->MVP[0][0]);
  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, this->textureAtlas->GetId());
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  glUniform1i(this->textureId, 0);

  GLW::EnableVertexAttribArray(0);
  GLW::EnableVertexAttribArray(1);

  glm::mat4 theMVP;

  for (int k = 0; k < this->game->GetMap()->GetLayersCount(); k++) {
	  for (int i = 0; i < this->game->GetMap()->GetHeight(); i++) {
		  for (int j = 0; j < this->game->GetMap()->GetWidth(); j++) {
			  
			  Tile* theTile = this->game->GetMap()->Tiles[k][i][j];


			  int theTileId = theTile->GetId();
			  int theBufferIndex = this->buffers->at(theTileId);

			  GLW::BindBuffer(GL_ARRAY_BUFFER, this->VBO[theBufferIndex]);
			  GLW::VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

			  glBindBuffer(GL_ARRAY_BUFFER, this->UBO[theBufferIndex]);
			  glVertexAttribPointer(
				  1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				  2,                                // size : U+V => 2
				  GL_FLOAT,                         // type
				  GL_FALSE,                         // normalized?
				  0,                                // stride
				  (void*)0                          // array buffer offset
			  );

			  theMVP = this->MVP * glm::translate(glm::mat4(1.0f), glm::vec3(theTile->GetX(), theTile->GetY(), 0.0f));

			  glUniformMatrix4fv(this->matrixId, 1, GL_FALSE, &theMVP[0][0]);

			  glDrawArrays(GL_QUADS, 0, 4);
		  }
	  }
  }

  GLW::DisableVertexAttribArray(0);
  GLW::DisableVertexAttribArray(1);

  this->RenderString("ASDADASD !", 100, 100);
  GLW::UseProgram(0);
}

void Renderer::RenderText(char aChar, float x, float y) {
  GLW::UseProgram(this->fontProgramId);

  GLW::EnableVertexAttribArray(0);
  glm::mat4 theMVP = this->MVP;

  if (FT_Load_Char(this->face, aChar, FT_LOAD_RENDER)) {
    fprintf(stderr, "Could not load character 'X'\n");
  }

  glUniformMatrix4fv(this->fontMVPLocation, 1, GL_FALSE, &theMVP[0][0]);

  GLfloat black[4] = { 1, 1, 1, 1 };
  glUniform4fv(this->fontColorLocation, 1, black);

  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, this->fontTextureId);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RED,
    this->glyph->bitmap.width,
    this->glyph->bitmap.rows,
    0,
    GL_RED,
    GL_UNSIGNED_BYTE,
    this->glyph->bitmap.buffer
    );
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  glUniform1i(this->fontTextureLocation, 0);

  
  /*
  x2, -y2                    x2 + w, - y2
  0, 0                       1,0

  x2, -y2 + h                x2 + w, -y2 + h
  0,1                        1,1
  */

  float x2 = x + this->glyph->bitmap_left;
  float y2 = y - this->glyph->bitmap_top;
  float w = this->glyph->bitmap.width;
  float h = this->glyph->bitmap.rows;
  GLfloat box[4][4] = {
    { x2, y2, 0, 0 },
    { x2 + w, y2, 1, 0 },
    { x2, y2 + h, 0,  1 },
    { x2 + w, y2 + h, 1, 1 }
  };

  GLW::BindBuffer(GL_ARRAY_BUFFER, this->fontVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
  GLW::VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  GLW::DisableVertexAttribArray(0);

}

void Renderer::RenderString(const char* aString, float x, float y) {
  GLW::UseProgram(this->fontProgramId);
  GLW::EnableVertexAttribArray(0);

  glm::mat4 theMVP = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(this->fontMVPLocation, 1, GL_FALSE, &theMVP[0][0]);

  GLfloat black[4] = { 0.4, 0.5, 1, 1 };
  glUniform4fv(this->fontColorLocation, 1, black);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->fontTextureId);
  glUniform1i(this->fontTextureLocation, 0);

  int strl = strlen(aString) * 6;
  
  struct point {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
  };

  point* coords = new point[strl];

  int n = 0;
  
  for (int i = 0; i < strlen(aString); i++) {
    int p = (int) aString[i] - 32;

    float x2 = x + this->chars[p].bl;
    float y2 = y - this->chars[p].bt;
    float w = this->chars[p].bw;
    float h = this->chars[p].bh;

    x += this->chars[p].ax;
    y += this->chars[p].ay;

    if (!w || !h)
      continue;
    
    coords[n] = { x2, y2, this->chars[p].tx, 0 };
    n++;
    
    coords[n] = { x2 + w, y2, this->chars[p].tx + this->chars[p].bw / this->fontAtlasWidth, 0 };
    n++;

    coords[n] = { x2, y2 + h, this->chars[p].tx, this->chars[p].bh / this->fontAtlasHeight };
    n++;
    
    coords[n] = { x2 + w, y2, this->chars[p].tx + this->chars[p].bw / this->fontAtlasWidth, 0 };
    n++;
    
    coords[n] = { x2, y2 + h, this->chars[p].tx, this->chars[p].bh / this->fontAtlasHeight };
    n++;
    
    coords[n] = { x2 + w, y2 + h, this->chars[p].tx + this->chars[p].bw / this->fontAtlasWidth, this->chars[p].bh / this->fontAtlasHeight };
    n++;
  }
  
  GLW::BindBuffer(GL_ARRAY_BUFFER, this->fontVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof GLfloat * 4 * n, coords, GL_DYNAMIC_DRAW);
  GLW::VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, n);
  GLW::DisableVertexAttribArray(0);

}

void Renderer::SwapWindow() {
  SDLW::GLSwapWindow(this->window);
}

GLuint Renderer::GetWidth() {
  return this->screenWidth;
}

GLuint Renderer::GetHeight() {
  return this->screenHeight;
}