// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLTexture.h"

class GLTextureRectangle : public GLTexture 
{
public:
	static GLTextureRectangle *New(
		int width, int height, int internalformat, 
		int format, int type, void *data);

	~GLTextureRectangle();

	inline GLenum GetTextureTarget() const { return GL_TEXTURE_RECTANGLE_ARB; }

protected:
	GLTextureRectangle(int width, int height, int internalformat);

	bool Allocate(int format, int type, void *data);

private:
	// Not implemented
	GLTextureRectangle(const GLTextureRectangle&);
	void operator=(const GLTextureRectangle&);
};
