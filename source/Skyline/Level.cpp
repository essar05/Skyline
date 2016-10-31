#include "Level.h"
#include "Game.h"
#include <Camera2D.h>
#include <iostream>

Level::Level() : Level(0, 0) {}

Level::Level(float width, float height) : _height(height), _width(width) {
  _game = Game::GetInstance();
}

Level::~Level() {
  if (_world != nullptr) {
	  delete _world;
  }

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
  _world->Step(deltaTime, 8, 3);
  _world->ClearForces();

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
    if(camera->getPosition().y + viewportSize[1] / 2 < it->first) {
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

void Level::load(std::string levelName) {
  std::ifstream t("Levels/" + levelName + ".sky");
  std::string str;
  
  t.seekg(0, std::ios::end);
  str.reserve((unsigned int) t.tellg());
  t.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(t)),
             std::istreambuf_iterator<char>());

  rapidjson::Document document;

  document.Parse(str.c_str());
  Game* gameInstance = Game::GetInstance();
  Essengine::TextureCache* textureCache = gameInstance->getTextureCache();
  EntityManager* entityManager = gameInstance->getEntityManager();
  Essengine::Camera2D* camera = gameInstance->getMainCamera();

  if(!document.HasMember("sections") || !document["sections"].IsArray()) {
    throw Essengine::ERuntimeException("Level file invalid: missing sections");
  }

  _world = new b2World(b2Vec2(0, 0));

  //in constructor, usually
  _world->SetDebugDraw(&_glDebugDrawInstance);
  //somewhere appropriate
  _glDebugDrawInstance.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);

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
        Entity *e = new Entity(textureCache->getTexture("Textures/camel.png")._id, 100.0f, 115.0f, glm::vec2(objectX, objectY));
        section->addObject(entityManager->addEntity(e));
      }
    }

    this->addSection(section);
  }
}

b2World* Level::getWorld() {
  return this->_world;
}