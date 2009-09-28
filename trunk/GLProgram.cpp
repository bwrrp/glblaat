#include "GLProgram.h"

#include "GL.h"
#include "GLShader.h"
#include "GLVertexShader.h"
#include "GLGeometryShader.h"
#include "GLFragmentShader.h"

#include "GLUtility.h"

#ifndef NDEBUG
#include <iostream>
#endif

#include <cassert>

using namespace std;

// ----------------------------------------------------------------------------
GLProgram *GLProgram::New() 
{
	if (GLEW_VERSION_2_0) 
	{
#ifndef NDEBUG
		cout << "GLProgram: Using OpenGL 2.0" << endl;
#endif
		return new GLProgram();
	} 
	else if (GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader && 
		GLEW_ARB_fragment_shader) 
	{
		// TODO: add an ARB version of these classes as fallback
#ifndef NDEBUG
		cerr << "GLProgram: Falling back to ARB (not implemented!)" << endl;
#endif
		return 0;
	} 
	else 
	{
#ifndef NDEBUG
		cerr << "GLProgram: Shaders not supported!" << endl;
#endif
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLProgram::GLProgram() 
: inuse(false) 
{
	id = glCreateProgram();
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glCreateProgram()");
#endif
}

// ----------------------------------------------------------------------------
GLProgram::~GLProgram() 
{
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
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glDeleteProgram()");
#endif
}

// ----------------------------------------------------------------------------
bool GLProgram::AddVertexShader(const string &source) 
{
	GLShader *shader = GLVertexShader::New();
	if (!shader) return false;

	if (!shader->SetSource(source)) 
	{
#ifndef NDEBUG
		cerr << "GLProgram: Error compiling vertex shader, dumping log..." 
			<< endl;
		cerr << shader->GetInfoLog() << endl;
		cerr << "GLProgram: Vertex shader source:" << endl;
		cerr << shader->GetSource() << endl;
#endif
		return false;
	}

	AttachShader(*shader);
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::AddGeometryShader(const std::string &source)
{
	GLShader *shader = GLGeometryShader::New();
	if (!shader) return false;

	if (!shader->SetSource(source))
	{
#ifndef NDEBUG
		cerr << "GLProgram: Error compiling geometry shader, dumping log..." 
			<< endl;
		cerr << shader->GetInfoLog() << endl;
		cerr << "GLProgram: Geometry shader source:" << endl;
		cerr << shader->GetSource() << endl;
#endif
		return false;
	}

	AttachShader(*shader);
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::AddFragmentShader(const string &source) 
{
	GLShader *shader = GLFragmentShader::New();
	if (!shader) return false;

	if (!shader->SetSource(source)) 
	{
#ifndef NDEBUG
		cerr << "GLProgram: Error compiling fragment shader, dumping log..." 
			<< endl;
		cerr << shader->GetInfoLog() << endl;
		cerr << "GLProgram: Fragment shader source:" << endl;
		cerr << shader->GetSource() << endl;
#endif
		return false;
	}

	AttachShader(*shader);
	return true;
}

// ----------------------------------------------------------------------------
void GLProgram::SetParameteri(int param, int value)
{
	if (GL_GEOMETRY_SHADER_EXT)
	{
		glProgramParameteriEXT(id, param, value);
#ifndef NDEBUG
		GLUtility::CheckOpenGLError("GLProgram: SetParameteri()");
#endif
	}
}

// ----------------------------------------------------------------------------
void GLProgram::AttachShader(GLShader *shader) 
{
	if (shader) AttachShader(*shader);
}

// ----------------------------------------------------------------------------
void GLProgram::AttachShader(GLShader &shader) 
{
	glAttachShader(id, shader.id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: AttachShader()");
#endif
	shaders.insert(&shader);
}

// ----------------------------------------------------------------------------
void GLProgram::DetachShader(GLShader *shader) 
{
	if (shader) DetachShader(*shader);
}

// ----------------------------------------------------------------------------
void GLProgram::DetachShader(GLShader &shader) 
{
	glDetachShader(id, shader.id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: DetachShader()");
#endif
	shaders.erase(&shader);
}

// ----------------------------------------------------------------------------
bool GLProgram::Link() 
{
	glLinkProgram(id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: Link()");
#endif

	bool ok = IsOk();
	if (!ok) 
	{
#ifndef NDEBUG
		cerr << "GLProgram: Error linking shader program, dumping log..." 
			<< endl;
		cerr << GetInfoLog() << endl;
#endif
	}
	return ok;
}

// ----------------------------------------------------------------------------
bool GLProgram::IsOk() const 
{
	GLint ok = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &ok);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: IsOk()");
#endif

	return (ok == GL_TRUE);
}

// ----------------------------------------------------------------------------
string GLProgram::GetInfoLog() const 
{
	int loglength = 0;
	
	// Get the length of the log
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &loglength);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: GetInfoLog() - getting log length");
#endif
	if (loglength > 0) 
	{
		// Allocate a buffer and get the log
		int dummy;
		char *buffer = new char[loglength];
		glGetProgramInfoLog(id, loglength, &dummy, buffer);
#ifndef NDEBUG
		GLUtility::CheckOpenGLError("GLProgram: glGetProgramInfoLog()");
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

// ----------------------------------------------------------------------------
string GLProgram::GetInfoLogs() const 
{
	string log = "===Shader logs===\r\n";
	// Get shader logs
	for (set<GLShader*>::const_iterator i = shaders.begin(); 
		i != shaders.end(); ++i) 
	{
		log += (*i)->GetInfoLog();
		log += "\r\n-----------------\r\n";
	}
	// Get program log
	log += "===Program log===\r\n";
	log += GetInfoLog();

	return log;
}

// ----------------------------------------------------------------------------
void GLProgram::Start() 
{
	assert(!inuse);
	if (inuse) return;

	glUseProgram(id);
	inuse = true;
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: Start()");
#endif
}

// ----------------------------------------------------------------------------
void GLProgram::Stop() 
{
	assert(inuse);
	if (!inuse) return;

	glUseProgram(0);
	inuse = false;
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: Stop()");
#endif
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniform1f(const string &name, float v0) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniform1f() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniform1f(loc, v0);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniform1f");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniform2f(const string &name, float v0, float v1) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniform2f() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniform2f(loc, v0, v1);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniform2f");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniform3f(const string &name, float v0, float v1, float v2) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniform3f() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniform3f(loc, v0, v1, v2);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniform3f");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniform4f(const string &name, 
	float v0, float v1, float v2, float v3) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniform4f() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniform4f(loc, v0, v1, v2, v3);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniform4f");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniform1i(const string &name, int v0) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniform1i() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniform1i(loc, v0);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniform1i");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniform2i(const string &name, int v0, int v1) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniform2i() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniform2i(loc, v0, v1);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniform2i");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniform3i(const string &name, int v0, int v1, int v2) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniform3i() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniform3i(loc, v0, v1, v2);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniform3i");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniform4i(const string &name, 
	int v0, int v1, int v2, int v3) 
{ 
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniform4i() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniform4i(loc, v0, v1, v2, v3);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniform4i");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniformMatrix2fv(const std::string &name, 
	int count, float *v, bool transpose)
{
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniformMatrix2fv() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniformMatrix2fv(loc, count, (transpose ? 1 : 0), v);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniformMatrix2fv");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniformMatrix3fv(const std::string &name, 
	int count, float *v, bool transpose)
{
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniformMatrix3fv() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniformMatrix3fv(loc, count, (transpose ? 1 : 0), v);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniformMatrix3fv");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::SetUniformMatrix4fv(const std::string &name, 
	int count, float *v, bool transpose)
{
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: SetUniformMatrix4fv() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniformMatrix4fv(loc, count, (transpose ? 1 : 0), v);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: glUniformMatrix4fv");
#endif
	return true;
}

// ----------------------------------------------------------------------------
bool GLProgram::UseTexture(const string &name, int texunit) 
{
	GLint loc = glGetUniformLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: UseTexture() - glGetUniformLocation()");
#endif
	if (loc == -1) return false;

	glUniform1i(loc, texunit);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: UseTexture() - glUniform1i");
#endif
	return true;
}

// ----------------------------------------------------------------------------
// The program needs to be re-linked for the change to take effect
// Matrices are automatically bound as column vectors to index + i, i = 0..n
bool GLProgram::BindAttribLocation(const std::string &name, int index)
{
	int maxAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
	if (index < 0 || index >= maxAttribs)
		return false;

	glBindAttribLocation(id, index, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: BindAttribLocation()");
#endif
	return true;
}

// ----------------------------------------------------------------------------
// Returns the assigned location of the given attribute, or -1 if not found
// The program needs to be linked for this to work
int GLProgram::GetAttribLocation(const std::string &name)
{
	int loc = glGetAttribLocation(id, name.c_str());
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLProgram: GetAttribLocation()");
#endif
	return loc;
}

// ----------------------------------------------------------------------------
// Returns information on the active attributes in the current program
std::vector<GLAttributeInfo> GLProgram::GetActiveAttributes()
{
	std::vector<GLAttributeInfo> attribs;

	// Check if program is OK first
	if (!IsOk())
	{
#ifndef NDEBUG
		cerr << "GLProgram: Program not ok, can't enumerate attributes..." 
			<< endl;
#endif
		return attribs;
	}

	// Get number of active attributes and max attribute name length
	int numAttribs;
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &numAttribs);
	int maxAttribNameLength;
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: Getting number of active attributes");
#endif

	// Temporary buffer for attribute names
	char *name = new char[maxAttribNameLength];
	unsigned int type = GL_NONE;
	int size = 0;
	int length = 0;

	// Enumerate attributes
	for (int i = 0; i < numAttribs; ++i)
	{
		// Get attrib info
		glGetActiveAttrib(id, i, maxAttribNameLength, 
			&length, &size, &type, name);
		// Add to list
		GLAttributeInfo attrib;
		attrib.size = size;
		attrib.type = type;
		attrib.name = std::string(name);
		attrib.index = GetAttribLocation(attrib.name);
		attribs.push_back(attrib);
	}

	// Clean up and return
	delete [] name;

	return attribs;
}

// ----------------------------------------------------------------------------
// Returns information on the active uniforms in the current program
std::vector<GLUniformInfo> GLProgram::GetActiveUniforms()
{
	std::vector<GLUniformInfo> uniforms;

	// Check if program is OK first
	if (!IsOk())
	{
#ifndef NDEBUG
		cerr << "GLProgram: Program not ok, can't enumerate uniforms..." 
			<< endl;
#endif
		return uniforms;
	}

	// Get number of active uniforms and max uniform name length
	int numUniforms;
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numUniforms);
	int maxUniformNameLength;
	glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError(
		"GLProgram: Getting number of active uniforms");
#endif

	// Temporary buffer for uniform names
	char *name = new char[maxUniformNameLength];
	unsigned int type = GL_NONE;
	int size = 0;
	int length = 0;

	// Enumerate uniforms
	for (int i = 0; i < numUniforms; ++i)
	{
		// Get uniform info
		glGetActiveUniform(id, i, maxUniformNameLength, 
			&length, &size, &type, name);
		// Add to list
		GLUniformInfo uniform;
		uniform.size = size;
		uniform.type = type;
		uniform.name = std::string(name);
		uniforms.push_back(uniform);
	}

	// Clean up and return
	delete [] name;

	return uniforms;
}
