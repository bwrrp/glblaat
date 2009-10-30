// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#include "GLTexture3D.h"
#include "GLTextureRectangle.h"

#include "GLUtility.h"

#ifndef NDEBUG
#include <iostream>
#endif

using namespace std;

// ----------------------------------------------------------------------------
GLTexture3D *GLTexture3D::New(int width, int height, int depth, 
	int internalformat, int format, int type, void *data) 
{
	// Check if width and height are powers of two
    unsigned int xs = static_cast<unsigned int>(width);
    unsigned int ys = static_cast<unsigned int>(height);
	unsigned int zs = static_cast<unsigned int>(depth);
    if ((xs & (xs - 1)) || (ys & (ys - 1)) || (zs & (zs - 1))) 
	{
		// Non-power-of-two sizes, check available extensions
		if (GLEW_ARB_texture_non_power_of_two) 
		{
			GLTexture3D *tex = new GLTexture3D(
				width, height, depth, internalformat);
			if (!tex->Allocate(format, type, data)) 
			{
				delete tex;
				return 0;
			}
			return tex;
		}
#ifndef NDEBUG
		cerr << "GLTexture: non-power-of-two sized textures not supported" 
			<< endl;
#endif
		return 0;
	} 
	else 
	{
		// Create a normal texture
		GLTexture3D *tex = new GLTexture3D(
			width, height, depth, internalformat);
		if (!tex->Allocate(format, type, data)) 
		{
			delete tex;
			return 0;
		}
		return tex;
	}
}

// ----------------------------------------------------------------------------
GLTexture3D::GLTexture3D(int width, int height, int depth, int internalformat)
: GLTexture(width, height, internalformat), depth(depth)
{
}

// ----------------------------------------------------------------------------
GLTexture3D::~GLTexture3D() 
{
}

// ----------------------------------------------------------------------------
bool GLTexture3D::Allocate(int format, int type, void *data) 
{
	// Store old binding to avoid messing up the state
	glPushAttrib(GL_TEXTURE_BIT);

	// Store new params
	this->dataformat = format;
	this->datatype = type;

	BindToCurrent();

	// Set texture object parameters
	glTexParameterf(GetTextureTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GetTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GetTextureTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GetTextureTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GetTextureTarget(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Try a proxy allocation to check available memory and parameters
	glTexImage3D(GL_PROXY_TEXTURE_3D, 0, internalformat, 
		width, height, depth, 0, format, type, data);
	GLint w;
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &w);
	
	GLUtility::CheckOpenGLError("GLTexture3D: Allocate() - proxy allocation");
	
	bool eflag = GLUtility::GetErrorFlag();
	if (w == 0 || eflag) 
	{
#ifndef NDEBUG
		cerr << "GLTexture3D: Proxy allocation failed, out of video memory?" 
			<< endl;
#endif
		UnbindCurrent();
		glPopAttrib();
		return false;
	}
	
	// Allocate the texture
	glTexImage3D(GetTextureTarget(), 0, internalformat, 
		width, height, depth, 0, format, type, data);
	GLUtility::CheckOpenGLError("GLTexture3D: Allocate - glTexImage3D");

	UnbindCurrent();
	glPopAttrib();

	if (GLUtility::GetErrorFlag()) 
	{
#ifndef NDEBUG
		cerr << "GLTexture3D: An error occurred while allocating the texture" 
			<< endl;
#endif
		GLUtility::ClearOpenGLError();
		return false;
	}

	return true;
}
