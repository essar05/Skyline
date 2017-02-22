#pragma once

#include <string>
#include <GL/glew.h>
#include <stdlib.h>
#include <fstream>
#include <map>

namespace Ess2D {

  enum ShaderType { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER};

  class Shader
  {
  public:
    Shader(bool debugMode);
    ~Shader();

    void loadShader(ShaderType type, const std::string& shaderFilePath);

    void compileShaders();
    void linkShaders();

    void addAttribute(const std::string& attributeName);
    GLuint getUniformLocation(const std::string& uniformName);

    void use();
    void unuse();

  private:
    int _attributeCount;
    bool _debugMode;

    GLuint _programId;

    std::map<GLuint, GLuint> _shaderIds;

  };

}