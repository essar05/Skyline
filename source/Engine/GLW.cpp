#include <glew.h>
#include "RuntimeException.cpp"
#include "GLW.h"

using namespace Engine;

GLW::GLW() {
  throw ERuntimeException("You cannot directly initialize OpenGL Wrapper class (Engine::GLLW)");
}

void GLW::test() {

}

GLuint GLW::CreateProgram() {
  return glCreateProgram();
}

GLuint GLW::CreateShader(GLuint aShader) {
  return glCreateShader(aShader);
}

const GLchar** GLW::GetVertexShaderSource() {
  const GLchar** theVertexShaderSource = new const GLchar*[1];
  theVertexShaderSource[0] = {
    "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0.9f, 1 ); }"
  };

	return theVertexShaderSource;
}

const GLchar** GLW::GetFragmentShaderSource() {
  const GLchar** theFragmentShaderSource = new const GLchar*[1];
  theFragmentShaderSource[0] =
  {
    "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
  };
  return theFragmentShaderSource;
}

void GLW::ShaderSource(GLuint aShader, GLsizei aCount, const GLchar ** aSource, const GLint* aLength) {
  glShaderSource(aShader, aCount, aSource, aLength);
}

void GLW::CompileShader(GLuint aShader) {
  glCompileShader(aShader);
  GLint isShaderCompiled = GL_FALSE;
	GLW::GetShaderiv( aShader, GL_COMPILE_STATUS, &isShaderCompiled );
	if(isShaderCompiled != GL_TRUE) {
	  char* theError = NULL;
    sprintf(theError, "Unable to compile vertex shader %d!\n", aShader);
	  throw ERuntimeException(theError);
	}
}

void GLW::GetShaderiv(GLuint aShader, GLenum aParameterName, GLint* aParameter) {
  glGetShaderiv(aShader, aParameterName, aParameter);
}

void GLW::AttachShader(GLuint aProgram, GLuint aShader) {
  glAttachShader(aProgram, aShader);
}

void GLW::LinkProgram(GLuint aProgram) {
  glLinkProgram(aProgram);
  GLint isLinkValid = GL_TRUE;
  GLW::GetProgramiv(aProgram, GL_LINK_STATUS, &isLinkValid );
  if(isLinkValid != GL_TRUE)
  {
    char* theError = NULL;
    sprintf(theError, "Error linking program %d!\n", aProgram);
	  throw ERuntimeException(theError);
  }
}

void GLW::GetProgramiv(GLuint aProgram, GLenum aParameterName, GLint* aParameter) {
  glGetProgramiv(aProgram, aParameterName, aParameter);
}

GLint GLW::GetAttribLocation(GLuint aProgram, const GLchar* aName) {
  GLint theAttribLocation = glGetAttribLocation(aProgram, aName);
  if(theAttribLocation == -1)
  {
    throw ERuntimeException("LVertexPos2D is not a valid glsl program variable!\n");
  }
  return theAttribLocation;
}

void GLW::ClearColor(GLclampf aRed, GLclampf aGreen, GLclampf aBlue, GLclampf anAlpha) {
  glClearColor(aRed, aGreen, aBlue, anAlpha);
}

void GLW::GenBuffers(GLsizei aSize, GLuint* someBuffers) {
  glGenBuffers(aSize, someBuffers);
}

void GLW::BindBuffer(GLenum aTarget, GLuint aBuffer) {
  glBindBuffer(aTarget, aBuffer);
}

void GLW::BufferData(GLenum aTarget, GLsizeiptr aSize, const GLvoid* aData, GLenum aUsage) {
  glBufferData(aTarget, aSize, aData, aUsage);
}

void GLW::Clear(GLbitfield aMask) {
  glClear(aMask);
}

void GLW::EnableVertexAttribArray(GLuint anIndex) {
  glEnableVertexAttribArray(anIndex);
}

void GLW::DisableVertexAttribArray(GLuint anIndex) {
  glDisableVertexAttribArray(anIndex);
}

void GLW::DrawElements(GLenum aMode, GLsizei aCount, GLenum aType, const GLvoid* someIndices) {
  glDrawElements(aMode, aCount, aType, someIndices);
}

void GLW::VertexAttribPointer(GLuint anIndex, GLint aSize, GLenum aType, GLboolean isNormalized, GLsizei aStride, const GLvoid* aPointer){
  glVertexAttribPointer(anIndex, aSize, aType, isNormalized, aStride, aPointer);
}

void GLW::UseProgram(GLuint aProgram) {
  glUseProgram(aProgram);
}

void GLW::DeleteProgram(GLuint aProgram) {
  glDeleteProgram(aProgram);
}

void GLW::DeleteShader(GLuint aShaderId) {
  glDeleteShader(aShaderId);
}
