#pragma once
#include "GLRendertarget.h"
#include "GLTexture3D.h"
#include "GL.h"

class GLRenderTexture3DLayer : public GLRendertarget 
{
public:
	static GLRenderTexture3DLayer *New(GLTexture3D *tex, int layer);

	virtual ~GLRenderTexture3DLayer();

	GLTexture3D* GetTexture() { return tex; }
	const GLTexture3D* GetTexture() const { return tex; }

	bool Resize(int width, int height) { return false; };

protected:
	GLTexture3D *tex;
	int layer;

	GLRenderTexture3DLayer(GLTexture3D *tex, int layer);

	virtual void AttachToBoundFBO(int attachment);

private:
	// Not implemented
	GLRenderTexture3DLayer(const GLRenderTexture3DLayer&);
	void operator=(const GLRenderTexture3DLayer&);
};
