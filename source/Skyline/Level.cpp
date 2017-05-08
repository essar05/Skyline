#include "Level.h"
#include <Camera2D.h>
#include <iostream>
#include "Utils.h"
#include "SpaceshipA.h"
#include "SpaceshipB.h"
#include "SpaceshipC.h"
#include "SpaceshipD.h"

Level::Level() : Level(0, 0) {}

Level::Level(float width, float height) : _height(height), _width(width) {
  _game = Game::GetInstance();
}

Level::~Level() {
  delete _player;

  for(b2Body * b = _world->GetBodyList(); b != NULL; b = b->GetNext()) {
    Entity* entity = static_cast<Entity*>(b->GetUserData());
    if(entity) {
      entity->cleanupBody();
    }
  }

  auto it = _sections.begin();
  while(it != _sections.end()) {
    delete it->second;

    it++;
  }

  delete _world;

  for(unsigned int i = 0; i < _decorationLayers.size(); i++) {
    auto decorationsIt = _decorations.find(_decorationLayers[i]);
    if(decorationsIt == _decorations.end()) {
      continue;
    }

    int layer = _decorationLayers[i];
    for(unsigned int j = 0; j < decorationsIt->second.size();) {
      delete decorationsIt->second[j];

      j++;
    }
  }
  //DELETE DECORATIONS !!!!!
}

bool Level::isAtEnd() {
  Ess2D::Camera2D* camera = _game->getGameplayScreen()->getMainCamera();
  return camera->getPosition().y >= camera->getWorldScalar(_end);
}

