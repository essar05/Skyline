#include "Level.h"
#include "Game.h"
#include <Camera2D.h>
#include <iostream>

Level::Level() {
  Level(0, 0);
}

Level::Level(float width, float height) :_height(height), _width(width) {
}

Level::~Level() {
}

void Level::addSection(LevelSection* section) {
  this->_sections[_height] = section;
  _height += section->getHeight();

  if(_sections.size() == 1) {
    _renderStartIt = _sections.begin();
  }
}

void Level::draw() {
  Game* gameInstance = Game::GetInstance();

  Essengine::SpriteBatch* spriteBatch = gameInstance->getSpriteBatch();
  Essengine::Camera2D* camera = gameInstance->getMainCamera();
  glm::vec2 viewportSize = camera->getViewportSize();
  
  /*
    only render the sections currently in the viewport.
    originally starting from the first section, 
    once we reach the first section that is above the top edge of the viewport
      we are going to break the loop and stop rendering
    if section we are looking at currently is below the bottom edge of the viewport, 
      we set the new section rendering iterator to the next section and we skip over to the next loop cycle
  */
  auto it = _renderStartIt;
  while(it != _sections.end()) {
    if(camera->getScaledPosition()[1] + viewportSize[1] < it->first) {
      break;
    } else if(camera->getScaledPosition()[1] > it->first + it->second->getHeight()) {
      _renderStartIt = ++it;
      continue;
    }

    it->second->draw(0.0f, it->first);

    it++;
  }

  
}