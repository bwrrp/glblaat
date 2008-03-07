#include "GLQuery.h"

#include "GLUtility.h"
#include <iostream>
#include <cassert>

using namespace std;

// ----------------------------------------------------------------------------
GLQuery::GLQuery(GLenum type) 
: id(0), target(type)
{
	//cout << "GLQuery: Constructor" << endl;
	glGenQueries(1, &id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLBuffer: glGenQueries");
#endif
}

// ----------------------------------------------------------------------------
GLQuery::~GLQuery() 
{
	//cout << "GLQuery: Destructor" << endl;
	glDeleteQueries(1, &id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: glDeleteQueries()");
#endif
}

// ----------------------------------------------------------------------------
void GLQuery::Start() 
{ 
	glBeginQuery(target, id);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: glBeginQuery()");
#endif
}

// ----------------------------------------------------------------------------
void GLQuery::Stop() 
{
	glEndQuery(target);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: glEndQuery()");
#endif
}

// ----------------------------------------------------------------------------
bool GLQuery::IsDone()
{
	int done = 0;
	glGetQueryObjectiv(id, GL_QUERY_RESULT_AVAILABLE, &done);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: IsDone()");
#endif
	return (done == GL_TRUE);
}

// ----------------------------------------------------------------------------
int GLQuery::GetCounterBits()
{
	int bits = 0;
	glGetQueryiv(target, GL_QUERY_COUNTER_BITS, &bits);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: GetCounterBits()");
#endif
	return bits;
}

// ----------------------------------------------------------------------------
int GLQuery::GetResulti()
{
	int result = 0;
	glGetQueryObjectiv(id, GL_QUERY_RESULT, &result);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: GetResult()");
#endif
	return result;
}

// ----------------------------------------------------------------------------
unsigned int GLQuery::GetResultui()
{
	unsigned int result = 0;
	glGetQueryObjectuiv(id, GL_QUERY_RESULT, &result);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: GetResult()");
#endif
	return result;
}
