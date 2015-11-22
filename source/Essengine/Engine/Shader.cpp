#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <glew.h>
#include "GLW.h"
#include "RuntimeException.cpp"
#include "Shader.h"

using namespace std;
namespace Essengine {

  Shader::Shader() {}

  GLuint Shader::Load(const char* aVertexFilePath, const char* aFragmentFilePath) {
    GLuint theVertexShaderId = GLW::CreateShader(GL_VERTEX_SHADER);
    GLuint theFragmentShaderId = GLW::CreateShader(GL_FRAGMENT_SHADER);

    std::string theVertexShaderCode;
    std::ifstream theVertexShaderStream(aVertexFilePath, std::ios::in);

    if (theVertexShaderStream.is_open()) {
      std::string theLine = "";
      while (getline(theVertexShaderStream, theLine)) {
        theVertexShaderCode += "\n" + theLine;
      }
      theVertexShaderStream.close();
    }
    else {
      char* theFormat = "Cannot load shader file %s";
      char* theError = new char[strlen(theFormat) + strlen(aVertexFilePath)];
      _snprintf_s(theError, strlen(theError), 200, theFormat, aVertexFilePath);
      throw ERuntimeException(theError);
    }

    std::string theFragmentShaderCode;
    std::ifstream theFragmentShaderStream(aFragmentFilePath, std::ios::in);
    if (theFragmentShaderStream.is_open()){
      std::string theLine = "";
      while (getline(theFragmentShaderStream, theLine)) {
        theFragmentShaderCode += "\n" + theLine;
      }
      theFragmentShaderStream.close();
    }

    char const* theVertexSourcePointer = theVertexShaderCode.c_str();
    GLW::ShaderSource(theVertexShaderId, 1, &theVertexSourcePointer, NULL);
    GLW::CompileShader(theVertexShaderId);

    char const * theFragmentSourcePointer = theFragmentShaderCode.c_str();
    GLW::ShaderSource(theFragmentShaderId, 1, &theFragmentSourcePointer, NULL);
    GLW::CompileShader(theFragmentShaderId);

    GLuint theProgramId = GLW::CreateProgram();
    GLW::AttachShader(theProgramId, theVertexShaderId);
    GLW::AttachShader(theProgramId, theFragmentShaderId);
    GLW::LinkProgram(theProgramId);

    GLW::DeleteShader(theVertexShaderId);
    GLW::DeleteShader(theFragmentShaderId);

    return theProgramId;
  }

}