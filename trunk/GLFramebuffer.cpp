#include "GLFramebuffer.h"

#include "GL.h"
#include "GLRenderbuffer.h"
#include "GLRenderTexture2D.h"
#include "GLRenderTexture2DRectangle.h"

#include <iostream>
#include "GLUtility.h"

using namespace std;

// Need to use this factory to create FBOs
GLFramebuffer* GLFramebuffer::New(int width, int height) 
{
	// Check for framebuffer support
	if (!GLEW_EXT_framebuffer_object) 
	{
		// Oops, not supported
		cerr << "GLFramebuffer: Framebuffer objects not supported!" << endl;
		return 0;
	}

	// Create and return new FBO
	return new GLFramebuffer(width, height);
}

GLFramebuffer::GLFramebuffer(int width, int height) 
: id(0), width(width), height(height), bound(false) 
{
	// Create the framebuffer
	//cout << "GLFramebuffer: Constructor" << endl;
	glGenFramebuffersEXT(1, &id);
	GLUtility::CheckOpenGLError("GLFramebuffer: glGenFramebuffersEXT()");
}

GLFramebuffer::~GLFramebuffer() 
{
	//cout << "GLFramebuffer: Destructor" << endl;
	
	// Delete all attachments
	for (map<int, GLRendertarget*>::iterator i = attachments.begin(); i != attachments.end(); ) 
	{
		int attachment = i->first;
		i++;
		GLRendertarget *rt = DetachRendertarget(attachment);
		// Delete the rendertarget if it's no longer attached anywhere
		if (rt->GetTimesAttached() == 0) delete rt;
	}
	// Delete the framebuffer
	glDeleteFramebuffersEXT(1, &id);
	GLUtility::CheckOpenGLError("GLFramebuffer: glDeleteFramebuffersEXT()");
}

GLRendertarget *GLFramebuffer::AttachRendertarget(int attachment, GLRendertarget &rt) 
{
	if (!bound) Bind();

	// Remove the old render target (if any)
	GLRendertarget *oldRt = DetachRendertarget(attachment);

	// Attach the new target
	rt.AttachToBoundFBO(attachment);
	attachments[attachment] = &rt;

	// Return the old attachment (or null)
	return oldRt;
}

GLRendertarget* GLFramebuffer::DetachRendertarget(int attachment) 
{
	if (!bound) Bind();

	// Release the old render target (if any) to the caller and remove it from the map
	GLRendertarget *rt = 0;
	map<int, GLRendertarget*>::iterator it = attachments.find(attachment);
	if (it != attachments.end()) 
	{
		rt = it->second;
		rt->DetachFromBoundFBO(attachment);
	}
	attachments.erase(attachment);

	return rt;
}

bool GLFramebuffer::CreateDepthBuffer(int format) 
{
	GLRendertarget *depthbuffer = GLRenderbuffer::New(width, height, format);
	AttachRendertarget(GL_DEPTH_ATTACHMENT_EXT, *depthbuffer);

	return true;
}

