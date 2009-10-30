// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLRenderTexture2D.h"

class GLRenderTexture2DRectangle : public GLRenderTexture2D 
{
public:
	static GLRenderTexture2DRectangle *New(int width, int height, 
		int internalformat, int format, int type);

	virtual ~GLRenderTexture2DRectangle();

protected:
	GLRenderTexture2DRectangle(int width, int height);

	bool Allocate(int internalformat, int format, int type);

private:
	// Not implemented
	GLRenderTexture2DRectangle(const GLRenderTexture2DRectangle&);
	void operator=(const GLRenderTexture2DRectangle&);
};
