#include "GLRenderTexture2DRectangle.h"
#include "GLTextureRectangle.h"

#include "GLUtility.h"

// ----------------------------------------------------------------------------
GLRenderTexture2DRectangle *GLRenderTexture2DRectangle::New(
	int width, int height, int internalformat, int format, int type) 
{
	GLRenderTexture2DRectangle *rt = 
		new GLRenderTexture2DRectangle(width, height);
	// Allocate memory
	if (!rt->Allocate(internalformat, format, type)) 
	{
		delete rt;
		return 0;
	}

	return rt;
}

// ----------------------------------------------------------------------------
GLRenderTexture2DRectangle::GLRenderTexture2DRectangle(int width, int height) 
: GLRenderTexture2D(width, height) 
{ 
}

// ----------------------------------------------------------------------------
GLRenderTexture2DRectangle::~GLRenderTexture2DRectangle() 
{ 
}

// ----------------------------------------------------------------------------
bool GLRenderTexture2DRectangle::Allocate(
	int internalformat, int format, int type) 
{
	// Create an empty texture
	tex = GLTextureRectangle::New(width, height, 
		internalformat, format, type, 0);
	if (!tex) 
	{
		return false;
	}

	return true;
}
