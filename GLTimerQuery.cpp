#include "GLTimerQuery.h"

#include "GLUtility.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------------
GLTimerQuery *GLTimerQuery::New() 
{
	if (GLEW_EXT_timer_query) 
	{
		return new GLTimerQuery();
	} 
	else 
	{
		cerr << "GLTimerQuery: Timer queries not supported!" << endl;
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
	GLint64EXT result;
	glGetQueryObjecti64vEXT(id, GL_QUERY_RESULT, &result);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: GetResult()");
#endif
	return result;
}

// ----------------------------------------------------------------------------
GLuint64EXT GLTimerQuery::GetResultui64()
{
	GLuint64EXT result;
	glGetQueryObjectui64vEXT(id, GL_QUERY_RESULT, &result);
#ifndef NDEBUG
	GLUtility::CheckOpenGLError("GLQuery: GetResult()");
#endif
	return result;
}
