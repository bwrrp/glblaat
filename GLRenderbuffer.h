// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLRendertarget.h"
#include "GL.h"

class GLRenderbuffer : public GLRendertarget 
{
public:
	static GLRenderbuffer *New(int width, int height, int format);
	virtual ~GLRenderbuffer();

	virtual bool Resize(int width, int height);

protected:
	GLuint id;
	int format;

	GLRenderbuffer(int width, int height, int format);

	virtual void AttachToBoundFBO(int attachment);

	virtual void Allocate();

private:
	// Not implemented
	GLRenderbuffer(const GLRenderbuffer&);
	void operator=(const GLRenderbuffer&);
};
