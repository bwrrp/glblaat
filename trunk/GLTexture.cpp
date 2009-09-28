#include "GLTexture.h"
#include "GLTextureRectangle.h"

#include "GLUtility.h"

#ifndef NDEBUG
#include <iostream>
#endif

using namespace std;

// ----------------------------------------------------------------------------
GLTexture *GLTexture::New(int width, int height, int internalformat, 
	int format, int type, void *data) 
{
	if (width == 0) width = 1;
	if (height == 0) height = 1;
	// Check if width and height are powers of two
    unsigned int xs = static_cast<unsigned int>(width);
    unsigned int ys = static_cast<unsigned int>(height);
    if ((xs & (xs - 1)) || (ys & (ys - 1))) 
	{
		// Non-power-of-two sizes, check available extensions
		if (GLEW_ARB_texture_non_power_of_two) 
		{
			// Standard textures can have non-power-of-two sizes
			GLTexture *tex = new GLTexture(width, height, internalformat);
			if (!tex->Allocate(format, type, data)) 
			{
				delete tex;
				return 0;
			}
			return tex;
		}
		// Try a rectangle texture, these use different texture coordinates!
		if (GLEW_ARB_texture_rectangle) 
		{
#ifndef NDEBUG
			cerr << "GLTexture: warning, using rectangle texture "
				<< "for NPOTS texture" << endl;
#endif
			return GLTextureRectangle::New(width, height, 
				internalformat, format, type, data);
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
		GLTexture *tex = new GLTexture(width, height, internalformat);
		if (!tex->Allocate(format, type, data)) 
		{
			delete tex;
			return 0;
		}
		return tex;
	}
}

// ----------------------------------------------------------------------------
GLTexture::GLTexture(int width, int height, int internalformat)
: id(0), width(width), height(height), internalformat(internalformat), 
	dataformat(0), datatype(0) 
{
	//cout << "GLTexture: Constructor" << endl;
	// Create the texture object
	glGenTextures(1, &id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLTexture: glGenTextures()");
#endif
}

// ----------------------------------------------------------------------------
GLTexture::~GLTexture() 
{
	//cout << "GLTexture: Destructor" << endl;
	// Delete the texture object
	glDeleteTextures(1, &id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLTexture: glDeleteTextures()");
#endif
}

// ----------------------------------------------------------------------------
void GLTexture::BindToCurrent() 
{
	glBindTexture(GetTextureTarget(), id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLTexture: BindToCurrent()");
#endif
}

// ----------------------------------------------------------------------------
void GLTexture::UnbindCurrent() 
{
	glBindTexture(GetTextureTarget(), 0);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLTexture: UnbindCurrent()");
#endif
}

// ----------------------------------------------------------------------------
bool GLTexture::Allocate(int format, int type, void *data) 
{
	//cout << "GLTexture: Allocate" << endl;
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
	glTexImage2D(GL_PROXY_TEXTURE_2D, 0, internalformat, 
		width, height, 0, format, type, data);
	GLint w;
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);

	GLUtility::CheckOpenGLError("GLTexture: Allocate() - proxy allocation");

	bool eflag = GLUtility::GetErrorFlag();
	if (w == 0 || eflag) 
	{
#ifndef NDEBUG		
		cerr << "GLTexture: Proxy allocation failed, out of video memory?" 
			<< endl;
#endif
		UnbindCurrent();
		glPopAttrib();
		return false;
	}
	
	// Allocate the texture
	glTexImage2D(GetTextureTarget(), 0, internalformat, 
		width, height, 0, format, type, data);
	GLUtility::CheckOpenGLError("GLTexture: Allocate - glTexImage2D");

	UnbindCurrent();
	glPopAttrib();

	if (GLUtility::GetErrorFlag()) 
	{
#ifndef NDEBUG
		cerr << "GLTexture: An error occurred while allocating the texture" 
			<< endl;
#endif
		GLUtility::ClearOpenGLError();
		return false;
	}

	return true;
}
