#pragma once
#include "GLRendertarget.h"
#include "GLTexture.h"
#include "GL.h"

class GLRenderTexture2D : public GLRendertarget {
public:
	static GLRenderTexture2D *New(int width, int height, int internalformat, int format, int type);

	virtual ~GLRenderTexture2D();

	virtual GLTexture* GetTexture();

protected:
	GLTexture *tex;

	GLRenderTexture2D(int width, int height);

	virtual void AttachToBoundFBO(int attachment);
	virtual bool Allocate(int internalformat, int format, int type);
};
