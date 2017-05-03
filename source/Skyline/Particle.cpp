#include "Particle.h"
#include "Game.h"
#include <cstdlib>

Particle::Particle(std::string type, float x, float y, float width, float height, float depth, float playbackRate) : _width(width), _height(height), _x(x), _y(y), _depth(depth) {
  std::vector<std::string> frames;
  std::string atlasImage;
  std::string atlasMetaData;

  if(type == "explosion1") {
    for(int i = 0; i <= 99; i++) {
      frames.push_back(std::to_string(i));
    }
    atlasImage = "Explosions/explosion1.png";
    atlasMetaData = "Explosions/explosion1.json";
  } else if(type == "explosion2") {
    for(int i = 0; i <= 49; i++) {
      frames.push_back(std::to_string(i));
    }
    atlasImage = "Explosions/explosion2.png";
    atlasMetaData = "Explosions/explosion2.json";
  } else if(type == "explosion3") {
    for(int i = 0; i <= 21; i++) {
      frames.push_back(std::to_string(i));
    }
    atlasImage = "Explosions/explosion3.png";
    atlasMetaData = "Explosions/explosion3.json";
  } else if (type == "explosion4") {
    for (int i = 0; i <= 67; i++) {
      frames.push_back(std::to_string(i));
    }
    atlasImage = "Explosions/explosion4.png";
    atlasMetaData = "Explosions/explosion4.json";
  } else if(type == "explosion5") {
    for(int i = 0; i <= 78; i++) {
      frames.push_back(std::to_string(i));
    }
    atlasImage = "Explosions/explosion5.png";
    atlasMetaData = "Explosions/explosion5.json";
  } else if(type == "explosion6") {
    for(int i = 0; i <= 55; i++) {
      frames.push_back(std::to_string(i));
    }
    atlasImage = "Explosions/explosion6.png";
    atlasMetaData = "Explosions/explosion6.json";
  }


  float scale = (float) (rand() % 100) / 100 + 1.0f; //RANDOM SCALING between 1.0f and 2.0f

  _width = width * scale;
  _height = height * scale;
  
  Ess2D::TextureAtlas* textureAtlas = Game::GetInstance()->getGameplayScreen()->getTextureCache()->getAtlas("Textures/" + atlasImage, "Textures/" + atlasMetaData);

  _color = Ess2D::ColorRGBA8(225, 255, 255, 255);

  _animationManager = new Ess2D::AnimationManager();

  Ess2D::Animation* animation = _animationManager->add("DEFAULT");
  animation->setPlaybackRate(playbackRate);
  animation->setTextureAtlas(textureAtlas);
  animation->setAutoStop(true);
  animation->setRepeat(false);
  animation->setFrames(frames);

  _animationManager->play("DEFAULT");
}

Particle::~Particle() {
  delete _animationManager;
}

void Particle::update(float deltaTime) {
  _animationManager->update(deltaTime);
}

void Particle::draw() {
  Ess2D::SpriteBatch* spriteBatch = Game::GetInstance()->getGameplayScreen()->getSpriteBatch();
  Ess2D::TextureAtlas* textureAtlas = _animationManager->getCurrent()->getTextureAtlas();
  std::string currentAnimationFrame = _animationManager->getCurrent()->getCurrentFrame();

  spriteBatch->draw(glm::vec4(_x - _width / 2, _y - _height / 2, _width, _height), textureAtlas->getUV(currentAnimationFrame), textureAtlas->getTextureId(), _color, (float) _depth, 0.0f);
}

bool Particle::isDead() {
  return !_animationManager->isPlaying();
}
