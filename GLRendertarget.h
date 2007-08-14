#pragma once
#include "GLResource.h"
#include "GL.h"

class GLRendertarget : public GLResource {
public:
	friend class GLFramebuffer;

	virtual ~GLRendertarget();

protected:
	int width;
	int height;

	GLRendertarget(int width, int height);

	virtual void AttachToBoundFBO(int attachment) = 0;
};
