#include "Level.h"
#include <Camera2D.h>
#include <iostream>
#include "Utils.h"

Level::Level() : Level(0, 0) {}

Level::Level(float width, float height) : _height(height), _width(width) {
  _game = Game::GetInstance();
}

Level::~Level() {
  delete _player;
  delete _world;

  auto it = _sections.begin();
  while(it != _sections.end()) {
    delete it->second;

    it++;
  }
}

void Level::addSection(LevelSection* section) {
  this->_sections[_height] = section;
  _height += section->getHeight(); //already scaled to world

  if(_sections.size() == 1) {
    _renderStartIt = _sections.begin();
  }
}

void Level::update(float deltaTime) {
  Essengine::SpriteBatch* spriteBatch = _game->getSpriteBatch();
  Essengine::Camera2D* camera = _game->getMainCamera();
  glm::vec2 viewportSize = camera->getWorldViewportSize();

  /*
  only render the sections currently in the viewport.
  originally starting from the first section,
  once we reach the first section that is above the top edge of the viewport
  we are going to break the loop and stop rendering
  if section we are looking at currently is below the bottom edge of the viewport,
  we set the new section rendering iterator to the next section and we skip over to the next loop cycle
  */
  auto it = _sections.begin();
  std::unordered_map<float, LevelSection*>::iterator endIt = _sections.begin();
  while(it != _sections.end()) {
    if(camera->getPosition().y / camera->getZoom() + viewportSize[1] / 2 < it->first) {
      break;
    } else if(camera->getPosition().y / camera->getZoom() - viewportSize[1] / 2 > it->first + it->second->getHeight()) {
      delete it->second;
      endIt = ++it;
      continue;
    }

    it->second->update();

    it++;
  }
  
  _sections.erase(_sections.begin(), endIt);

  //update the player stuff
  _player->update(deltaTime);

  //iterate b2 world
  _world->Step(deltaTime, 8, 3);
}

void Level::draw() {
  Essengine::SpriteBatch* spriteBatch = _game->getSpriteBatch();
  Essengine::Camera2D* camera = _game->getMainCamera();
  glm::vec2 viewportSize = camera->getWorldViewportSize();
  
  /*
    only render the sections currently in the viewport.
    originally starting from the first section, 
    once we reach the first section that is above the top edge of the viewport
      we are going to break the loop and stop rendering
    if section we are looking at currently is below the bottom edge of the viewport, 
      we set the new section rendering iterator to the next section and we skip over to the next loop cycle
  */

  auto it = _sections.begin();
  while(it != _sections.end()) {
    if(camera->getPosition().y + viewportSize[1] < it->first) {
      break;
    }

    it->second->draw(0.0f, it->first);

    it++;
  }

  _player->draw();
}

void Level::smoothStates() {
  const float oneMinusRatio = 1.f - _game->getTimeStepAccumulatorRatio();

  for(b2Body * b = _world->GetBodyList(); b != NULL; b = b->GetNext()) {
    if(b->GetType() == b2_staticBody) {
      continue;
    }

    Entity* e = static_cast<Entity*>(b->GetUserData());
    if(e) {
      e->setPosition(
        _game->getTimeStepAccumulatorRatio() * Utils::toVec2(b->GetPosition()) +
        oneMinusRatio * e->getPreviousPosition()
      );
    }
  }
}

void Level::resetSmoothStates() {
  for(b2Body * b = _world->GetBodyList(); b != NULL; b = b->GetNext()) {
    if(b->GetType() == b2_staticBody) {
      continue;
    }

    Entity* e = static_cast<Entity*>(b->GetUserData());
    if(e) {
      glm::vec2 position = Utils::toVec2(b->GetPosition());
      e->setPosition(position);
      e->setPreviousPosition(position);
    }
  }
}

//this function is a bit of a clusterfuck, should think of breaking it down better
void Level::load(std::string levelName) {
  //should probably think about discarding old level when loading a new one.

  Essengine::TextureCache* textureCache = _game->getTextureCache();
  EntityManager* entityManager = _game->getEntityManager();
  Essengine::Camera2D* camera = _game->getMainCamera();

  _world = new b2World(b2Vec2(0, 0));
  _world->SetAutoClearForces(false);
  //_world->SetDebugDraw(&_glDebugDrawInstance);
  _glDebugDrawInstance.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);

  _player = new Player(textureCache->getTexture("Textures/Cumz4AC.png")._id, 90.0f, 120.0f, glm::vec2(camera->getViewportSize().x / 2, 100.0f));
  _player->spawn();

  std::ifstream t("Levels/" + levelName + ".sky");
  std::string str;
  
  t.seekg(0, std::ios::end);
  str.reserve((unsigned int) t.tellg());
  t.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

  rapidjson::Document document;
  document.Parse(str.c_str());

  if(!document.HasMember("sections") || !document["sections"].IsArray()) {
    throw Essengine::ERuntimeException("Level file invalid: missing sections");
  }

  for(rapidjson::SizeType i = 0; i < document["sections"].Size(); i++) {
    //check exists;

    float sectionWidth = (float) document["sections"][i]["width"].GetDouble();
    float sectionHeight = (float) document["sections"][i]["height"].GetDouble();
    std::string textureName = document["sections"][i]["background"].GetString();

    LevelSection* section = new LevelSection(sectionWidth, sectionHeight);
    section->setBackground(textureCache->getTexture("Textures/" + textureName)._id);

    if(document["sections"][i].HasMember("objects") && document["sections"][i]["objects"].IsArray()) {
      for(rapidjson::SizeType j = 0; j < document["sections"][i]["objects"].Size(); j++) {
        float objectX = (float) document["sections"][i]["objects"][j]["x"].GetDouble();
        float objectY = (float) document["sections"][i]["objects"][j]["y"].GetDouble() + camera->getScreenScalar(_height); //translate on y axis by _height because the initial coordinates are relative to the section bounds
        
        //create the entity, add it to the manager and then add it to the section.
        Entity* e = new Entity(textureCache->getTexture("Textures/camel.png")._id, 100.0f, 115.0f, glm::vec2(objectX, objectY));
        section->addObject(entityManager->addEntity(e));
      }
    }

    this->addSection(section);
  }
}

void Level::addActiveObject(unsigned int k) {
  _activeObjects.push_back(k);
}

void Level::discardActiveObject() {
  _activeObjects.erase(_activeObjects.begin());
}

std::vector<unsigned int> Level::getActiveObjects() {
  return _activeObjects;
}

b2World* Level::getWorld() {
  return _world;
}

Player* Level::getPlayer() {
  return _player;
}