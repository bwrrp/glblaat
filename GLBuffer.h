#pragma once
#include "GLResource.h"
#include "GLProgram.h"
#include "GL.h"
#include <string>

class GLBuffer : public GLResource 
{
public:
	// TODO: decide if we want to have VertexBuffer / PixelBuffer subclasses 
	// (note that they are really the same thing, the PBO just adds targets)

	GLBuffer();
	virtual ~GLBuffer();

	// TODO: maybe we can use several bind methods for different applications of the buffer
	// e.g., BindForRead, BindForWrite, BindForPixelRead, ... Need to look at docs
	// Binding for setting / getting buffer data should not require the user to call bind...
	void BindAsVertexData() 
	{
		target = GL_ARRAY_BUFFER;
		Bind();
	}

	void BindAsIndexData()
	{
		target = GL_ELEMENT_ARRAY_BUFFER;
		Bind();
	}

	void Unbind() 
	{ 
		if (!boundTo) return;
		glBindBuffer(target, 0);
		boundTo = 0;
	}

	void SetData(int size, void *data, GLenum usage) 
	{
		Bind();
		glBufferData(target, size, data, usage);
		this->size = size;
		this->usage = usage;
	}

	void SetSubData(int offset, int size, void *data)
	{
		Bind();
		glBufferSubData(target, offset, size, data);
	}

	void Clear() 
	{
		// NOTE: clearing data before calling map can actually speed things up
		SetData(size, 0, usage); 
	}

	void *Map(GLenum access) 
	{
		Bind();
		return glMapBuffer(target, access);
	}

	bool Unmap() 
	{
		// NOTE: if this returns false, data needs to be resubmitted
		return (glUnmapBuffer(target) == GL_TRUE);
		// TODO: unbind buffer?
	}

	// TODO: glVertexPointer, glNormalPointer, glWhateverPointer - handle these here?

	int GetSize() 
	{ 
		return size; 
	}

protected:
	GLuint id;
	GLenum target;
	GLenum boundTo;

	int size;
	GLenum usage;

	void Bind()
	{
		if (boundTo == target) return;
		if (boundTo) Unbind();
		glBindBuffer(target, id);
		boundTo = target;
	}

private:
	// Not implemented
	GLBuffer(const GLBuffer&);
	void operator=(const GLBuffer&);
};
