#include "GLRenderTexture3DLayer.h"

#include "GLUtility.h"

// ----------------------------------------------------------------------------
GLRenderTexture3DLayer *GLRenderTexture3DLayer::New(
	GLTexture3D *tex, int layer) 
{
	if (!tex) return false;
	if (layer < 0 || layer >= tex->GetDepth()) return false;

	GLRenderTexture3DLayer *rt = new GLRenderTexture3DLayer(tex, layer);
	return rt;
}

// ----------------------------------------------------------------------------
GLRenderTexture3DLayer::GLRenderTexture3DLayer(GLTexture3D *tex, int layer)
: GLRendertarget(tex->GetWidth(), tex->GetHeight()), tex(tex), layer(layer)
{
}

// ----------------------------------------------------------------------------
GLRenderTexture3DLayer::~GLRenderTexture3DLayer() 
{
}

// ----------------------------------------------------------------------------
void GLRenderTexture3DLayer::AttachToBoundFBO(int attachment) 
{
	GLRendertarget::AttachToBoundFBO(attachment);

	glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachment, 
		tex->GetTextureTarget(), tex->id, 0, layer);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLRenderTexture3DLayer: AttachToBoundFBO()");
#endif
}
