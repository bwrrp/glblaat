#pragma once
#include "GLBuffer.h"
#include "GL.h"

class GLPixelBuffer : public GLBuffer
{
public:
	static GLPixelBuffer *New();
	virtual ~GLPixelBuffer();

	void BindAsPixelSource() { target = GL_PIXEL_UNPACK_BUFFER; Bind(); }
	void BindAsPixelSink() { target = GL_PIXEL_PACK_BUFFER; Bind(); }

protected:
	GLPixelBuffer();

private:
	// Not implemented
	GLPixelBuffer(const GLPixelBuffer&);
	void operator=(const GLPixelBuffer&);
};
