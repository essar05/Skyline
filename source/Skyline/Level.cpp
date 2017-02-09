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
  std::map<float, LevelSection*>::iterator endIt = _sections.begin();
  bool deleteSection = false;
  while(it != _sections.end()) {
    if(camera->getPosition().y / camera->getZoom() + viewportSize[1] / 2 < it->first) {
      break;
    } else if(camera->getPosition().y / camera->getZoom() - viewportSize[1] / 2 > it->first + it->second->getHeight()) {
      delete it->second;
      endIt = ++it;
      deleteSection = true;
      continue;
    }

    it->second->update();

    it++;
  }
  
  if(deleteSection) {
    _sections.erase(_sections.begin(), endIt);
  }

  //update the player stuff
  _player->update(deltaTime);

  //iterate b2 world
  _world->Step(deltaTime, 8, 3);
}

void Level::draw() {
  Essengine::SpriteBatch* spriteBatch = _game->getSpriteBatch();
  Essengine::Camera2D* camera = _game->getMainCamera();
  glm::vec2 viewportSize = camera->getWorldViewportSize();
  
  float bottomCameraEdge = camera->getPosition().y / camera->getZoom() - viewportSize[1] / 2;
  float topCameraEdge = camera->getPosition().y / camera->getZoom() + viewportSize[1] / 2;
  int bgBottomPassedCount = floor(bottomCameraEdge / _backgroundHeight); // number of times the background image has repeated entirely below the bottom camera edge
  int bgTopPassedCount = floor(topCameraEdge / _backgroundHeight); // number of times the background image has repeated entirely below the top camera edge

  glm::vec4 position(0, _backgroundHeight * bgBottomPassedCount, _backgroundWidth, _backgroundHeight);
  glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
  Essengine::ColorRGBA8 color(255, 255, 255, 255);
  spriteBatch->draw(position, uv, _backgroundId, color, 0);

  if (bgBottomPassedCount < bgTopPassedCount) {
    position = glm::vec4(0, _backgroundHeight * bgTopPassedCount, _backgroundWidth, _backgroundHeight);
    spriteBatch->draw(position, uv, _backgroundId, color, 0);
  }

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
    if(camera->getPosition().y / camera->getZoom() + viewportSize[1] / 2 < it->first) {
      break;
    }

    it->second->draw(0.0f, it->first);

    it++;
  }

  _player->draw();
}

void Level::smoothStates() {
  const float timestepAccumulatorRatio = _game->getTimeStepAccumulatorRatio();

  for(b2Body * b = _world->GetBodyList(); b != NULL; b = b->GetNext()) {
    if(b->GetType() == b2_staticBody) {
      continue;
    }

    Entity* entity = static_cast<Entity*>(b->GetUserData());
    if(entity) {
      entity->smoothStates(timestepAccumulatorRatio);
    }
  }
}

void Level::resetSmoothStates() {
  for(b2Body * b = _world->GetBodyList(); b != NULL; b = b->GetNext()) {
    if(b->GetType() == b2_staticBody) {
      continue;
    }

    Entity* entity = static_cast<Entity*>(b->GetUserData());
    if(entity) {
      entity->resetSmoothStates();
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
  _world->SetContactListener(&_contactListener);
  _glDebugDrawInstance.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);

  Essengine::TextureAtlas* atlas = textureCache->getAtlas("Textures/player.png", "Textures/player.json");
  //glm::vec4 uv = atlas->getUV("player");
  
  _player = new Player(atlas->getTextureId(), atlas->getUV("Spaceship_default"), 100.0f, 76.0f, glm::vec2(camera->getViewportSize().x / 2, 100.0f));
  _player->createB2Data();
  _player->spawn();

  std::ifstream t("Levels/" + levelName + ".sky");
  std::string str;
  
  t.seekg(0, std::ios::end);
  str.reserve((unsigned int) t.tellg());
  t.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

  rapidjson::Document document;
  document.Parse(str.c_str());

  if (document.HasMember("defaultBackground")) {
    std::string defaultBackground = document["defaultBackground"].GetString();
    this->setBackground( &(textureCache->getTexture("Textures/" + defaultBackground)) );
  }

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
        Essengine::TextureAtlas* atlas = textureCache->getAtlas("Textures/spritesheet.png", "Textures/sprites.json");
        Entity* e = new Entity(atlas->getTextureId(), atlas->getUV("enemy"), 100.0f, 115.0f, glm::vec2(objectX, objectY));
        e->createB2Data();
        section->addObject(entityManager->addEntity(e));
      }
    }

    this->addSection(section);
  }
}

void Level::setBackground(Essengine::GLTexture* texture) {
  _backgroundId = texture->_id;

  _backgroundWidth = _game->getMainCamera()->getWorldViewportSize().x;
  _backgroundHeight = texture->_height * (_backgroundWidth / texture->_width);
}

b2World* Level::getWorld() {
  return _world;
}

Player* Level::getPlayer() {
  return _player;
}