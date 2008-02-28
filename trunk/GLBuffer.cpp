#include "GLBuffer.h"

#include "GLUtility.h"

using namespace std;

// ----------------------------------------------------------------------------
GLBuffer::GLBuffer() 
: id(0), target(0), boundTo(GL_NONE), size(0)
{
	glGenBuffers(1, &id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glGenBuffers");
#endif
}

// ----------------------------------------------------------------------------
GLBuffer::~GLBuffer() 
{
	glDeleteBuffers(1, &id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLShader: glDeleteBuffers()");
#endif
}
