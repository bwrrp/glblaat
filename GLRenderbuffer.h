#pragma once
#include "GLRendertarget.h"
#include "GL.h"

class GLRenderbuffer : public GLRendertarget {
public:
	static GLRenderbuffer *New(int width, int height, int format);
	virtual ~GLRenderbuffer();

protected:
	GLuint id;
	int format;

	GLRenderbuffer(int width, int height, int format);

	virtual void AttachToBoundFBO(int attachment);
	virtual void Allocate();
};
