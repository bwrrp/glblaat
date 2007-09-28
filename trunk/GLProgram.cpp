#include "GLProgram.h"

#include "GL.h"
#include "GLShader.h"
#include "GLVertexShader.h"
#include "GLFragmentShader.h"

#include <iostream>
#include "GLUtility.h"

using namespace std;

GLProgram *GLProgram::New() 
{
	if (GLEW_VERSION_2_0) 
	{
		cout << "GLProgram: Using OpenGL 2.0" << endl;
		return new GLProgram();
	} 
	else if (GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) 
	{
		// TODO: add an ARB version of these classes as fallback
		cerr << "GLProgram: Falling back to ARB (not implemented!)" << endl;
		return 0;
	} 
	else 
	{
		cerr << "GLProgram: Shaders not supported!" << endl;
		return 0;
	}
}

GLProgram::GLProgram() 
: inuse(false) 
{
	//cout << "GLProgram: Constructor" << endl;
	id = glCreateProgram();
	GLUtility::CheckOpenGLError("GLProgram: glCreateProgram()");
}

GLProgram::~GLProgram() 
{
	//cout << "GLProgram: Destructor" << endl;
	for (set<GLShader*>::iterator i = shaders.begin(); i != shaders.end();) 
	{
		// Grab shader reference
		GLShader *shader = *i;
		// Increase iterator to prevent invalidating it
		++i;
		// Detach shader
		DetachShader(*shader);
		// Delete shader
		delete shader;
	}
	shaders.clear();

	// Delete the program
	glDeleteProgram(id);
	GLUtility::CheckOpenGLError("GLProgram: glDeleteProgram()");
}

bool GLProgram::AddVertexShader(const string &source) 
{
	GLShader *shader = new GLVertexShader();
	if (!shader->SetSource(source)) 
	{
		cerr << "GLProgram: Error compiling vertex shader, dumping infolog..." << endl;
		cerr << shader->GetInfoLog() << endl;
		cerr << "GLProgram: Vertex shader source:" << endl;
		cerr << shader->GetSource() << endl;
		return false;
	}

	AttachShader(*shader);
	return true;
}

bool GLProgram::AddFragmentShader(const string &source) 
{
	GLShader *shader = new GLFragmentShader();
	if (!shader->SetSource(source)) 
	{
		cerr << "GLProgram: Error compiling fragment shader, dumping infolog..." << endl;
		cerr << shader->GetInfoLog() << endl;
		cerr << "GLProgram: Fragment shader source:" << endl;
		cerr << shader->GetSource() << endl;
		return false;
	}

	AttachShader(*shader);
	return true;
}

void GLProgram::AttachShader(GLShader &shader) 
{
	glAttachShader(id, shader.id);
	GLUtility::CheckOpenGLError("GLProgram: AttachShader()");
	shaders.insert(&shader);
	//cout << "GLProgram: Attached shader" << endl;
}

void GLProgram::DetachShader(GLShader &shader) 
{
	glDetachShader(id, shader.id);
	GLUtility::CheckOpenGLError("GLProgram: DetachShader()");
	shaders.erase(&shader);
	//cout << "GLProgram: Detached shader" << endl;
}

bool GLProgram::Link() 
{
	glLinkProgram(id);
	GLUtility::CheckOpenGLError("GLProgram: Link()");

	bool ok = IsOk();
	if (!ok) 
	{
		cerr << "GLProgram: Error linking shader program, dumping infolog..." << endl;
		cerr << GetInfoLog() << endl;
	}
	return ok;
}

bool GLProgram::IsOk() const 
{
	GLint ok = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &ok);
	GLUtility::CheckOpenGLError("GLProgram: IsOk()");

	return (ok == GL_TRUE);
}

