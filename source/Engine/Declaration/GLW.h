#ifndef GLW_H
#define GLW_H

#include <glew.h>

namespace Engine {
  class GLW {
    public:
      //Methods
      GLW();
      static void test();
      static GLuint CreateProgram();
      static GLuint CreateShader(GLuint aShader);
      static const GLchar** GetVertexShaderSource();
      static const GLchar** GetFragmentShaderSource();
      static void ShaderSource(GLuint aShader, GLsizei aCount, const GLchar ** aSource, const GLint* aLength);
      static void CompileShader(GLuint aShader);
      static void GetShaderiv(GLuint aShader, GLenum aParameterName, GLint* aParameter);
      static void AttachShader(GLuint aProgram, GLuint aShader);
      static void LinkProgram(GLuint aProgram);
      static void GetProgramiv(GLuint aProgram, GLenum aParameterName, GLint* aParameter);
      static GLint GetAttribLocation(GLuint aProgram, const GLchar* aName);
      static void ClearColor(GLclampf aRed, GLclampf aGreen, GLclampf aBlue, GLclampf anAlpha);
      static void GenBuffers(GLsizei aSize, GLuint* someBuffers);
      static void BindBuffer(GLenum aTarget, GLuint aBuffer);
      static void BufferData(GLenum aTarget, GLsizeiptr aSize, const GLvoid* aData, GLenum aUsage);
      static void Clear(GLbitfield aMask);
      static void EnableVertexAttribArray(GLuint anIndex);
      static void DisableVertexAttribArray(GLuint anIndex);
      static void DrawElements(GLenum aMode, GLsizei aCount, GLenum aType, const GLvoid* someIndices);
      static void VertexAttribPointer(GLuint anIndex, GLint aSize, GLenum aType, GLboolean isNormalized, GLsizei aStride, const GLvoid* aPointer);
      static void UseProgram(GLuint aProgram);
      static void DeleteProgram(GLuint aProgram);

      //Properties

    protected:
    private:
  };
}
#endif // GLW_H
