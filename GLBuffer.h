// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLResource.h"
#include "GL.h"

class GLBuffer : public GLResource 
{
public:
	static GLBuffer *New();
	virtual ~GLBuffer();

	// We use a seperate bind method for each available target. 
	// Future operations use the same target until a different Bind is called.
	void BindAsVertexData() { target = GL_ARRAY_BUFFER; Bind();	}
	void BindAsIndexData() { target = GL_ELEMENT_ARRAY_BUFFER; Bind(); }
	void Unbind();

	void SetData(int size, void *data, GLenum usage);
	void SetSubData(int offset, int size, void *data);
	// NOTE: clearing data before calling map can actually speed things up
	void Clear();
	void GetSubData(int offset, int size, void *data);

	void *Map(GLenum access);
	bool Unmap();

	int GetSize() { return size; }

protected:
	GLuint id;
	GLenum target;
	GLenum boundTo;
	int size;
	GLenum usage;
	void *mapped;

	GLBuffer();

	void Bind();

private:
	// Not implemented
	GLBuffer(const GLBuffer&);
	void operator=(const GLBuffer&);
};
