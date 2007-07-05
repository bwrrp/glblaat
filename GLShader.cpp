#include "GLShader.h"

#include "GLUtility.h"

using namespace std;

GLShader::GLShader(GLenum type) 
: id(0), type(type) {
	id = glCreateShader(type);
	GLUtility::CheckOpenGLError("GLShader: glCreateShader()");
}

GLShader::~GLShader() {
	glDeleteShader(id);
	GLUtility::CheckOpenGLError("GLShader: glDeleteShader()");
}

string GLShader::GetSource() const {
	return source;
}

bool GLShader::SetSource(const string &source) {
	this->source = source;

	// Need a char** for glShaderSource
	const char *s = source.c_str();
	glShaderSource(id, 1, &s, 0);
	GLUtility::CheckOpenGLError("GLShader: glShaderSource()");

	// Compile the shader
	glCompileShader(id);
	GLUtility::CheckOpenGLError("GLShader: glCompileShader()");

	return IsOk();
}

bool GLShader::IsOk() const {
	GLint ok = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
	GLUtility::CheckOpenGLError("GLShader: IsOk()");

	return (ok == GL_TRUE);
}

string GLShader::GetInfoLog() const {
	int loglength = 0;
	
	// Get the length of the log
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &loglength);
	GLUtility::CheckOpenGLError("GLShader: GetInfoLog - getting log length");
	if (loglength > 0) {
		// Allocate a buffer and get the log
		int dummy;
		char *buffer = new char[loglength];
		glGetShaderInfoLog(id, loglength, &dummy, buffer);
		GLUtility::CheckOpenGLError("GLShader: glGetShaderInfoLog()");
		string infolog(buffer);
		delete[] buffer;

		return infolog;
	} else {
		// No log...
		return string("");
	}
}