void Level::update(float deltaTime) {
  Ess2D::SpriteBatch* spriteBatch = _game->getGameplayScreen()->getSpriteBatch();
  Ess2D::Camera2D* camera = _game->getGameplayScreen()->getMainCamera();
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
    } else if(it->second->getHeight() > 0 && camera->getPosition().y / camera->getZoom() - viewportSize[1] / 2 > it->first + it->second->getHeight()) {
      delete it->second;
      endIt = ++it;
      deleteSection = true;
      continue;
    }

    it->second->update(deltaTime);

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
  Ess2D::SpriteBatch* spriteBatch = _game->getGameplayScreen()->getSpriteBatch();
  Ess2D::Camera2D* camera = _game->getGameplayScreen()->getMainCamera();
  glm::vec2 viewportSize = camera->getWorldViewportSize();
  
  float bottomCameraEdge = camera->getPosition().y / camera->getZoom() - viewportSize[1] / 2;
  float bgPos = bottomCameraEdge - (camera->getPosition().y / camera->getZoom()) / 10.f;

  int bgBottomPassedCount = (int) floor((bottomCameraEdge - bgPos) / _backgroundHeight);
  int bgTopPassedCount = (int) floor((bottomCameraEdge - bgPos + viewportSize[1]) / _backgroundHeight);

  glm::vec4 position(0, bgPos + _backgroundHeight * bgBottomPassedCount, _backgroundWidth, _backgroundHeight);
  glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
  Ess2D::ColorRGBA8 color(255, 255, 255, 255);
  spriteBatch->draw(position, uv, _backgroundId, color, 0);

  if (bgBottomPassedCount < bgTopPassedCount) {
    position = glm::vec4(0, bgPos + _backgroundHeight * bgTopPassedCount, _backgroundWidth, _backgroundHeight);
    spriteBatch->draw(position, uv, _backgroundId, color, 0);
  }
  
  for (unsigned int i = 0; i < _decorationLayers.size(); i++) {
    auto decorationsIt = _decorations.find(_decorationLayers[i]);
    if (decorationsIt == _decorations.end()) {
      continue;
    }

    int layer = _decorationLayers[i];
    for (unsigned int j = 0; j < decorationsIt->second.size();) {
      LevelDecoration* decoration = decorationsIt->second[j];

      float cameraY = camera->getPosition().y / camera->getZoom();
      float decorationY = decoration->getY();
      float deltaY = cameraY - decorationY;
      decorationY += deltaY * decoration->getDisplacement();

      if (camera->getPosition().y / camera->getZoom() - viewportSize[1] / 2 > decorationY + decoration->getHeight() / 2) {
        decorationsIt->second.erase(decorationsIt->second.begin() + j);
        continue;
      } else if (camera->getPosition().y / camera->getZoom() + viewportSize[1] / 2 < decorationY - decoration->getHeight() / 2) {
        break;
      }

      position = glm::vec4(decoration->getX() - decoration->getWidth() / 2, decorationY - decoration->getHeight() / 2, decoration->getWidth(), decoration->getHeight());
      spriteBatch->draw(position, uv, decoration->getTextureId(), color, (float) layer);

      j++;
    }
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
  const float timestepAccumulatorRatio = _game->getTimestepAccumulator()->getAccumulatorRatio();

  int k = 0;
  for(b2Body * b = _world->GetBodyList(); b != NULL; b = b->GetNext()) {
    if(b->GetType() == b2_staticBody) {
      continue;
    }

    Entity* entity = static_cast<Entity*>(b->GetUserData());
    if(entity) {
      entity->smoothStates(timestepAccumulatorRatio);
    }

    k++;
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

  Ess2D::TextureCache* textureCache = _game->getGameplayScreen()->getTextureCache();
  EntityManager* entityManager = _game->getGameplayScreen()->getEntityManager();
  Ess2D::Camera2D* camera = _game->getGameplayScreen()->getMainCamera();

  _world = new b2World(b2Vec2(0, 0));
  _world->SetAutoClearForces(false);
  //_world->SetDebugDraw(&_glDebugDrawInstance);
  _world->SetContactListener(&_contactListener);
  _glDebugDrawInstance.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);

  Ess2D::TextureAtlas* atlas = textureCache->getAtlas("Textures/player.png", "Textures/player.json");
  //glm::vec4 uv = atlas->getUV("player");
  
  _player = new Player(atlas->getTextureId(), atlas->getUV("Spaceship_default"), 100.0f, 76.0f, glm::vec2(camera->getViewportSize().x / 2, camera->getScreenScalar(camera->getPosition().y) - camera->getViewportSize().y  / 2 + 100.0f));
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

  _end = document["height"].GetInt();

  //LOAD SECTIONS
  if(!document.HasMember("sections") || !document["sections"].IsArray()) {
    throw Ess2D::ERuntimeException("Level file invalid: missing sections");
  }

  float sectionWidth = 1280.0f;

  for(rapidjson::SizeType i = 0; i < document["sections"].Size(); i++) {
    //check exists;

    sectionWidth = (float) document["sections"][i]["width"].GetDouble();
    float sectionHeight = 0.0f;
    if (document["sections"][i].HasMember("height")) {
      sectionHeight = (float) document["sections"][i]["height"].GetDouble();
    }
    std::string sectionBackground = "";
    if (document["sections"][i].HasMember("background")) {
      sectionBackground = document["sections"][i]["background"].GetString();
    }
    
    LevelSection* section = new LevelSection(sectionWidth, sectionHeight);
    if(sectionBackground != "") {
      section->setBackground(textureCache->getTexture("Textures/" + sectionBackground)._id);
    }

    if(document["sections"][i].HasMember("objects") && document["sections"][i]["objects"].IsArray()) {
      for(rapidjson::SizeType j = 0; j < document["sections"][i]["objects"].Size(); j++) {

        float objectX = 0;
        if(document["sections"][i]["objects"][j].HasMember("xP") && document["sections"][i]["objects"][j]["xP"].IsDouble()) {
          objectX = (float) document["sections"][i]["objects"][j]["xP"].GetDouble() * sectionWidth;
        } else {
          objectX = (float) document["sections"][i]["objects"][j]["x"].GetDouble();
        }
        float objectY = (float) document["sections"][i]["objects"][j]["y"].GetDouble() + camera->getScreenScalar(_height); //translate on y axis by _height because the initial coordinates are relative to the section bounds
        
        float velocityY = 0;
        if(document["sections"][i]["objects"][j].HasMember("velocityY") && document["sections"][i]["objects"][j]["velocityY"].IsDouble()) {
          velocityY = (float)document["sections"][i]["objects"][j]["velocityY"].GetDouble();
        }

        float velocityX = 0;
        if(document["sections"][i]["objects"][j].HasMember("velocityX") && document["sections"][i]["objects"][j]["velocityX"].IsDouble()) {
          velocityX = (float)document["sections"][i]["objects"][j]["velocityX"].GetDouble();
        }

        //create the entity, add it to the manager and then add it to the section.
        Entity* e = NULL;
        
        if(document["sections"][i]["objects"][j].HasMember("type") && document["sections"][i]["objects"][j]["type"].IsString()) {
          std::string type = document["sections"][i]["objects"][j]["type"].GetString();

          if(type == "spaceshipA") {
            e = new SpaceshipA(atlas->getTextureId(), atlas->getUV("enemy"), 100.0f, 115.0f, glm::vec2(objectX, objectY), 180.0f);
          } else if(type == "spaceshipB") {
            e = new SpaceshipB(atlas->getTextureId(), atlas->getUV("enemy"), 100.0f, 115.0f, glm::vec2(objectX, objectY), 180.0f);
          } else if(type == "spaceshipC") {
            e = new SpaceshipC(atlas->getTextureId(), atlas->getUV("enemy"), 100.0f, 115.0f, glm::vec2(objectX, objectY), 180.0f);
          } else if(type == "spaceshipD") {
            e = new SpaceshipD(atlas->getTextureId(), atlas->getUV("enemy"), 100.0f, 115.0f, glm::vec2(objectX, objectY), 180.0f);
          }
        } else {
          e = new Entity(atlas->getTextureId(), atlas->getUV("enemy"), 100.0f, 115.0f, glm::vec2(objectX, objectY), 180.0f);
        }

        e->setVelocity(glm::vec2(velocityX, velocityY));
        e->createB2Data();
        section->addObject(entityManager->addEntity(e));
      }
    }

    this->addSection(section);
  }

  //LOAD DECORATIONS
  if (document.HasMember("decorations") && document["decorations"].IsArray()) {
    for (rapidjson::SizeType i = 0; i < document["decorations"].Size(); i++) {

      float x = 0;
      if(document["decorations"][i].HasMember("xP") && document["decorations"][i]["xP"].IsDouble()) {
        x = (float)document["decorations"][i]["xP"].GetDouble() * sectionWidth;
      } else {
        x = (float)document["decorations"][i]["x"].GetDouble();
      }

      float y = (float) document["decorations"][i]["y"].GetDouble();
      std::string textureName = document["decorations"][i]["textureName"].GetString();
      int layer = document["decorations"][i]["layer"].GetInt();
      float displacement = (float)document["decorations"][i]["displacement"].GetDouble();
      float scale = 1.0f;
      if (document["decorations"][i].HasMember("scale")) {
        scale = (float) document["decorations"][i]["scale"].GetDouble();
      }
      
      Ess2D::Texture2D texture = textureCache->getTexture("Textures/" + textureName);

      LevelDecoration* decoration = new LevelDecoration(texture._id, x, y, (float) texture._width, (float) texture._height, scale, displacement);
      
      this->addDecoration(decoration, layer);
    }
  }
}


