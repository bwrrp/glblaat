#pragma once
#include "GLResource.h"
#include "GL.h"

class GLTexture : public GLResource {
public:
	friend class GLRenderTexture2D;

	// Factory for textures
	static GLTexture *New(int width, int height, int internalformat, int format, int type, void *data);

	virtual ~GLTexture();

	virtual void BindToCurrent();
	virtual void UnbindCurrent();

	inline virtual GLenum GetTextureTarget() { return GL_TEXTURE_2D; }

	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
	inline int GetInternalFormat() { return internalformat; }

protected:
	GLuint id;
	int width;
	int height;
	int internalformat;

	GLTexture(int width, int height, int internalformat);

	virtual bool Allocate(int format, int type, void *data);
};