bool GLFramebuffer::CreateDepthTexture(int format) 
{
	GLRendertarget *depthbuffer = GLRenderTexture2D::New(width, height, 
		format, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
	AttachRendertarget(GL_DEPTH_ATTACHMENT_EXT, *depthbuffer);

	return true;
}

bool GLFramebuffer::CreateDepthTextureRectangle(int format) 
{
	if (!GLEW_ARB_texture_rectangle) return false;

	GLRendertarget *depthbuffer = GLRenderTexture2DRectangle::New(
		width, height, format, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
	AttachRendertarget(GL_DEPTH_ATTACHMENT_EXT, *depthbuffer);

	return true;
}

bool GLFramebuffer::CreatePackedDepthStencilBuffer() 
{
	if (!GLEW_EXT_packed_depth_stencil) return false;

	GLRendertarget *dsb = GLRenderbuffer::New(width, height, GL_DEPTH24_STENCIL8_EXT);
	AttachRendertarget(GL_DEPTH_ATTACHMENT_EXT, *dsb);
	AttachRendertarget(GL_STENCIL_ATTACHMENT_EXT, *dsb);

	return true;
}

bool GLFramebuffer::CreatePackedDepthStencilTexture() 
{
	if (!GLEW_EXT_packed_depth_stencil) return false;
	if (!GLEW_ARB_depth_texture && !GLEW_SGIX_depth_texture) return false;

	GLRendertarget *dsb = GLRenderTexture2D::New(width, height, 
		GL_DEPTH24_STENCIL8_EXT, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT);
	AttachRendertarget(GL_DEPTH_ATTACHMENT_EXT, *dsb);
	AttachRendertarget(GL_STENCIL_ATTACHMENT_EXT, *dsb);

	return true;
}

bool GLFramebuffer::CreatePackedDepthStencilTextureRectangle() 
{
	if (!GLEW_ARB_texture_rectangle) return false;
	if (!GLEW_EXT_packed_depth_stencil) return false;
	if (!GLEW_ARB_depth_texture && !GLEW_SGIX_depth_texture) return false;

	GLRendertarget *dsb = GLRenderTexture2DRectangle::New(width, height, 
		GL_DEPTH24_STENCIL8_EXT, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT);
	AttachRendertarget(GL_DEPTH_ATTACHMENT_EXT, *dsb);
	AttachRendertarget(GL_STENCIL_ATTACHMENT_EXT, *dsb);

	return true;
}

void GLFramebuffer::Bind() 
{
	if (bound) 
	{
		cerr << "GLFramebuffer: Bind() called, but framebuffer was already bound" << endl;
		return;
	}

	// Store old viewport
	glPushAttrib(GL_VIEWPORT_BIT);
	// Bind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	// Set viewport
	glViewport(0, 0, width, height);
	bound = true;
	GLUtility::CheckOpenGLError("GLFramebuffer: Bind()");
}

void GLFramebuffer::Unbind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	if (bound) 
	{
		// Restore viewport
		glPopAttrib();
	} 
	else 
	{
		cerr << "GLFramebuffer: Unbind() called, but framebuffer was not bound" << endl;
	}

	bound = false;

	GLUtility::CheckOpenGLError("GLFramebuffer: Unbind()");
}

bool GLFramebuffer::IsBound() 
{
	return bound;
}

int GLFramebuffer::GetStatus() 
{
	if (!bound) Bind();
	return glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
}

bool GLFramebuffer::IsOk() 
{
	int status = GetStatus();
	switch (status) 
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return true;
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			return false;
			break;
		default:
			//assert(false); // Error in framebuffer code!
			return false;
			break;
	}
}

GLTexture* GLFramebuffer::GetTexture2D(int attachment) 
{
	// Try to cast, return null if that attachment isn't a rendertarget
	GLRenderTexture2D *rt = dynamic_cast<GLRenderTexture2D*>(attachments[attachment]);
	if (!rt) return 0;
	// Return texture
	return rt->GetTexture();
}

bool GLFramebuffer::Resize(int width, int height)
{
	// Resize all attachments
	for (map<int, GLRendertarget*>::iterator i = attachments.begin(); i != attachments.end(); ) 
	{
		int attachment = i->first;
		i++;
		// We need to detach, because the internal rendertarget might change
		GLRendertarget *rt = DetachRendertarget(attachment);
		if (!rt->Resize(width, height))
		{
			cerr << "GLFrameBuffer: Resize failed for attachment " << attachment << endl;
			delete rt;
			return false;
		}
		// Re-attach the rendertarget
		AttachRendertarget(attachment, *rt);
	}

	// Update framebuffer size
	this->width = width;
	this->height = height;

	// Unbind the framebuffer if bound
	if (bound) Unbind();

	return true;
}
