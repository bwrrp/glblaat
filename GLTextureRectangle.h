#pragma once
#include "GLTexture.h"

class GLTextureRectangle : public GLTexture {
public:
	static GLTextureRectangle *New(int width, int height, int internalformat, int format, int type, void *data);

	~GLTextureRectangle();

	inline GLenum GetTextureTarget() { return GL_TEXTURE_RECTANGLE_ARB; }

protected:
	GLTextureRectangle(int width, int height, int internalformat);

	bool Allocate(int format, int type, void *data);
};
