// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLShader.h"
#include "GL.h"

class GLFragmentShader : public GLShader 
{
public:
	static GLFragmentShader *New() { return new GLFragmentShader(); }
	virtual ~GLFragmentShader();

protected:
	GLFragmentShader();

private:
	// Not implemented
	GLFragmentShader(const GLFragmentShader&);
	void operator=(const GLFragmentShader&);
};
