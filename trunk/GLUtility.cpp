#include "GLUtility.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include "GL.h"
#include "GLTexture.h"
#include "GLTextureRectangle.h"

using namespace std;

GLUtility::GLUtility() 
{ 
}

GLUtility::~GLUtility() 
{ 
}

// ----------------------------------------------------------------------------
GLTexture *GLUtility::GrabColorBuffer(int vpx, int vpy, 
									  int vpwidth, int vpheight, 
									  GLTexture *oldtex) 
{
	// Can we use an existing texture?
	GLTexture *tex = oldtex;

	if (!tex)
	{
		// Create the texture
		tex = GLTexture::New(vpwidth, vpheight, 
			GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		if (!tex)
		{
			cerr << "GLUtility: GrabColorBuffer failed to create texture!" << endl;
			return tex;
		}

		// This binds the texture
		SetDefaultColorTextureParameters(tex);
	}
	else
	{
		tex->BindToCurrent();
	}
	
	assert(tex->GetHeight() == vpheight && tex->GetWidth() == vpwidth);

	// Capture RGBA buffer
	glCopyTexSubImage2D(tex->GetTextureTarget(), 0, 0, 0, 
		vpx, vpy, vpwidth, vpheight);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLUtility: GrabColorBuffer");
#endif

	// Done
	tex->UnbindCurrent();
	return tex;
}

// ----------------------------------------------------------------------------
GLTextureRectangle *GLUtility::GrabColorBufferRectangle(int vpx, int vpy, 
														int vpwidth, int vpheight, 
														GLTextureRectangle *oldtex) 
{
	// Can we use an existing texture?
	GLTextureRectangle *tex = oldtex;

	if (!tex)
	{
		// Create the texture
		tex = GLTextureRectangle::New(vpwidth, vpheight, 
			GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		if (!tex)
		{
			cerr << "GLUtility: GrabColorBufferRectangle failed to create texture!" 
				<< endl;
			return tex;
		}

		SetDefaultColorTextureParameters(tex);
		//tex->UnbindCurrent();
	}

	// Use this texture (rectangle) to grab the buffer
	tex = dynamic_cast<GLTextureRectangle*>(
		GrabColorBuffer(vpx, vpy, vpwidth, vpheight, tex));
	assert(tex);
	return tex;
}

// ----------------------------------------------------------------------------
GLTexture *GLUtility::GrabDepthBuffer(int vpx, int vpy, 
									  int vpwidth, int vpheight, 
									  GLTexture *oldtex) 
{
	// Can we use an existing texture?
	GLTexture *tex = oldtex;

	if (!tex)
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

		// Create a new texture
		tex = GLTexture::New(vpwidth, vpheight, depthformat, 
			GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		if (!tex) 
		{
			cerr << "GLUtility: GrabDepthBuffer failed to create texture!" << endl;
			return tex;
		}

		// This binds the texture
		SetDefaultDepthTextureParameters(tex);
	}
	else
	{
		tex->BindToCurrent();
	}

	assert(tex->GetHeight() == vpheight && tex->GetWidth() == vpwidth);

	// Capture depth buffer
	glCopyTexSubImage2D(tex->GetTextureTarget(), 0, 0, 0, 
		vpx, vpy, vpwidth, vpheight);

	// Done
	tex->UnbindCurrent();
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLUtility: GrabDepthBuffer");
#endif
	return tex;
}

// ----------------------------------------------------------------------------
GLTextureRectangle *GLUtility::GrabDepthBufferRectangle(int vpx, int vpy, 
														int vpwidth, int vpheight, 
														GLTextureRectangle *oldtex) 
{
	// Can we use an existing texture?
	GLTextureRectangle *tex = oldtex;

	if (!tex)
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

		// Create a new texture
		tex = GLTextureRectangle::New(vpwidth, vpheight, 
			depthformat, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		if (!tex) 
		{
			cerr << "GLUtility: GrabDepthBufferRectangle failed to create texture!" 
				<< endl;
			return tex;
		}
		
		SetDefaultDepthTextureParameters(tex);
		//tex->UnbindCurrent();
	}
	
	// Use this texture (rectangle) to capture the buffer
	tex = dynamic_cast<GLTextureRectangle*>(
		GrabDepthBuffer(vpx, vpy, vpwidth, vpheight, tex));
	assert(tex);
	return tex;
}

// ----------------------------------------------------------------------------
void GLUtility::SetDefaultColorTextureParameters(GLTexture *tex)
{
	tex->BindToCurrent();
	glTexParameteri(tex->GetTextureTarget(), 
		GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(tex->GetTextureTarget(), 
		GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(tex->GetTextureTarget(), 
		GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(tex->GetTextureTarget(), 
		GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

// ----------------------------------------------------------------------------
void GLUtility::SetDefaultDepthTextureParameters(GLTexture *tex)
{
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

// ----------------------------------------------------------------------------
bool GLUtility::ErrorFlag = false;

// ----------------------------------------------------------------------------
void GLUtility::CheckOpenGLError(const std::string &task) 
{
	// Clear the previous error flag
	ErrorFlag = false;

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
			cerr << "GLUtility: 10 consecutive errors reached, are you missing a glEnd()?";
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
