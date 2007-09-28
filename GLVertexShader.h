#pragma once
#include "GLShader.h"

#include "GL.h"

class GLVertexShader : public GLShader 
{
public:
	GLVertexShader();
	virtual ~GLVertexShader();

private:
	// Not implemented
	GLVertexShader(const GLVertexShader&);
	void operator=(const GLVertexShader&);
};
