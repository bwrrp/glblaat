#pragma once
#include "GLResource.h"
#include "GLProgram.h"
#include "GL.h"
#include <string>

class GLShader : public GLResource {
public:
	friend class GLProgram;

	//GLShader();
	virtual ~GLShader();

	std::string GetSource() const;
	bool SetSource(const std::string &source);

	bool IsOk() const;
	std::string GetInfoLog() const;

protected:
	GLShader(GLenum type);

private:
	GLuint id;
	GLenum type;

	std::string source;
};
