// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLShader.h"

#include "GL.h"

class GLGeometryShader : public GLShader 
{
public:
	static GLGeometryShader *New();
	virtual ~GLGeometryShader();

protected:
	GLGeometryShader();

private:
	// Not implemented
	GLGeometryShader(const GLGeometryShader&);
	void operator=(const GLGeometryShader&);
};
