#pragma once
#include "GL.h"
#include "GLRendertarget.h"
#include "GLTexture.h"
#include <map>

class GLFramebuffer {
public:
	static GLFramebuffer *New(int width, int height);
	~GLFramebuffer();

	void AttachRendertarget(int attachment, GLRendertarget &rt);
	GLRendertarget *DetachRendertarget(int attachment);

	bool CreateDepthBuffer(int format = GL_DEPTH_COMPONENT);
	bool CreateDepthTexture(int format = GL_DEPTH_COMPONENT);
	bool CreateDepthTextureRectangle(int format = GL_DEPTH_COMPONENT);
	bool CreatePackedDepthStencilBuffer();
	bool CreatePackedDepthStencilTexture();
	bool CreatePackedDepthStencilTextureRectangle();

	void Bind();
	void Unbind();
	bool IsBound();

	int GetStatus();
	bool IsOk();

	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }

	GLTexture *GetTexture2D(int attachment);

private:
	GLFramebuffer(int width, int height);
	GLuint id;
	std::map<int, GLRendertarget*> attachments;

	int width;
	int height;

	bool bound;
};
