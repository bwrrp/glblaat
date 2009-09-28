#include "GLBuffer.h"

#include "GLUtility.h"

#ifndef NDEBUG
#include <iostream>
#endif

#include <cassert>

using namespace std;

// ----------------------------------------------------------------------------
GLBuffer *GLBuffer::New() 
{
	if (GLEW_VERSION_1_5) 
	{
#ifndef NDEBUG
		cout << "GLBuffer: Using OpenGL 1.5" << endl;
#endif
		return new GLBuffer();
	} 
	else if (GLEW_ARB_vertex_buffer_object) 
	{
		// TODO: add an ARB version of these classes as fallback
#ifndef NDEBUG
		cerr << "GLBuffer: Falling back to ARB (not implemented!)" << endl;
#endif
		return 0;
	} 
	else 
	{
#ifndef NDEBUG
		cerr << "GLBuffer: Buffer objects not supported!" << endl;
#endif
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLBuffer::GLBuffer() 
: id(0), target(0), boundTo(GL_NONE), size(0), usage(GL_NONE), mapped(0)
{
	//cout << "GLBuffer: Constructor" << endl;
	glGenBuffers(1, &id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glGenBuffers");
#endif
}

// ----------------------------------------------------------------------------
GLBuffer::~GLBuffer() 
{
	//cout << "GLBuffer: Destructor" << endl;
	glDeleteBuffers(1, &id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glDeleteBuffers()");
#endif
}

// ----------------------------------------------------------------------------
void GLBuffer::Unbind() 
{ 
	if (!boundTo) return;
	glBindBuffer(target, 0);
	boundTo = GL_NONE;
}

// ----------------------------------------------------------------------------
void GLBuffer::SetData(int size, void *data, GLenum usage) 
{
	Bind();
	glBufferData(boundTo, size, data, usage);
	this->size = size;
	this->usage = usage;
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glBufferData()");
#endif
}

// ----------------------------------------------------------------------------
void GLBuffer::SetSubData(int offset, int size, void *data)
{
	Bind();
	glBufferSubData(boundTo, offset, size, data);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glBufferSubData()");
#endif
}

// ----------------------------------------------------------------------------
void GLBuffer::Clear()
{
	SetData(size, 0, usage); 
}

// ----------------------------------------------------------------------------
void GLBuffer::GetSubData(int offset, int size, void *data)
{
	Bind();
	glGetBufferSubData(boundTo, offset, size, data);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glGetBufferSubData()");
#endif
}

// ----------------------------------------------------------------------------
void *GLBuffer::Map(GLenum access) 
{
	// No need to map twice
	assert(!mapped);
	if (mapped) return 0;

	Bind();
	mapped = glMapBuffer(boundTo, access);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glMapBuffer()");
#endif
	return mapped;
}

// ----------------------------------------------------------------------------
bool GLBuffer::Unmap() 
{
	assert(mapped);
	if (!mapped) return false;

	// NOTE: if this returns false, data needs to be resubmitted
	GLboolean res = glUnmapBuffer(boundTo);
	mapped = 0;
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glUnmapBuffer()");
#endif
	// TODO: unbind buffer?
	return (res == GL_TRUE);
}

// ----------------------------------------------------------------------------
void GLBuffer::Bind()
{
	if (boundTo == target) return;
	if (boundTo) Unbind();
	glBindBuffer(target, id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glBindBuffer()");
#endif
	boundTo = target;
}
