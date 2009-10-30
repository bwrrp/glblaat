// Copyright (c) 2009 Stef Busking
// Distributed under the terms of the MIT License.

#pragma once
#include "GLResource.h"
#include "GLProgram.h"
#include "GL.h"
#include <string>

class GLShader : public GLResource 
{
public:
	friend class GLProgram;

	//GLShader();
	virtual ~GLShader();

	std::string GetSource() const;
	bool SetSource(const std::string &source);

	bool IsOk() const;
	std::string GetInfoLog() const;

protected:
	GLuint id;
	GLenum type;

	std::string source;

	GLShader(GLenum type);

private:
	// Not implemented
	GLShader(const GLShader&);
	void operator=(const GLShader&);
};
