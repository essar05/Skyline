#include "SDLW.h"
#include "GLW.h"
#include "Texture.h"

using namespace Engine;

Texture::Texture(const char* aFilePath) {
  this->Load(aFilePath);
}

Texture::~Texture() {}

GLuint Texture::GetId() {
  return this->id;
}

int Texture::GetHeight() {
  return this->height;
}

int Texture::GetWidth() {
  return this->height;
}

int Texture::GetMode() {
  return this->mode;
}

void Texture::Load(const char* aFilePath) {

  SDL_Surface* theSurface = SDLW::ImageLoad(aFilePath);
  if (theSurface->format->BytesPerPixel == 3) { // RGB 24bit
    this->mode = GL_RGB;
  } else if (theSurface->format->BytesPerPixel == 4) { // RGBA 32bit
    this->mode = GL_RGBA;
  } else {
    SDL_FreeSurface(theSurface);
    char* theError = new char[200];
    sprintf(theError, "Unknown image format: %s", aFilePath);
    throw ERuntimeException(theError);
  }

  this->width = theSurface->w;
  this->height = theSurface->h;

  GLW::GenTextures(1, &this->id);
  GLW::BindTexture(GL_TEXTURE_2D, this->id);
  GLW::TexImage2D(GL_TEXTURE_2D, 0, this->mode, this->width, this->height, 0, this->mode, GL_UNSIGNED_BYTE, theSurface->pixels);

  GLW::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GLW::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  SDLW::FreeSurface(theSurface);
}

void Texture::loadBMP_custom(const char * imagepath) {

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file){
    printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
    return;
  }

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){
		printf("Not a correct BMP file\n");
		return;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		return;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file wan be closed
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	this->id = textureID;
}
