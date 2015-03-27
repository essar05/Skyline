#include "MapParser.h"

MapParser::MapParser() {
  unsigned int i = 1;
  char *c = (char*)&i;
  if(*c) {
    this->bigEndian = false;
  } else {
    this->bigEndian = true;
  }
}

MapParser::~MapParser() {

}

void MapParser::Parse(Map* aMap, const char* aMapFile) {
  this->pointer = 0;
  this->input = new ifstream;
  this->input->open(aMapFile, ios::in | ios::binary | ios::ate);
  if(!this->input->is_open()) {
    char* theFormat = "Could not open map file %s";
    char* theError = new char[strlen(theFormat) + strlen(aMapFile)];
    _snprintf_s(theError, strlen(theError), 200, theFormat, aMapFile);
    throw ERuntimeException(theError);
  }
  long theSize = (long) this->input->tellg();
  this->buffer = new Byte[theSize];
  this->input->seekg(0, ios::beg);
  this->input->read((char*) this->buffer, theSize);
  this->input->close();

  //3B file identifier
  char* TypeIdentifier = new char[3];
  TypeIdentifier[0] = this->ParseU8();
  TypeIdentifier[1] = this->ParseU8();
  TypeIdentifier[2] = this->ParseU8();

  if(TypeIdentifier[0] != 'e' || TypeIdentifier[1] != 'm' || TypeIdentifier[2] != 'p') {
    char* theFormat = "Map file invalid %s";
    char* theError = new char[strlen(theFormat) + strlen(aMapFile)];
    _snprintf_s(theError, strlen(theError), 200, theFormat, aMapFile);
    throw ERuntimeException(theError);
  }

  short MajorVersion = this->ParseU8();
  short MinorVersion = this->ParseU8();
  aMap->SetLayersCount(this->ParseU8());
  aMap->SetWidth(this->ParseU16());
  aMap->SetHeight(this->ParseU16());

  if(aMap->Tiles) {
    delete aMap->Tiles;
    delete aMap->Walkable;
    delete aMap->Passable;
  }

  aMap->Tiles = new Tile***[aMap->GetLayersCount()];
  aMap->Walkable = new bool*[aMap->GetWidth()];
  aMap->Passable = new bool*[aMap->GetWidth()];

  for(int i = 0; i < aMap->GetWidth(); i++) {
    aMap->Walkable[i] = new bool[aMap->GetHeight()];
    aMap->Passable[i] = new bool[aMap->GetHeight()];
  }

  for(int i = 0; i < aMap->GetLayersCount(); i++) {
    aMap->Tiles[i] = new Tile**[aMap->GetWidth()];
    
	for (int j = 0; j < aMap->GetHeight(); j++) {
      aMap->Tiles[i][j] = new Tile*[aMap->GetHeight()];
      
      for(int k = 0; k < aMap->GetWidth(); k++) {
		aMap->Tiles[i][j][k] = new Tile;
        aMap->Tiles[i][j][k]->SetId(this->ParseU8());
        char theFlags = this->ParseU8();
        bool isWalkable = (((theFlags >> 7) & 0x01) == 1);
        bool isPassable = (((theFlags >> 6) & 0x01) == 1);

        aMap->Walkable[j][k] = isWalkable;
        aMap->Passable[j][k] = isPassable;
      }
    }
  }
}

void MapParser::Write(Map* aMap, const char* aMapFile) {
  this->output = new ofstream();
  this->output->open(aMapFile, ios::out | ios::binary);
  if(!this->output->is_open()) {
    char* theFormat = "Could not open map file %s";
    char* theError = new char[strlen(theFormat) + strlen(aMapFile)];
    _snprintf_s(theError, strlen(theError), 200, theFormat, aMapFile);
    throw ERuntimeException(theError);
  }
  
  this->pointer = 0;
  this->buffer = new Byte[10 + 2 * aMap->GetLayersCount() * aMap->GetWidth() * aMap->GetHeight()];

  //3B file identifier
  this->WriteU8('e');
  this->WriteU8('m');
  this->WriteU8('p');
  this->WriteU8(1); //1B major version
  this->WriteU8(0); //1B minor version
  this->WriteU8(aMap->GetLayersCount()); //1B layers count
  this->WriteU16((U32) aMap->GetWidth()); //2B map width
  this->WriteU16((U32) aMap->GetHeight()); //2B map height

  for(int i = 0; i < aMap->GetLayersCount(); i++) {
    for(int j = 0; j < aMap->GetLayersCount(); j++) {
      for(int k = 0; k < aMap->GetLayersCount(); k++) {
        this->WriteU8(aMap->Tiles[i][j][k]->GetId()); //1B TILE ID
        char theFlags = 0x00;
        if(aMap->Walkable[j][k] == true) {
          theFlags = theFlags | (0x01 << 7);
        }
        if(aMap->Passable[j][k] == true) {
          theFlags = theFlags | (0x01 << 6);
        }
        this->WriteU8(theFlags); //1B TILE FLAGS WPXXXXXX (Walkable, Passable, 6 x Unused)
      }
    }
  }
  this->output->close();
}

void MapParser::WriteU32(U32 aValue) {
  Byte* bytes = new Byte[4];
  bytes[0] = (aValue >> 24) & 0xFF; // high byte
  bytes[1] = (aValue >> 16) & 0xFF;
  bytes[2] = (aValue >> 8) & 0xFF;
  bytes[3] = (aValue)& 0xFF;  // low byte
  this->output->write((char*)bytes, 4);
}

void MapParser::WriteU16(U16 aValue) {
  Byte* bytes = new Byte[2];
  bytes[0] = (aValue >> 8) & 0xFF; //high byte
  bytes[1] = (aValue)& 0xFF;  //low byte
  this->output->write((char*)bytes, 2);
}

void MapParser::WriteU16(U32 aValue) {
  Byte* bytes = new Byte[2];
  bytes[0] = (aValue >> 8) & 0xFF; //high byte
  bytes[1] = (aValue)& 0xFF;  //low byte
  this->output->write((char*)bytes, 2);
}

void MapParser::WriteU8(U32 aValue) {
  Byte byte = (aValue)& 0xFF;
  this->output->write((char*)&byte, 1);
}

void MapParser::WriteU8(U8 aValue) {
  Byte byte = (aValue) & 0xFF;
  this->output->write((char*)&byte, 1);
}

U32 MapParser::ParseU32() {
  Byte* bytes = new Byte[4];
  
  bytes[0] = (this->buffer[this->pointer++] & 0xff) << 24; //high byte
  bytes[1] = (this->buffer[this->pointer++] & 0xff) << 16;
  bytes[2] = (this->buffer[this->pointer++] & 0xff) << 8;
  bytes[3] = (this->buffer[this->pointer++] & 0xff);  //low byte
  
  return bytes[0] | bytes[1] | bytes[2] | bytes[3];
}

U16 MapParser::ParseU16() {
  Byte* bytes = new Byte[4];
  bytes[0] = (this->buffer[this->pointer++] & 0xff) << 8; //high byte
  bytes[1] = (this->buffer[this->pointer++] & 0xff); //low byte
  return bytes[0] | bytes[1];
}

U8 MapParser::ParseU8() {
  return (this->buffer[this->pointer++] & 0xff);
}