#pragma once
#include "GLShader.h"
#include "GL.h"

class GLFragmentShader : public GLShader 
{
public:
	GLFragmentShader();
	virtual ~GLFragmentShader();

private:
	// Not implemented
	GLFragmentShader(const GLFragmentShader&);
	void operator=(const GLFragmentShader&);
};