void Level::addSection(LevelSection* section) {
  this->_sections[_height] = section;
  _height += section->getHeight(); //already scaled to world

  if (_sections.size() == 1) {
    _renderStartIt = _sections.begin();
  }
}


void Level::addDecoration(LevelDecoration* decoration, int layer) {
  std::map<int, std::vector<LevelDecoration*>>::iterator it = _decorations.find(layer);
  if (it == _decorations.end()) {
    it = _decorations.insert(std::make_pair(layer, std::vector<LevelDecoration*>())).first;

    _decorationLayers.push_back(layer);
    std::stable_sort(_decorationLayers.begin(), _decorationLayers.end(), compareDecorationLayerDesc);
  }

  it->second.push_back(decoration);

  std::stable_sort(it->second.begin(), it->second.end(), compareDecorationYPosition);
}

LevelSection* Level::getSection(int index) {
  auto it = _sections.begin();
  int k = 0;
  while(it != _sections.end()) {
    if(k == index) {
      return it->second;
    }
    
    if(k > index) {
      throw Ess2D::ERuntimeException("Level Section index out of bounds");
    }

    k++;
    it++;
  }
}

void Level::setBackground(Ess2D::Texture2D* texture) {
  _backgroundId = texture->_id;

  _backgroundWidth = _game->getGameplayScreen()->getMainCamera()->getWorldViewportSize().x;
  _backgroundHeight = texture->_height * (_backgroundWidth / texture->_width);
}

b2World* Level::getWorld() {
  return _world;
}

Player* Level::getPlayer() {
  return _player;
}

bool Level::compareDecorationYPosition(LevelDecoration* a, LevelDecoration* b) {
  return a->getY() < b->getY();
}

bool Level::compareDecorationLayerDesc(int y1, int y2) {
  return y1 > y2;
}