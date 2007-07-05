#pragma once
#include "GLRenderTexture2D.h"

class GLRenderTexture2DRectangle : public GLRenderTexture2D {
public:
	static GLRenderTexture2DRectangle *New(int width, int height, int internalformat, int format, int type);

	virtual ~GLRenderTexture2DRectangle();

protected:
	GLRenderTexture2DRectangle(int width, int height);

	virtual bool Allocate(int internalformat, int format, int type);
};
