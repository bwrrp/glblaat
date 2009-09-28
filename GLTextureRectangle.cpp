#include "GLTextureRectangle.h"

#include "GLUtility.h"

#ifndef NDEBUG
#include <iostream>
#endif

using namespace std;

// ----------------------------------------------------------------------------
GLTextureRectangle *GLTextureRectangle::New(
	int width, int height, int internalformat, 
	int format, int type, void *data) 
{
	// Check if rectangular textures are supported
	if (GLEW_ARB_texture_rectangle) 
	{
		GLTextureRectangle *tex = new GLTextureRectangle(
			width, height, internalformat);
		if (!tex->Allocate(format, type, data)) 
		{
			delete tex;
			return 0;
		}
		return tex;
	} 
	else 
	{
#ifndef NDEBUG
		cerr << "GLTextureRectangle: rectangular textures not supported" 
			<< endl;
#endif
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLTextureRectangle::GLTextureRectangle(
	int width, int height, int internalformat)
: GLTexture(width, height, internalformat) 
{ 
}

// ----------------------------------------------------------------------------
GLTextureRectangle::~GLTextureRectangle() 
{ 
}

// ----------------------------------------------------------------------------
bool GLTextureRectangle::Allocate(int format, int type, void *data) 
{
	// Store old binding to avoid messing up the state
	glPushAttrib(GL_TEXTURE_BIT);

	// Store new params
	this->dataformat = format;
	this->datatype = type;

	BindToCurrent();

	// Set texture object parameters
	glTexParameterf(GetTextureTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf(GetTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf(GetTextureTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf(GetTextureTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// Try a proxy allocation to check available memory and parameters
	glTexImage2D(GL_PROXY_TEXTURE_RECTANGLE_ARB, 0, internalformat, 
		width, height, 0, format, type, data);
	GLint w;
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_RECTANGLE_ARB, 0, 
		GL_TEXTURE_WIDTH, &w);

	GLUtility::CheckOpenGLError(
		"GLTextureRectangle: Allocate() - proxy allocation");

	bool eflag = GLUtility::GetErrorFlag();
	if (w == 0 || eflag) 
	{
#ifndef NDEBUG
		cerr << "GLTextureRectangle: "
			<< "Proxy allocation failed, out of video memory?" << endl;
#endif
		UnbindCurrent();
		glPopAttrib();
		return false;
	}
	
	// Allocate the texture
	glTexImage2D(GetTextureTarget(), 0, internalformat, 
		width, height, 0, format, type, data);
	GLUtility::CheckOpenGLError("GLTextureRectangle: Allocate - glTexImage2D");

	UnbindCurrent();
	glPopAttrib();

	if (GLUtility::GetErrorFlag()) 
	{
#ifndef NDEBUG
		cerr << "GLTextureRectangle: "
			<< "An error occurred while allocating the texture" << endl;
#endif
		GLUtility::ClearOpenGLError();
		return false;
	}

	return true;
}
