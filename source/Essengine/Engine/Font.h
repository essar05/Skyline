#ifndef FONT_H
#define FONT_H
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Engine {
	class Font {
	public:
		Font(const char* aFilePath, GLuint aFontSize);
		virtual ~Font();
		static bool initFreeType();
		GLuint GetId();
	protected:
		void Load(const char* aFilePath, GLuint aFontSize);
	private:
		GLuint id;

		static FT_Library library;

		GLfloat mSpace;
		GLfloat mLineHeight;
		GLfloat mNewLine;
	};
};

#endif // FONT_H
