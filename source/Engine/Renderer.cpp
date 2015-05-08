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

  this->screenWidth = 640;
  this->screenHeight = 480;

  this->window = SDLW::CreateWindow_("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL);
  this->context = SDLW::GLCreateContext(this->window);

  SDLW::GLEWInit();
  SDLW::GLSetSwapInterval(1);

  this->programId = Shader::Load("Shaders/Vertex.shader", "Shaders/Fragment.shader");
  this->fontShaderId = Shader::Load("Shaders/FontVertex.shader", "Shaders/FontFragment.shader");

  GLW::Enable(GL_BLEND);
  GLW::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GLW::ClearColor(1.f, 1.f, 1.f, 1.f);

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

  this->matrixId = glGetUniformLocation(this->programId, "MVP");

  glm::mat4 Projection = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f);
  this->MVP = (Projection) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

  this->textureId = glGetUniformLocation(this->programId, "textureSampler");

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

  if (FT_Init_FreeType(&this->ft)) {
	  fprintf(stderr, "Could not init freetype library\n");
  }

  

  if (FT_New_Face(ft, "Candara.ttf", 0, &this->face)) {
	  fprintf(stderr, "Could not open font\n");
  }

  FT_Set_Pixel_Sizes(this->face, 0, 48);

  if (FT_Load_Char(this->face, 'X', FT_LOAD_RENDER)) {
	  fprintf(stderr, "Could not load character 'X'\n");
  }

  
  this->glyph = this->face->glyph;

  this->fontTextureLocation = glGetUniformLocation(this->fontShaderId, "textureSampler");
  this->fontColorLocation = glGetUniformLocation(this->fontShaderId, "fontColor");
  this->fontMVPLocation = glGetUniformLocation(this->fontShaderId, "MVP");

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &this->fontTextureId);
  glBindTexture(GL_TEXTURE_2D, this->fontTextureId);
  glUniform1i(this->fontTextureLocation, 0);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glGenBuffers(1, &this->fontVBO);
  
  //glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
}

Renderer::~Renderer() {
  GLW::DeleteProgram(this->programId);
  SDLW::DestroyWindow_(this->window);
  SDLW::Quit();
}

void Renderer::Render() {
	

  GLW::Clear(GL_COLOR_BUFFER_BIT);

  if(!this->renderQuad) {
    return;
  }

  GLW::UseProgram(this->programId);

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

  this->RenderText('a', 100, 100);
  this->RenderText('d', 150, 100);
  this->RenderText('v', 200, 100);
  this->RenderText('e', 250, 100);
  this->RenderText('r', 300, 100);
  this->RenderText('s', 350, 100);
  this->RenderText('u', 400, 100);
  this->RenderText('s', 450, 100);
  GLW::UseProgram(0);
 
}

void Renderer::RenderText(char aChar, float x, float y) {
  GLW::UseProgram(this->fontShaderId);

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
  glUniform1i(this->textureId, 0);

  
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

void Renderer::SwapWindow() {
  SDLW::GLSwapWindow(this->window);
}

GLuint Renderer::GetWidth() {
  return this->screenWidth;
}

GLuint Renderer::GetHeight() {
  return this->screenHeight;
}