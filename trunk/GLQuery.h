// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLResource.h"
#include "GL.h"

class GLQuery : public GLResource
{
public:
	virtual ~GLQuery();

	void Start();
	void Stop();

	bool IsDone();
	virtual int GetCounterBits();

	int GetResulti();
	unsigned int GetResultui();

protected:
	GLuint id;
	GLenum target;

	GLQuery(GLenum type);
	
private:
	// Not implemented
	GLQuery(const GLQuery&);
	void operator=(const GLQuery&);
};
