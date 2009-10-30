// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLBuffer.h"
#include "GL.h"

class GLPixelBuffer : public GLBuffer
{
public:
	static GLPixelBuffer *New();
	virtual ~GLPixelBuffer();

	void BindAsPixelSink() { target = GL_PIXEL_PACK_BUFFER_ARB; Bind(); }
	void BindAsPixelSource() { target = GL_PIXEL_UNPACK_BUFFER_ARB; Bind(); }

protected:
	GLPixelBuffer();

private:
	// Not implemented
	GLPixelBuffer(const GLPixelBuffer&);
	void operator=(const GLPixelBuffer&);
};
