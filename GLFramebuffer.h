#pragma once
#include "GLResource.h"
#include "GL.h"
#include "GLRendertarget.h"
#include "GLTexture.h"
#include <map>

class GLFramebuffer : public GLResource 
{
public:
	static GLFramebuffer *New(int width, int height);
	~GLFramebuffer();

	GLRendertarget *AttachRendertarget(int attachment, GLRendertarget *rt);
	GLRendertarget *AttachRendertarget(int attachment, GLRendertarget &rt);
	GLRendertarget *DetachRendertarget(int attachment);

	bool CreateDepthBuffer(int format = GL_DEPTH_COMPONENT);
	bool CreateDepthTexture(int format = GL_DEPTH_COMPONENT);
	bool CreateDepthTextureRectangle(int format = GL_DEPTH_COMPONENT);
	bool CreatePackedDepthStencilBuffer();
	bool CreatePackedDepthStencilTexture();
	bool CreatePackedDepthStencilTextureRectangle();
	
	bool CreateColorBuffer(
		int attachment = GL_COLOR_ATTACHMENT0_EXT, 
		int format = GL_RGBA);
	bool CreateColorTexture(
		int attachment = GL_COLOR_ATTACHMENT0_EXT, 
		int internalformat = GL_RGBA8, 
		int format = GL_RGBA, 
		int type = GL_UNSIGNED_BYTE);
	bool CreateColorTextureRectangle(
		int attachment = GL_COLOR_ATTACHMENT0_EXT, 
		int internalformat = GL_RGBA8, 
		int format = GL_RGBA, 
		int type = GL_UNSIGNED_BYTE);

	void Bind();
	void Unbind();
	bool IsBound() const { return bound; }

	int GetStatus();
	bool IsOk();

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	GLTexture *GetTexture2D(int attachment);
	const GLTexture *GetTexture2D(int attachment) const;

	bool Resize(int width, int height);

protected:
	GLFramebuffer(int width, int height);
	GLuint id;
	std::map<int, GLRendertarget*> attachments;

	int width;
	int height;

	bool bound;

private:
	// Not implemented
	GLFramebuffer(const GLFramebuffer&);
	void operator=(const GLFramebuffer&);
};
