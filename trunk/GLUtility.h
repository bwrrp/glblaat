// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include <string>

class GLTexture;
class GLTextureRectangle;

class GLUtility 
{
public:
	~GLUtility();

	// Buffer copying
	static GLTexture *GrabColorBuffer(int vpx, int vpy, 
		int vpwidth, int vpheight, GLTexture *oldtex = 0);
	static GLTextureRectangle *GrabColorBufferRectangle(int vpx, int vpy, 
		int vpwidth, int vpheight, GLTextureRectangle *oldtex = 0);
	static GLTexture *GrabDepthBuffer(int vpx, int vpy, 
		int vpwidth, int vpheight, GLTexture *oldtex = 0);
	static GLTextureRectangle *GrabDepthBufferRectangle(int vpx, int vpy, 
		int vpwidth, int vpheight, GLTextureRectangle *oldtex = 0);

	// Default parameters for grab textures
	static void SetDefaultColorTextureParameters(GLTexture *tex);
	static void SetDefaultDepthTextureParameters(GLTexture *tex);

	// Error handling
	static void CheckOpenGLError(const std::string &task = "");
	static void ClearOpenGLError();
	static bool GetErrorFlag();

private:
	GLUtility(); // Class can not be instantiated

	static bool ErrorFlag;
};
