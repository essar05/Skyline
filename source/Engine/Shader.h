#ifndef SHADER_H
#define SHADER_H

class Shader
{
  public:
    Shader();
    static GLuint Load(const char* aVertexFilePath, const char* aFragmentFilePath);
  protected:
  private:
};

#endif // SHADER_H
