// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLShader.h"

#include "GL.h"

class GLVertexShader : public GLShader 
{
public:
	static GLVertexShader *New() { return new GLVertexShader(); }
	virtual ~GLVertexShader();

protected:
	GLVertexShader();

private:
	// Not implemented
	GLVertexShader(const GLVertexShader&);
	void operator=(const GLVertexShader&);
};
