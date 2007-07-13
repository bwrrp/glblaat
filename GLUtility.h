#pragma once
#include <string>

class GLTextureRectangle;

class GLUtility {
public:
	~GLUtility();

	// Buffer copying
	static GLTextureRectangle *GrabColorBuffer(int vpx, int vpy, int vpwidth, int vpheight);
	static GLTextureRectangle *GrabDepthBuffer(int vpx, int vpy, int vpwidth, int vpheight);

	// Error handling
	static void CheckOpenGLError(const std::string &task = "");
	static void ClearOpenGLError();
	static bool GetErrorFlag();

private:
	GLUtility(); // Class can not be instantiated

	static bool ErrorFlag;
};
