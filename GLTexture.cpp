#include "GLTexture.h"
#include "GLTextureRectangle.h"

#include <iostream>
#include "GLUtility.h"

using namespace std;

// ----------------------------------------------------------------------------
GLTexture *GLTexture::New(int width, int height, int internalformat, 
						  int format, int type, void *data) 
{
	// Check if width and height are powers of two
    unsigned short xs = (unsigned short)width;
    unsigned short ys = (unsigned short)height;
    while (!(xs & 0x01)) 
	{
		xs = xs >> 1;
    }
    while (!(ys & 0x01)) 
	{
		ys = ys >> 1;
	}
    if ((xs > 1)||(ys > 1)) 
	{
		// Non-power-of-two sizes, check available extensions
		if (GLEW_ARB_texture_non_power_of_two) 
		{
			//cout << "GLTexture: sizes are not powers of two, creating NPOTS texture" << endl;
			GLTexture *tex = new GLTexture(width, height, internalformat);
			if (!tex->Allocate(format, type, data)) 
			{
				delete tex;
				return 0;
			}
			return tex;
		} 
		// Try a rectangle texture, note that these use different texture coordinates!
		if (GLEW_ARB_texture_rectangle) 
		{
			cerr << "GLTexture: warning, using rectangle texture for NPOTS texture" << endl;
			return GLTextureRectangle::New(width, height, internalformat, format, type, data);
		}
		cerr << "GLTexture: non-power-of-two sized textures not supported" << endl;
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
		cerr << "GLTexture: Proxy allocation failed, may be out of video memory" << endl;
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
		cerr << "GLTexture: An OpenGL error occurred while allocating the texture" << endl;
		GLUtility::ClearOpenGLError();
		return false;
	}

	return true;
}
