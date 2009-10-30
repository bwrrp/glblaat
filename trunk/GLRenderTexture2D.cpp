// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#include "GLRenderTexture2D.h"

#include "GLUtility.h"

// ----------------------------------------------------------------------------
GLRenderTexture2D *GLRenderTexture2D::New(
	int width, int height, int internalformat, int format, int type) 
{
	GLRenderTexture2D *rt = new GLRenderTexture2D(width, height);
	// Allocate memory
	if (!rt->Allocate(internalformat, format, type)) 
	{
		delete rt;
		return 0;
	}

	return rt;
}

// ----------------------------------------------------------------------------
GLRenderTexture2D::GLRenderTexture2D(int width, int height)
: GLRendertarget(width, height), tex(0) 
{ 
}

// ----------------------------------------------------------------------------
GLRenderTexture2D::~GLRenderTexture2D() 
{
	if (tex) delete tex;
}

// ----------------------------------------------------------------------------
bool GLRenderTexture2D::Resize(int width, int height)
{
	// Get texture metadata
	int internalformat = tex->GetInternalFormat();
	int dataformat = tex->GetDataFormat();
	int datatype = tex->GetDataType();

	// Delete the existing texture
	if (tex) delete tex;
	
	this->width = width;
	this->height = height;

	// Allocate a new texture
	return Allocate(internalformat, dataformat, datatype);
}

// ----------------------------------------------------------------------------
void GLRenderTexture2D::AttachToBoundFBO(int attachment) 
{
	GLRendertarget::AttachToBoundFBO(attachment);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
		attachment, tex->GetTextureTarget(), tex->id, 0);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLRenderTexture2D: AttachToBoundFBO()");
#endif
}

// ----------------------------------------------------------------------------
bool GLRenderTexture2D::Allocate(int internalformat, int format, int type) 
{
	// Create an empty texture
	tex = GLTexture::New(width, height, internalformat, format, type, 0);
	if (!tex) 
	{
		return false;
	}

	return true;
}
