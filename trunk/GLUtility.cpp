#include "GLUtility.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include "GL.h"
#include "GLTextureRectangle.h"

using namespace std;

GLUtility::GLUtility() { }

GLUtility::~GLUtility() { }

// ----------------------------------------------------------------------------
GLTextureRectangle *GLUtility::GrabColorBuffer(int vpx, int vpy, 
											   int vpwidth, int vpheight, 
											   GLTextureRectangle *oldtex) 
{
	// Can we use an existing texture?
	GLTextureRectangle *tex = oldtex;
	assert(tex->GetHeight() == vpheight && tex->GetWidth() == vpwidth);

	if (!tex)
	{
		// Create the texture
		tex = GLTextureRectangle::New(vpwidth, vpheight, 
			GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		if (!tex)
		{
			cerr << "GLUtility: GrabColorBuffer failed to create texture!" << endl;
			return tex;
		}
		// Set texture parameters
		tex->BindToCurrent();
		glTexParameteri(tex->GetTextureTarget(), 
			GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(tex->GetTextureTarget(), 
			GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		tex->BindToCurrent();
	}
	

	// Capture RGBA buffer
	glCopyTexSubImage2D(tex->GetTextureTarget(), 0, 0, 0, 
		vpx, vpy, vpwidth, vpheight);

	// Done
	tex->UnbindCurrent();
	GLUtility::CheckOpenGLError("GLUtility: GrabColorBuffer");
	return tex;
}

// ----------------------------------------------------------------------------
GLTextureRectangle *GLUtility::GrabDepthBuffer(int vpx, int vpy, 
											   int vpwidth, int vpheight, 
											   GLTextureRectangle *oldtex) 
{
	// get z bits
	GLint depthBits;
	GLint depthformat;
	glGetIntegerv(GL_DEPTH_BITS,&depthBits);
	if(depthBits==16)
	{
		depthformat = GL_DEPTH_COMPONENT16;
	}
	else
	{
		depthformat = GL_DEPTH_COMPONENT24;
	}

	// Can we use an existing texture?
	GLTextureRectangle *tex = oldtex;
	assert(tex->GetHeight() == vpheight && tex->GetWidth() == vpwidth);

	if (!tex)
	{
		tex = GLTextureRectangle::New(vpwidth, vpheight, 
			depthformat, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		if (!tex) 
		{
			cerr << "GLUtility: GrabDepthBuffer failed to create texture!" << endl;
			return tex;
		}
		// Set texture parameters
		tex->BindToCurrent();
		glTexParameteri(tex->GetTextureTarget(), 
			GL_TEXTURE_MIN_FILTER,	GL_NEAREST);
		glTexParameteri(tex->GetTextureTarget(), 
			GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(tex->GetTextureTarget(), 
			GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(tex->GetTextureTarget(), 
			GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(tex->GetTextureTarget(), 
			GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(tex->GetTextureTarget(), 
			GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	}
	else
	{
		tex->BindToCurrent();
	}
	
	// Capture depth buffer
	glCopyTexSubImage2D(tex->GetTextureTarget(), 0, 0, 0, 
		vpx, vpy, vpwidth, vpheight);

	// Done
	tex->UnbindCurrent();
	GLUtility::CheckOpenGLError("GLUtility: GrabDepthBuffer");
	return tex;
}

// ----------------------------------------------------------------------------
bool GLUtility::ErrorFlag = false;

// ----------------------------------------------------------------------------
void GLUtility::CheckOpenGLError(const std::string &task) 
{
	GLenum err = glGetError();
	// Make sure we get all error flags before we continue, but don't loop more than 100 times
	int i = 0;
	while (err != GL_NO_ERROR && i < 10) {
		ErrorFlag = true;
		const char *errstring = reinterpret_cast<const char*>(gluErrorString(err));
		cerr << task << " FAILED (err[" << (i+1) << "]=" << err << 
			", \"" << (errstring ? errstring : "???") << "\")" << endl;
		//assert(false);
		err = glGetError();
		++i;
		if (i == 10) {
			// This should not happen, but apparently it does
			// glGetError should return 0 if it generates an error itself
			cerr << "GLUtility: 10 consecutive errors reached, are you misssing a glEnd()?";
		}
	}
}

// ----------------------------------------------------------------------------
void GLUtility::ClearOpenGLError() 
{
	// Loop until there are no more errors
	int i = 0;
	while (glGetError() != GL_NO_ERROR && i < 10) { 
		++i;
		if (i == 10) {
			// This should not happen, but apparently it does
			// glGetError should return 0 if it generates an error itself
			cerr << "GLUtility: 10 consecutive errors reached, are you missing a glEnd()?";
		}
	}
	ErrorFlag = false;
}

// ----------------------------------------------------------------------------
bool GLUtility::GetErrorFlag() 
{
	return ErrorFlag;
}
