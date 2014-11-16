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

  this->window = SDLW::CreateWindow_("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
  this->context = SDLW::GLCreateContext(this->window);

  SDLW::GLEWInit();
  SDLW::GLSetSwapInterval(1);

  this->programId = Shader::Load("Shaders/Vertex.shader", "Shaders/Fragment.shader");

  GLW::Enable(GL_BLEND);
  GLW::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GLW::ClearColor(1.f, 1.f, 1.f, 1.f);

  this->textureAtlas = new Texture("Textures/grass_tile.png");
  this->models = new modelMap();
  this->models->insert(std::pair<string, Model*>("Grass", new Grass()));
  this->VBO = new GLuint[this->models->size()];
  this->UBO = new GLuint[this->models->size()];

  int k = 0;
  for (modelMap::iterator it = this->models->begin(); it != this->models->end(); ++it) {

    GLW::GenBuffers(1, &this->VBO[k]);
    GLW::BindBuffer(GL_ARRAY_BUFFER, this->VBO[k]);
    GLW::BufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), this->models->at("Grass")->getVertices(), GL_STATIC_DRAW);

    GLW::GenBuffers(1, &this->UBO[k]);
    GLW::BindBuffer(GL_ARRAY_BUFFER, this->UBO[k]);
    GLW::BufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), this->models->at("Grass")->getUV(), GL_STATIC_DRAW);
  }

	this->matrixId = glGetUniformLocation(this->programId, "MVP");

	glm::mat4 Projection = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f);
	this->MVP = (Projection) * glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f));

	this->textureId = glGetUniformLocation(this->programId, "textureSampler");
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

  glUniformMatrix4fv(this->matrixId, 1, GL_FALSE, &this->MVP[0][0]);
  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, this->textureAtlas->GetId());
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  glUniform1i(this->textureId, 0);

  GLW::EnableVertexAttribArray(0);
  GLW::EnableVertexAttribArray(1);

  GLW::BindBuffer(GL_ARRAY_BUFFER, this->VBO[0]);
  GLW::VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, this->UBO[0]);
  glVertexAttribPointer(
    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    2,                                // size : U+V => 2
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
  );

  glDrawArrays(GL_QUADS, 0, 4);

  GLW::DisableVertexAttribArray(0);
  GLW::DisableVertexAttribArray(1);

  glm::mat4 newMVP = this->MVP * glm::translate(glm::mat4(1.0f), glm::vec3(64.0f, 32.0f, 0.0f));

  glUniformMatrix4fv(this->matrixId, 1, GL_FALSE, &newMVP[0][0]);

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, this->textureAtlas->GetId());
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  glUniform1i(this->textureId, 0);

  GLW::EnableVertexAttribArray(0);
  GLW::EnableVertexAttribArray(1);

  GLW::BindBuffer(GL_ARRAY_BUFFER, this->VBO[0]);
  GLW::VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, this->UBO[0]);
  glVertexAttribPointer(
    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    2,                                // size : U+V => 2
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
  );

  glDrawArrays(GL_QUADS, 0, 4);

  GLW::DisableVertexAttribArray(0);
  GLW::DisableVertexAttribArray(1);

  GLW::UseProgram(0);
}

void Renderer::SwapWindow() {
  SDLW::GLSwapWindow(this->window);
}
