#include "GLW.h"
#include <vector>

using namespace Engine;

GLW::GLW() {
	throw ERuntimeException("You cannot directly initialize OpenGL Wrapper class (Engine::GLW)");
}

void GLW::test() {

}

GLuint GLW::CreateProgram() {
	return glCreateProgram();
}

GLuint GLW::CreateShader(GLuint aShader) {
	return glCreateShader(aShader);
}

void GLW::ShaderSource(GLuint aShader, GLsizei aCount, const GLchar** aSource, const GLint* aLength) {
	glShaderSource(aShader, aCount, aSource, aLength);
}

void GLW::CompileShader(GLuint aShader) {
	glCompileShader(aShader);
	GLint isShaderCompiled = GL_FALSE;
	GLW::GetShaderiv(aShader, GL_COMPILE_STATUS, &isShaderCompiled);
	if (isShaderCompiled != GL_TRUE) {
		char* theError = new char[200];
		GLint logSize = 0;
		GLW::GetShaderiv(aShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<GLchar> errorLog(logSize);
		glGetShaderInfoLog(aShader, logSize, &logSize, &errorLog[0]);
		sprintf_s(theError, sizeof(theError) + 30, "Unable to compile vertex shader %d", aShader);
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
	GLW::GetProgramiv(aProgram, GL_LINK_STATUS, &isLinkValid);
	if (isLinkValid != GL_TRUE) {
		char* theError = new char[200];
		sprintf_s(theError, sizeof(theError) + 30, "Error linking program %d", aProgram);
		throw ERuntimeException(theError);
	}
}

void GLW::GetProgramiv(GLuint aProgram, GLenum aParameterName, GLint* aParameter) {
	glGetProgramiv(aProgram, aParameterName, aParameter);
}

GLint GLW::GetAttribLocation(GLuint aProgram, const GLchar* aName) {
	GLint theAttribLocation = glGetAttribLocation(aProgram, aName);
	if (theAttribLocation == -1) {
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

void GLW::GenTextures(GLsizei aSize, GLuint* someTextures) {
	glGenTextures(aSize, someTextures);
}

void GLW::BindTexture(GLenum aTarget, GLuint aTexture) {
	glBindTexture(aTarget, aTexture);
}

void GLW::TexImage2D(GLenum aTarget, GLint aLevel, GLint anInternalFormat, GLsizei aWidth, GLsizei aHeight, GLint aBorder, GLenum aFormat, GLenum aType, const GLvoid* aData) {
	glTexImage2D(aTarget, aLevel, anInternalFormat, aWidth, aHeight, aBorder, aFormat, aType, aData);
}

void GLW::TexParameteri(GLenum aTarget, GLenum aParameterName, GLint aParameter) {
	glTexParameteri(aTarget, aParameterName, aParameter);
}

void GLW::Enable(GLenum aCap) {
	glEnable(aCap);
}

void GLW::BlendFunc(GLenum anSFactor, GLenum aDFactor) {
	glBlendFunc(anSFactor, aDFactor);
}

GLint GLW::GetUniformLocation(GLuint aProgramId, const GLchar *aName) {
	return glGetUniformLocation(aProgramId, aName);
}

void GLW::ActiveTexture(GLenum aTexture) {
	glActiveTexture(aTexture);
}

void GLW::Uniform1i(GLint aLocation, GLint aValue) {
	glUniform1i(aLocation, aValue);
}

void GLW::UniformMatrix4fv(GLint aLocation, GLsizei aCount, GLboolean aTranspose, const GLfloat *aValue) {
	glUniformMatrix4fv(aLocation, aCount, aTranspose, aValue);
}

void GLW::DrawArrays(GLenum aMode, GLint aStart, GLsizei aCount) {
	glDrawArrays(aMode, aStart, aCount);
}