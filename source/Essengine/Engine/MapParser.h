#pragma once

#include <fstream>
#include "RuntimeException.cpp"
#include "Map.h"

using namespace std;

typedef unsigned char Byte;
typedef int U32;
typedef short U16;
typedef char U8;

namespace Essengine {
  class MapParser {
    public:
      MapParser();
      ~MapParser();
      void Parse(Map* aMap, const char* aMapFile);
      void Write(Map* aMap, const char* aMapFile);
      void WriteU32(U32 aValue);
      void WriteU16(U32 aValue);
      void WriteU16(U16 aValue);
      void WriteU8(U32 aValue);
      void WriteU8(U8 aValue);
      U32 ParseU32();
      U16 ParseU16();
      U8 ParseU8();
    private:
      bool bigEndian;
      int pointer;
      Byte* buffer;
      ofstream* output;
      ifstream* input;
  };
}