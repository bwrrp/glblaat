#pragma once
#include "GLTexture.h"
#include "GL.h"

class GLTexture3D : public GLTexture 
{
public:
	// Factory for textures
	static GLTexture3D *New(int width, int height, int depth, 
		int internalformat, int format, int type, void *data);

	// Shadow existing factory
	static GLTexture3D *New(int width, int height, 
		int internalformat, int format, int type, void *data)
	{
		return New(width, height, 1, internalformat, format, type, data);
	}

	virtual ~GLTexture3D();

	inline virtual GLenum GetTextureTarget() { return GL_TEXTURE_3D; }

	inline int GetDepth() { return depth; }

protected:
	int depth;
	
	GLTexture3D(int width, int height, int depth, int internalformat);

	virtual bool Allocate(int format, int type, void *data);

private:
	// Not implemented
	GLTexture3D(const GLTexture3D&);
	void operator=(const GLTexture3D&);
};
