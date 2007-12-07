#include "GLShader.h"

#include "GLUtility.h"

using namespace std;

GLShader::GLShader(GLenum type) 
: id(0), type(type) 
{
	id = glCreateShader(type);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLShader: glCreateShader()");
#endif
}

GLShader::~GLShader() 
{
	glDeleteShader(id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLShader: glDeleteShader()");
#endif
}

string GLShader::GetSource() const 
{
	return source;
}

bool GLShader::SetSource(const string &source) 
{
	this->source = source;

	// Need a char** for glShaderSource
	const char *s = source.c_str();
	glShaderSource(id, 1, &s, 0);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLShader: glShaderSource()");
#endif

	// Compile the shader
	glCompileShader(id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLShader: glCompileShader()");
#endif

	return IsOk();
}

bool GLShader::IsOk() const 
{
	GLint ok = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLShader: IsOk()");
#endif

	return (ok == GL_TRUE);
}

string GLShader::GetInfoLog() const 
{
	int loglength = 0;
	
	// Get the length of the log
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &loglength);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLShader: GetInfoLog - getting log length");
#endif
	if (loglength > 0) 
	{
		// Allocate a buffer and get the log
		int dummy;
		char *buffer = new char[loglength];
		glGetShaderInfoLog(id, loglength, &dummy, buffer);
#ifndef NDEBUG
		GLUtility::CheckOpenGLError("GLShader: glGetShaderInfoLog()");
#endif
		string infolog(buffer);
		delete[] buffer;

		return infolog;
	} 
	else 
	{
		// No log...
		return string("");
	}
}
