#include "GLTimerQuery.h"

#include "GLUtility.h"

#ifndef NDEBUG
#include <iostream>
#endif

// Some support for older GLEW versions
#ifndef GLEW_EXT_timer_query
#define GL_TIME_ELAPSED_EXT GL_INVALID_VALUE
#endif

using namespace std;

// ----------------------------------------------------------------------------
GLTimerQuery *GLTimerQuery::New() 
{
#ifdef GLEW_EXT_timer_query
	if (GLEW_EXT_timer_query) 
	{
		return new GLTimerQuery();
	} 
	else 
#endif
	{
#ifndef NDEBUG
		cerr << "GLTimerQuery: Timer queries not supported!" << endl;
#endif
		return 0;
	}
}

// ----------------------------------------------------------------------------
GLTimerQuery::GLTimerQuery() 
: GLQuery(GL_TIME_ELAPSED_EXT)
{
	//cout << "GLTimerQuery: Constructor" << endl;
}

// ----------------------------------------------------------------------------
GLTimerQuery::~GLTimerQuery() 
{
	//cout << "GLTimerQuery: Destructor" << endl;
}

// ----------------------------------------------------------------------------
GLint64EXT GLTimerQuery::GetResulti64()
{
	GLint64EXT result = 0;
#ifdef GLEW_EXT_timer_query
	glGetQueryObjecti64vEXT(id, GL_QUERY_RESULT, &result);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: GetResult()");
#endif // NDEBUG
#endif // GLEW_EXT_timer_query
	return result;
}

// ----------------------------------------------------------------------------
GLuint64EXT GLTimerQuery::GetResultui64()
{
	GLuint64EXT result = 0;
#ifdef GLEW_EXT_timer_query
	glGetQueryObjectui64vEXT(id, GL_QUERY_RESULT, &result);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: GetResult()");
#endif // NDEBUG
#endif // GLEW_EXT_timer_query
	return result;
}
