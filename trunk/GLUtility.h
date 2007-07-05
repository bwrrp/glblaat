#pragma once
#include <string>

class GLUtility {
public:
	~GLUtility();

	// Error handling
	static void CheckOpenGLError(const std::string &task = "");
	static void ClearOpenGLError();
	static bool GetErrorFlag();

private:
	GLUtility(); // Class can not be instantiated

	static bool ErrorFlag;
};
