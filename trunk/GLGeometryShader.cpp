// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#include "GLGeometryShader.h"

#include "GL.h"

#include <iostream>

// ----------------------------------------------------------------------------
GLGeometryShader *GLGeometryShader::New()
{
	if (GL_GEOMETRY_SHADER_EXT)
	{
		return new GLGeometryShader();
	}
	else
	{
		std::cerr << 
			"GLGeometryShader: Geometry shaders not supported!" << std::endl;
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLGeometryShader::GLGeometryShader() 
: GLShader(GL_GEOMETRY_SHADER_EXT) 
{
}

// ----------------------------------------------------------------------------
GLGeometryShader::~GLGeometryShader() 
{
}
