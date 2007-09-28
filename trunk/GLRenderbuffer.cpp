#include "GLRenderbuffer.h"

#include "GLUtility.h"

GLRenderbuffer *GLRenderbuffer::New(int width, int height, int format) 
{
	GLRenderbuffer *rb = new GLRenderbuffer(width, height, format);
	// Allocate storage
	rb->Allocate();

	return rb;
}

GLRenderbuffer::GLRenderbuffer(int width, int height, int format) 
: GLRendertarget(width, height), id(0), format(format) 
{
	// Create the renderbuffer
	glGenRenderbuffersEXT(1, &id);
	GLUtility::CheckOpenGLError("GLRenderbuffer: glGenRenderbuffersEXT()");
}

GLRenderbuffer::~GLRenderbuffer() 
{
	// Delete the renderbuffer
	glDeleteRenderbuffersEXT(1, &id);
	GLUtility::CheckOpenGLError("GLRenderbuffer: glDeleteRenderbuffersEXT()");
}

void GLRenderbuffer::AttachToBoundFBO(int attachment) 
{
	GLRendertarget::AttachToBoundFBO(attachment);

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, 
		attachment, GL_RENDERBUFFER_EXT, id);
	GLUtility::CheckOpenGLError("GLRenderbuffer: AttachToBoundFBO()");
}

bool GLRenderbuffer::Resize(int width, int height)
{
	// Resize the renderbuffer object's data store
	this->width = width;
	this->height = height;
	this->Allocate();

	if (GLUtility::GetErrorFlag()) return false;

	return true;
}

void GLRenderbuffer::Allocate() 
{
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, id);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
		format, width, height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	GLUtility::CheckOpenGLError("GLRenderbuffer: Allocate()");
}
