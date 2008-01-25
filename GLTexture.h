#pragma once
#include "GLResource.h"
#include "GL.h"

class GLTexture : public GLResource 
{
public:
	friend class GLRenderTexture2D;

	// Factory for textures
	static GLTexture *New(int width, int height, int internalformat, int format, int type, void *data);

	virtual ~GLTexture();

	virtual void BindToCurrent();
	virtual void UnbindCurrent();

	inline virtual GLenum GetTextureTarget() const { return GL_TEXTURE_2D; }

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline int GetInternalFormat() const { return internalformat; }

	inline int GetDataFormat() const { return dataformat; }
	inline int GetDataType() const { return datatype; }

protected:
	GLuint id;
	int width;
	int height;
	int internalformat;
	
	int dataformat;
	int datatype;

	GLTexture(int width, int height, int internalformat);

	virtual bool Allocate(int format, int type, void *data);

private:
	// Not implemented
	GLTexture(const GLTexture&);
	void operator=(const GLTexture&);
};
