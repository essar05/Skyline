#include "Shader.h"
#include "Errors\RuntimeException.cpp"
#include <vector>

namespace Ess2D {

  Shader::Shader(bool debugMode = false):_debugMode(debugMode) {
    _programId = glCreateProgram();
  }

  Shader::~Shader() {
    glDeleteProgram(_programId);

    for (std::map<GLuint, GLuint>::iterator it = _shaderIds.begin(); it != _shaderIds.end(); ++it) {
      glDeleteShader(it->second);
    }
  }

  void Shader::loadShader(ShaderType type, const std::string& shaderFilePath) {
    if (_shaderIds[type] != 0) {
      throw Ess2D::ERuntimeException("Already loaded another shader of the same type.");
    }

    _shaderIds[type] = glCreateShader(type);

    if (_shaderIds[type] == 0) {
      throw Ess2D::ERuntimeException("Failed to create shader id");
    }

    std::string shaderCode;
    std::ifstream fileStream(shaderFilePath.c_str(), std::ios::in);

    //read the shader file line by line and store it all in shaderCode
    if (fileStream.is_open()) {
      std::string line = "";
      while (getline(fileStream, line)) {
        shaderCode += "\n" + line;
      }
      fileStream.close();
    } else {
      throw Ess2D::ERuntimeException("Cannot load shader file: " + shaderFilePath);
    }

    char const* shaderCodePointer = shaderCode.c_str();
    glShaderSource(_shaderIds[type], 1, &shaderCodePointer, nullptr);

    if (_debugMode) {
      std::cout << "Loaded shader " << shaderFilePath << std::endl;
    }
  }

  void Shader::compileShaders() {
    for (std::map<GLuint, GLuint>::iterator it = _shaderIds.begin(); it != _shaderIds.end(); ++it) {
      GLuint shaderId = it->second;

      glCompileShader(shaderId);

      GLint compileSuccess = 0;
      glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccess);

      if (compileSuccess == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

        //should be done in destructor
        //glDeleteShader(shaderId);
        std::printf("%s\n", &errorLog[0]);
        throw Ess2D::ERuntimeException("Failed to compile shader with id " + shaderId);
      }
    }

    if (_debugMode) {
      std::cout << _shaderIds.size() << " shaders compiled successfully" << std::endl;
    }
  }

  void Shader::linkShaders() {
    //atach all of our shaders to the program
    for (std::map<GLuint, GLuint>::iterator it = _shaderIds.begin(); it != _shaderIds.end(); ++it) {
      glAttachShader(_programId, it->second);
    }

    //link the program
    glLinkProgram(_programId);

    GLint isLinked = 0;
    glGetProgramiv(_programId, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
      GLint maxLength = 0;
      glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<char> errorLog(maxLength);
      glGetProgramInfoLog(_programId, maxLength, &maxLength, &errorLog[0]);

      std::printf("%s\n", &errorLog[0]);
      throw Ess2D::ERuntimeException("Failed to link program with id " + _programId);
    }

    //detach shaders after program was linked
    for (std::map<GLuint, GLuint>::iterator it = _shaderIds.begin(); it != _shaderIds.end(); ++it) {
      glDetachShader(_programId, it->second);
      glDeleteShader(it->second);
    }

    if (_debugMode) {
      std::cout << _shaderIds.size() << " shaders linked to the program successfully" << std::endl;
    }

    _shaderIds.clear();
  }

  void Shader::addAttribute(const std::string& attributeName) {
    glBindAttribLocation(_programId, _attributeCount++, attributeName.c_str());

    if (_debugMode) {
      std::cout << "Attribute added: " << attributeName << std::endl;
    }
  }

  GLuint Shader::getUniformLocation(const std::string& uniformName) {
    return glGetUniformLocation(_programId, uniformName.c_str());
  }

  void Shader::use() {
    glUseProgram(_programId);
    for (int i = 0; i < _attributeCount; i++) {
      glEnableVertexAttribArray(i);
    }
  }

  void Shader::unuse() {
    glUseProgram(_programId);
    for (int i = 0; i < _attributeCount; i++) {
      glDisableVertexAttribArray(i);
    }
  }

}