string GLProgram::GetInfoLog() const 
{
	int loglength = 0;
	
	// Get the length of the log
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &loglength);
	GLUtility::CheckOpenGLError("GLProgram: GetInfoLog() - getting log length");
	if (loglength > 0) 
	{
		// Allocate a buffer and get the log
		int dummy;
		char *buffer = new char[loglength];
		glGetProgramInfoLog(id, loglength, &dummy, buffer);
		GLUtility::CheckOpenGLError("GLProgram: glGetProgramInfoLog()");
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

string GLProgram::GetInfoLogs() const 
{
	string log = "===Shader logs===\r\n";
	// Get shader logs
	for (set<GLShader*>::const_iterator i = shaders.begin(); i != shaders.end(); ++i) 
	{
		log += (*i)->GetInfoLog();
		log += "\r\n-----------------\r\n";
	}
	// Get program log
	log += "===Program log===\r\n";
	log += GetInfoLog();

	return log;
}

void GLProgram::Start() 
{
	if (inuse) 
	{
		cerr << "GLProgram: Start() called, but program already in use" << endl;
		return;
	}

	glUseProgram(id);
	inuse = true;
	//cout << "GLProgram: Started" << endl;
	GLUtility::CheckOpenGLError("GLProgram: Start()");
}

void GLProgram::Stop() 
{
	if (!inuse) 
	{
		cerr << "GLProgram: Stop() called, but program not in use" << endl;
		return;
	}

	glUseProgram(0);
	inuse = false;
	//cout << "GLProgram: Stopped" << endl;
	GLUtility::CheckOpenGLError("GLProgram: Stop()");
}

bool GLProgram::SetUniform1f(const string &name, float v0) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLUtility::CheckOpenGLError("GLProgram: SetUniform1f() - glGetUniformLocation()");
	if (loc == -1) return false;

	glUniform1f(loc, v0);
	GLUtility::CheckOpenGLError("GLProgram: glUniform1f");
	return true;
}

bool GLProgram::SetUniform2f(const string &name, float v0, float v1) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLUtility::CheckOpenGLError("GLProgram: SetUniform2f() - glGetUniformLocation()");
	if (loc == -1) return false;

	glUniform2f(loc, v0, v1);
	GLUtility::CheckOpenGLError("GLProgram: glUniform2f");
	return true;
}

bool GLProgram::SetUniform3f(const string &name, float v0, float v1, float v2) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLUtility::CheckOpenGLError("GLProgram: SetUniform3f() - glGetUniformLocation()");
	if (loc == -1) return false;

	glUniform3f(loc, v0, v1, v2);
	GLUtility::CheckOpenGLError("GLProgram: glUniform3f");
	return true;
}

bool GLProgram::SetUniform4f(const string &name, float v0, float v1, float v2, float v3) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLUtility::CheckOpenGLError("GLProgram: SetUniform4f() - glGetUniformLocation()");
	if (loc == -1) return false;

	glUniform4f(loc, v0, v1, v2, v3);
	GLUtility::CheckOpenGLError("GLProgram: glUniform4f");
	return true;
}

bool GLProgram::SetUniform1i(const string &name, int v0) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLUtility::CheckOpenGLError("GLProgram: SetUniform1i() - glGetUniformLocation()");
	if (loc == -1) return false;

	glUniform1i(loc, v0);
	GLUtility::CheckOpenGLError("GLProgram: glUniform1i");
	return true;
}

bool GLProgram::SetUniform2i(const string &name, int v0, int v1) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLUtility::CheckOpenGLError("GLProgram: SetUniform2i() - glGetUniformLocation()");
	if (loc == -1) return false;

	glUniform2i(loc, v0, v1);
	GLUtility::CheckOpenGLError("GLProgram: glUniform2i");
	return true;
}

bool GLProgram::SetUniform3i(const string &name, int v0, int v1, int v2) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLUtility::CheckOpenGLError("GLProgram: SetUniform3i() - glGetUniformLocation()");
	if (loc == -1) return false;

	glUniform3i(loc, v0, v1, v2);
	GLUtility::CheckOpenGLError("GLProgram: glUniform3i");
	return true;
}

bool GLProgram::SetUniform4i(const string &name, int v0, int v1, int v2, int v3) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLUtility::CheckOpenGLError("GLProgram: SetUniform4i() - glGetUniformLocation()");
	if (loc == -1) return false;

	glUniform4i(loc, v0, v1, v2, v3);
	GLUtility::CheckOpenGLError("GLProgram: glUniform4i");
	return true;
}

bool GLProgram::UseTexture(const string &name, int texunit) 
{
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLUtility::CheckOpenGLError("GLProgram: UseTexture() - glGetUniformLocation()");
	if (loc == -1) return false;

	glUniform1i(loc, texunit);
	GLUtility::CheckOpenGLError("GLProgram: UseTexture() - glUniform1i");
	return true;
}
